#include "UscxmlClibBase.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

#include "UscxmlClibLogger.h"
#include "UscxmlLuaDataModelEx.h"

void issueLocationsForXML(uscxml::Interpreter *interpreter) {
	std::list<InterpreterIssue> issues = interpreter->validate();

	bool bIsError = false;

	for (std::list<InterpreterIssue>::iterator issueIter = issues.begin(); issueIter != issues.end(); issueIter++) {
		switch (issueIter->severity) {
		case InterpreterIssue::USCXML_ISSUE_FATAL: {
			std::stringstream ss;
			ss << *issueIter;
			LOG(interpreter->getLogger(), USCXML_ERROR) << ss.str();
			bIsError = true;
		} break;
		case InterpreterIssue::USCXML_ISSUE_INFO: {
			std::stringstream ss;
			ss << *issueIter;
			LOG(interpreter->getLogger(), USCXML_INFO) << ss.str();
		} break;
		case InterpreterIssue::USCXML_ISSUE_WARNING: {
			std::stringstream ss;
			ss << *issueIter;
			LOG(interpreter->getLogger(), USCXML_WARN) << ss.str();
		} break;
		}
	}
	if (bIsError)
		throw std::runtime_error("Fatal SCXML error! Interpreter stopped!");
}

ScxmlBase::ScxmlBase(const std::vector<std::string> &ACMDArgs,
	const std::set<TScxmlMsgType> &AMonitorMessages,
	const bool bMonitor/* = true*/,
	const std::string sRemoteHost/* = "127.0.0.1"*/,
	const int iRemotePort/* = SCXML_BASE_DISABLE_REMOTE_MONITOR*/, const bool bCheckIssues/* = true*/,
	const bool bHttpEnabled/* = false*/):
	_remotePort(iRemotePort), _remoteHost(sRemoteHost), _monitor(bMonitor),
	_CMDArgs(ACMDArgs), _validate(bCheckIssues), _Messages(AMonitorMessages), _httpEnabled(bHttpEnabled) {
	
}

ScxmlBase::~ScxmlBase(void)
{
	close();
}

void  ScxmlBase::start(const std::string &sTextOrFile, const bool bIsText) {	
	/// possible deadlock - think in future for replace!
	waitForStopped();
	
	if (bIsText) {
		_interpreter = Interpreter::fromXML(sTextOrFile, "");
	}
	else {
		_scxmlurl = sTextOrFile;
		_interpreter = Interpreter::fromURL(_scxmlurl);
	}

	if (!_interpreter)
		throw std::exception("Interpreter initialization FAILED!");

	setState(USCXML_UNDEF);

	_factory_ptr.reset(new FactoryDynamic(&Factory::getInstance()));

	std::set<Factory::PluginType> FACTORY_TYPES{
		Factory::PluginType::ptSCXMLIOProcessor,
		Factory::PluginType::ptLuaDataModel,
		Factory::PluginType::ptNullDataModel,
		Factory::PluginType::ptUSCXMLInvoker
	};

	if (_httpEnabled) {
		FACTORY_TYPES.insert(Factory::PluginType::ptBasicHTTPIOProcessor);
	}

	_factory_ptr->registerCustomPlugins(FACTORY_TYPES);
	
	_factory_ptr->registerExecutableContent(std::shared_ptr<SetValueExecutableContent>(new SetValueExecutableContent(this)));
	_factory_ptr->registerIOProcessor(std::shared_ptr<GlobalDataIOProcessor>(new GlobalDataIOProcessor(this)));
	_factory_ptr->registerDataModel(std::shared_ptr<LuaDataModelEx>(new LuaDataModelEx(this)));

	_interpreter.setFactory(_factory_ptr.get());
	
	_queue_ptr.reset(new PausableDelayedEventQueue(_interpreter.getImpl().get(),this));
	_lang.delayQueue = DelayedEventQueue(_queue_ptr);
	/* intercept native logger in any case */
	_lang.logger = std::shared_ptr<LoggerImpl>(new CLibLogger(_monitor, this, _OnInterpreterLog, _OnInterpreterLogUser));
	_interpreter.setActionLanguage(_lang);

	if (_validate) {
		issueLocationsForXML(&_interpreter);
	}

	if (_monitor) {
		_monitor_ptr.reset(new SequenceCheckingMonitor(this, _lang.logger));
		_monitor_ptr->copyToInvokers(true);
		_interpreter.addMonitor(_monitor_ptr.get());
	}

	_AppTimer.start();

	_interpreter_thread_ptr = new std::thread(std::bind(&ScxmlBase::blockingQueue, this));
}

void ScxmlBase::close()
{
	std::set<InterpreterState> ANonCancelledStates{ USCXML_UNDEF, USCXML_INSTANTIATED, USCXML_CANCELLED, USCXML_FINISHED };
	if (_interpreter && ANonCancelledStates.find(this->getState())== ANonCancelledStates.end()) {
		_interpreter.cancel();
	}

	waitForStopped();
}

void ScxmlBase::waitForStopped(void)
{
	if (_interpreter_thread_ptr) {
		_interpreter_thread_ptr->join();
		delete _interpreter_thread_ptr;
		_interpreter_thread_ptr = nullptr;		
	}
}

void ScxmlBase::pause()
{
	_AppTimer.pause();
	
	auto it = _nestedDelayQueue.begin();
	while (it != _nestedDelayQueue.end()) {
		std::shared_ptr<PausableDelayedEventQueue> AQueuePtr = it->lock();
		if (!AQueuePtr) {
			_nestedDelayQueue.erase(it++);
		}
		else {
			AQueuePtr->pause();
			it++;
		}
	}
	if (_queue_ptr)
		_queue_ptr->pause();	
}

void ScxmlBase::resume()
{
	auto it = _nestedDelayQueue.begin();
	while (it != _nestedDelayQueue.end()) {
		std::shared_ptr<PausableDelayedEventQueue> AQueuePtr = it->lock();
		if (!AQueuePtr) {
			_nestedDelayQueue.erase(it++);
		}
		else {
			AQueuePtr->resume();
			it++;
		}
	}
	if (_queue_ptr)
		_queue_ptr->resume();

	_AppTimer.resume();
}

void ScxmlBase::receive(const Event& event) {
	if (_interpreter)
		_interpreter.receive(event);
}

const std::string ScxmlBase::getExeDir() const
{
	return _CMDArgs.size() ? boost::filesystem::path(_CMDArgs[0]).parent_path().string() : "";
}

void ScxmlBase::blockingQueue()
{		
	InterpreterState state_temp = InterpreterState::USCXML_UNDEF;

	try {

		do {
			state_temp = _interpreter.step();
			setState(state_temp);
		} while (state_temp != InterpreterState::USCXML_FINISHED && state_temp != InterpreterState::USCXML_CANCELLED);

	}
	catch (uscxml::ErrorEvent &e) {
		LOG(_interpreter.getLogger(), USCXML_ERROR) << "InterpreterEventsQueue> " << e;
	}
		
	if (_OnInterpreterStopped) {
		_OnInterpreterStopped(this, _OnInterpreterStoppedUser);
	}
}

uscxml::InterpreterState ScxmlBase::getState() const
{
	std::shared_lock< std::shared_mutex > lock(_mutex);
	return _state;
}

void ScxmlBase::setState(InterpreterState AState)
{
	std::unique_lock<std::shared_mutex> lock(_mutex);
	_state = AState;
}

uscxml::Data ScxmlBase::getGlobal(const std::string & sScxmlName, const std::string &sPath) const
{
	std::shared_lock<std::shared_mutex> lock(_mutex);

	auto it = _global_data_storage.find(sScxmlName);
	if (it != _global_data_storage.end()) {

		const uscxml::Data *dataptr = &it->second;

		if (!sPath.empty()) {
			using namespace boost::algorithm;

			std::vector<std::string> VecStr;
			split(VecStr, sPath, is_any_of("."));

			for (const auto &elem : VecStr) {
				if (isNumeric(elem.c_str(), 10)) {
					auto it_array_elem = dataptr->array.find(std::stoi(elem));
					if (it_array_elem != dataptr->array.end()) {
						dataptr = &it_array_elem->second;
					}
					else
						return uscxml::Data();
				}
				else {
					auto it_compaund_elem = dataptr->compound.find(elem);
					if (it_compaund_elem != dataptr->compound.end()) {
						dataptr = &it_compaund_elem->second;
					}
					else
						return uscxml::Data();
				}
			}
		}

		return *dataptr;
	}

	return uscxml::Data();
}

void ScxmlBase::setGlobal(const std::string & sScxmlName, const std::string &sPath, const uscxml::Data &data, const int iType)
{
	std::unique_lock<std::shared_mutex> lock(_mutex);
	
	// в контролы отправляем только по простому пути
	if (sPath.empty()) {
		_global_data_storage[sScxmlName] = data;
	}
	else {
		uscxml::Data *dataptr = nullptr;
		auto it_global = _global_data_storage.find(sScxmlName);
		if (it_global != _global_data_storage.end()) {
			dataptr = &it_global->second;
		}
		else {
			_global_data_storage.insert(std::make_pair(sScxmlName, uscxml::Data()));
			dataptr = &_global_data_storage[sScxmlName];
		}

		using namespace boost::algorithm;

		std::vector<std::string> VecStr;
		split(VecStr, sPath, is_any_of("."));

		for (const auto &elem : VecStr) {
			if (isNumeric(elem.c_str(), 10)) {
				const int elemIndex = std::stoi(elem);
				auto it_array_elem = dataptr->array.find(elemIndex);
				if (it_array_elem != dataptr->array.end()) {
					dataptr = &it_array_elem->second;
				}
				else {
					dataptr->array.insert(std::make_pair(elemIndex, uscxml::Data()));
					dataptr = &dataptr->array[elemIndex];
				}
			}
			else {
				auto it_compaund_elem = dataptr->compound.find(elem);
				if (it_compaund_elem != dataptr->compound.end()) {
					dataptr = &it_compaund_elem->second;
				}
				else
				{
					dataptr->compound.insert(std::make_pair(elem, uscxml::Data()));
					dataptr = &dataptr->compound[elem];
				}
			}
		}

		*dataptr = data;
	}

	switch (iType) {
	case SCXMLTASK_TO_GLOBAL_ONLY:
		CLOG(INFO) << "Set LOCAL [" << sScxmlName << "] with data:" << data;
	break;	
	default:
		CLOG(ERROR) << "Type [" << iType << "] is not supported!";
	}

}

std::string ScxmlBase::getProjectPath() const
{
	return boost::filesystem::path(_scxmlurl).parent_path().string();
}

/************************************************************************/
/*                      SequenceCheckingMonitor                         */
/************************************************************************/

SequenceCheckingMonitor::SequenceCheckingMonitor(ScxmlBase *AScxmlBase, const Logger &logger) :
	_out_socket(_io_service), _ScxmlBase(AScxmlBase)
{
	if (!_ScxmlBase)
		throw std::exception("_ScxmlBase==NULL!");

	_logger = logger;
	
	if (_ScxmlBase->useRemoteMonitor()) {
		_endpoint = udp::endpoint(boost::asio::ip::address::from_string(_ScxmlBase->_remoteHost), _ScxmlBase->_remotePort);
		_out_socket.open(_endpoint.protocol());

		CLOG(INFO) << "Remote monitor started on host:[" << _ScxmlBase->_remoteHost << "] port:[" << _ScxmlBase->_remotePort << "]";
	}
}

void SequenceCheckingMonitor::sendMessage(const std::string &sInterpreterName, const std::string &sMsg, const TScxmlMsgType AType) {
	_out_socket.send_to(boost::asio::buffer(std::to_string((long long)AType) + "@" + sInterpreterName + "@" + sMsg), _endpoint);
}

void SequenceCheckingMonitor::beforeExitingState(Interpreter& interpreter, const XERCESC_NS::DOMElement* state)
{
	const bool bUseLocalMonitor = _ScxmlBase->_Messages.find(smttBeforeExit) != _ScxmlBase->_Messages.end() || 
		_ScxmlBase->_Messages.find(smttMAXSIZE) != _ScxmlBase->_Messages.end();
	if (bUseLocalMonitor || _ScxmlBase->useRemoteMonitor() || _ScxmlBase->_OnInterpreterEnter) {
		
		std::lock_guard<std::recursive_mutex> lock(_mutex);

		const std::string sId = HAS_ATTR(state, kXMLCharId) ? ATTR(state, kXMLCharId) : DOMUtils::xPathForNode(state);
		
		if (bUseLocalMonitor) {
			LOG(_logger, USCXML_VERBATIM) << "Exiting: " << sId << ", interpreter [" << interpreter.getImpl()->getName() << "]" << std::endl;
		}		

		if (_ScxmlBase->useRemoteMonitor()) {			
			sendMessage(interpreter.getImpl()->getName(), sId, smttBeforeExit);
		}

		if (_ScxmlBase->_OnInterpreterEnter) {
			_ScxmlBase->_OnInterpreterEnter(_ScxmlBase, 
				interpreter.getImpl()->getName().c_str(), sId.c_str(), false/*exit*/, _ScxmlBase->_OnInterpreterEnterUser);
		}
	}	
}

void SequenceCheckingMonitor::afterExitingState(Interpreter& interpreter, const XERCESC_NS::DOMElement* state)
{
	const bool bUseLocalMonitor = _ScxmlBase->_Messages.find(smttAfterExit) != _ScxmlBase->_Messages.end() ||
		_ScxmlBase->_Messages.find(smttMAXSIZE) != _ScxmlBase->_Messages.end();
	if (bUseLocalMonitor || _ScxmlBase->useRemoteMonitor()) {

		std::lock_guard<std::recursive_mutex> lock(_mutex);

		const std::string sId = HAS_ATTR(state, kXMLCharId) ? ATTR(state, kXMLCharId) : DOMUtils::xPathForNode(state);

		if (bUseLocalMonitor) {
			LOG(_logger, USCXML_VERBATIM) << "After Exiting: " << sId << ", interpreter [" << interpreter.getImpl()->getName() << "]" << std::endl;
		}

		if (_ScxmlBase->useRemoteMonitor()) {
			sendMessage(interpreter.getImpl()->getName(), sId, smttAfterExit);
		}
	}	
}

void SequenceCheckingMonitor::beforeEnteringState(Interpreter& interpreter, const XERCESC_NS::DOMElement* state)
{
	const bool bUseLocalMonitor = _ScxmlBase->_Messages.find(smttBeforeEnter) != _ScxmlBase->_Messages.end() ||
		_ScxmlBase->_Messages.find(smttMAXSIZE) != _ScxmlBase->_Messages.end();
	if (bUseLocalMonitor || _ScxmlBase->useRemoteMonitor() || _ScxmlBase->_OnInterpreterEnter) {

		std::lock_guard<std::recursive_mutex> lock(_mutex);

		const std::string sId = HAS_ATTR(state, kXMLCharId) ? ATTR(state, kXMLCharId) : DOMUtils::xPathForNode(state);

		if (bUseLocalMonitor) {
			LOG(_logger, USCXML_VERBATIM) << "Entering: " << sId << ", interpreter [" << interpreter.getImpl()->getName() << "]" << std::endl;
		}

		if (_ScxmlBase->useRemoteMonitor()) {
			sendMessage(interpreter.getImpl()->getName(), sId, smttBeforeEnter);
		}

		if (_ScxmlBase->_OnInterpreterEnter) {
			_ScxmlBase->_OnInterpreterEnter(_ScxmlBase,
				interpreter.getImpl()->getName().c_str(), sId.c_str(), true/*enter*/, _ScxmlBase->_OnInterpreterEnterUser);
		}
	}
}

void SequenceCheckingMonitor::afterEnteringState(Interpreter& interpreter, const XERCESC_NS::DOMElement* state)
{
	const bool bUseLocalMonitor = _ScxmlBase->_Messages.find(smttAfterEnter) != _ScxmlBase->_Messages.end() ||
		_ScxmlBase->_Messages.find(smttMAXSIZE) != _ScxmlBase->_Messages.end();
	if (bUseLocalMonitor || _ScxmlBase->useRemoteMonitor()) {

		std::lock_guard<std::recursive_mutex> lock(_mutex);

		const std::string sId = HAS_ATTR(state, kXMLCharId) ? ATTR(state, kXMLCharId) : DOMUtils::xPathForNode(state);

		if (bUseLocalMonitor) {
			LOG(_logger, USCXML_VERBATIM) << "After Entering: " << sId << ", interpreter [" << interpreter.getImpl()->getName() << "]" << std::endl;
		}

		if (_ScxmlBase->useRemoteMonitor()) {
			sendMessage(interpreter.getImpl()->getName(), sId, smttAfterEnter);
		}		
	}	
}

void SequenceCheckingMonitor::beforeExecutingContent(Interpreter& interpreter, const XERCESC_NS::DOMElement* execContent)
{
	if (_ScxmlBase->_Messages.find(smttBeforeExecContent) != _ScxmlBase->_Messages.end() || _ScxmlBase->_Messages.find(smttMAXSIZE) != _ScxmlBase->_Messages.end()) {
		StateTransitionMonitor::beforeExecutingContent(interpreter, execContent);

		if (_ScxmlBase->useRemoteMonitor()) {
			std::lock_guard<std::recursive_mutex> lock(_mutex);
			sendMessage(interpreter.getImpl()->getName(), DOMUtils::xPathForNode(execContent), smttBeforeExecContent);
		}			
	}
}

void SequenceCheckingMonitor::afterExecutingContent(Interpreter& interpreter, const XERCESC_NS::DOMElement* execContent)
{
	if (_ScxmlBase->_Messages.find(smttAfterExecContent) != _ScxmlBase->_Messages.end() || _ScxmlBase->_Messages.find(smttMAXSIZE) != _ScxmlBase->_Messages.end()) {
		StateTransitionMonitor::afterExecutingContent(interpreter, execContent);
		
		if (_ScxmlBase->useRemoteMonitor()) {
			std::lock_guard<std::recursive_mutex> lock(_mutex);
			sendMessage(interpreter.getImpl()->getName(), DOMUtils::xPathForNode(execContent), smttAfterExecContent);
		}			
	}
}

void SequenceCheckingMonitor::beforeUninvoking(Interpreter& interpreter, const XERCESC_NS::DOMElement* invokeElem, const std::string& invokeid)
{
	const bool bUseLocalMonitor = _ScxmlBase->_Messages.find(smttBeforeUnInvoke) != _ScxmlBase->_Messages.end() ||
		_ScxmlBase->_Messages.find(smttMAXSIZE) != _ScxmlBase->_Messages.end();
	if (bUseLocalMonitor || _ScxmlBase->useRemoteMonitor() || _ScxmlBase->_OnInterpreterInvoke) {

		std::lock_guard<std::recursive_mutex> lock(_mutex);			

		if (bUseLocalMonitor) {
			LOG(_logger, USCXML_VERBATIM) << "Uninvoking: " << invokeid << ", interpreter [" << interpreter.getImpl()->getName() << "]" << std::endl;
		}

		if (_ScxmlBase->useRemoteMonitor()) {
			sendMessage(interpreter.getImpl()->getName(), invokeid, smttBeforeUnInvoke);
		}

		if (_ScxmlBase->_OnInterpreterInvoke) {
			_ScxmlBase->_OnInterpreterInvoke(_ScxmlBase,
				interpreter.getImpl()->getName().c_str(), invokeid.c_str(), false/*UnInvoke*/, _ScxmlBase->_OnInterpreterInvokeUser);
		}
	}
}

void SequenceCheckingMonitor::afterUninvoking(Interpreter& interpreter, const XERCESC_NS::DOMElement* invokeElem, const std::string& invokeid)
{
	if (_ScxmlBase->_Messages.find(smttAfterUnInvoke) != _ScxmlBase->_Messages.end() || _ScxmlBase->_Messages.find(smttMAXSIZE) != _ScxmlBase->_Messages.end()) {
		StateTransitionMonitor::afterUninvoking(interpreter, invokeElem, invokeid);
		
		if (_ScxmlBase->useRemoteMonitor()) {
			std::lock_guard<std::recursive_mutex> lock(_mutex);
			sendMessage(interpreter.getImpl()->getName(), invokeid, smttAfterUnInvoke);
		}			
	}
}

void SequenceCheckingMonitor::beforeInvoking(Interpreter& interpreter, const XERCESC_NS::DOMElement* invokeElem, const std::string& invokeid)
{
	const bool bUseLocalMonitor = _ScxmlBase->_Messages.find(smttBeforeInvoke) != _ScxmlBase->_Messages.end() ||
		_ScxmlBase->_Messages.find(smttMAXSIZE) != _ScxmlBase->_Messages.end();
	if (bUseLocalMonitor || _ScxmlBase->useRemoteMonitor() || _ScxmlBase->_OnInterpreterInvoke) {

		std::lock_guard<std::recursive_mutex> lock(_mutex);

		if (bUseLocalMonitor) {
			LOG(_logger, USCXML_VERBATIM) << "Invoking: " << invokeid << ", interpreter [" << interpreter.getImpl()->getName() << "]" << std::endl;
		}

		if (_ScxmlBase->useRemoteMonitor()) {
			sendMessage(interpreter.getImpl()->getName(), invokeid, smttBeforeInvoke);
		}

		if (_ScxmlBase->_OnInterpreterInvoke) {
			_ScxmlBase->_OnInterpreterInvoke(_ScxmlBase,
				interpreter.getImpl()->getName().c_str(), invokeid.c_str(), true/*Invoke*/, _ScxmlBase->_OnInterpreterInvokeUser);
		}
	}
}

void SequenceCheckingMonitor::afterInvoking(Interpreter& interpreter, const XERCESC_NS::DOMElement* invokeElem, const std::string& invokeid)
{
	if (_ScxmlBase->_Messages.find(smttAfterInvoke) != _ScxmlBase->_Messages.end() || _ScxmlBase->_Messages.find(smttMAXSIZE) != _ScxmlBase->_Messages.end()) {
		StateTransitionMonitor::afterInvoking(interpreter, invokeElem, invokeid);

		if (_ScxmlBase->useRemoteMonitor()) {
			std::lock_guard<std::recursive_mutex> lock(_mutex);
			sendMessage(interpreter.getImpl()->getName(), invokeid, smttAfterInvoke);
		}			
	}
}

void SequenceCheckingMonitor::beforeTakingTransition(Interpreter& interpreter, const XERCESC_NS::DOMElement* transition)
{
	if (_ScxmlBase->_Messages.find(smttBeforeTakingTransition) != _ScxmlBase->_Messages.end() || _ScxmlBase->_Messages.find(smttMAXSIZE) != _ScxmlBase->_Messages.end()) {
		StateTransitionMonitor::beforeTakingTransition(interpreter, transition);
		
		if (_ScxmlBase->useRemoteMonitor()) {
			std::lock_guard<std::recursive_mutex> lock(_mutex);
			std::string s_event = "*";
			if (transition->hasAttribute(X("event"))) {
				s_event = X(transition->getAttribute(X("event")));
			}
			sendMessage(interpreter.getImpl()->getName(), s_event, smttBeforeTakingTransition);
		}		
	}
}

void SequenceCheckingMonitor::afterTakingTransition(Interpreter& interpreter, const XERCESC_NS::DOMElement* transition)
{
	if (_ScxmlBase->_Messages.find(smttAfterTakingTransition) != _ScxmlBase->_Messages.end() || _ScxmlBase->_Messages.find(smttMAXSIZE) != _ScxmlBase->_Messages.end()) {
		StateTransitionMonitor::afterTakingTransition(interpreter, transition);
		
		if (_ScxmlBase->useRemoteMonitor()) {
			std::lock_guard<std::recursive_mutex> lock(_mutex);
			std::string s_event = "*";
			if (transition->hasAttribute(X("event"))) {
				s_event = X(transition->getAttribute(X("event")));
			}
			sendMessage(interpreter.getImpl()->getName(), s_event, smttAfterTakingTransition);
		}
	}
}

void SequenceCheckingMonitor::beforeProcessingEvent(Interpreter & interpreter, const uscxml::Event & event)
{
	if (_ScxmlBase->_OnInterpreterEvent) {
		std::lock_guard<std::recursive_mutex> lock(_mutex);
		_ScxmlBase->_OnInterpreterEvent(_ScxmlBase, interpreter.getImpl()->getName().c_str(), event.name.c_str(),
			_ScxmlBase->_OnInterpreterEventAtomOrJson ? event.data.atom.c_str() : event.data.asJSON().c_str(),
			_ScxmlBase->_OnInterpreterEventAtomOrJson,
			_ScxmlBase->_OnInterpreterEventUser);
	}
}
