#include "UscxmlClibBase.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

#include <uscxml/util/String.h>

#include "UscxmlClibLogger.h"
#include "UscxmlLuaDataModelEx.h"

ScxmlBase::ScxmlBase(const std::vector<std::string> &ACMDArgs, const ScxmlBaseOptions &AScxmlBaseOptions, const bool bHttpEnabled/* = false*/):
	_options(AScxmlBaseOptions), _httpEnabled(bHttpEnabled), _CMDArgs(ACMDArgs) {
	
	if (_options.Monitor) {
		std::stringstream ss;
		for (auto it = _options.MonitorMsgTypes.begin(); it != _options.MonitorMsgTypes.end(); ++it) {
			if (it != _options.MonitorMsgTypes.begin())
				ss << "|";
			ss << ScxmlMsgTypeToString(*it);
		}
		CLOG(DEBUG) << "Monitor:[" << ss.str() << "]";
	}	
}

ScxmlBase::~ScxmlBase(void)
{
	close();

	// there were caught AccessViolation in microstepper (state chart with multiple invoked state charts)	
	// if '_interpreter' was not deleted before clearing '_monitor_ptr'
	// clearing '_monitor_ptr' here does not solve the problem
	_interpreter = Interpreter();
}

void  ScxmlBase::start(const std::string &sTextOrFile, const bool bIsText) {	
	/// possible deadlock - think in future how to replace,
	/// but we should be sure that previous interpreter stopped!
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

	/* We are setting custom factory per Intepreter */
	_factory_ptr.reset(new FactoryDynamic(&Factory::getInstance()));

	std::set<Factory::PluginType> FACTORY_TYPES{
		Factory::PluginType::ptSCXMLIOProcessor,
		Factory::PluginType::ptLuaDataModel,
		Factory::PluginType::ptJSCDataModel,
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
	_lang.logger = std::shared_ptr<LoggerImpl>(new CLibLogger(_options.Monitor, this, _OnInterpreterLog, _OnInterpreterLogUser));
	_interpreter.setActionLanguage(_lang);

	/* Validate for SCXML and script syntax errors */
	if (_options.Validate) {
		std::list<InterpreterIssue> issues = _interpreter.validate();

		bool bIsError = false;

		for (std::list<InterpreterIssue>::iterator issueIter = issues.begin(); issueIter != issues.end(); issueIter++) {
			switch (issueIter->severity) {
			case InterpreterIssue::USCXML_ISSUE_FATAL: {
				std::stringstream ss;
				ss << *issueIter;
				LOG(_interpreter.getLogger(), USCXML_ERROR) << ss.str();
				bIsError = true;
			} break;
			case InterpreterIssue::USCXML_ISSUE_INFO: {
				std::stringstream ss;
				ss << *issueIter;
				LOG(_interpreter.getLogger(), USCXML_INFO) << ss.str();
			} break;
			case InterpreterIssue::USCXML_ISSUE_WARNING: {
				std::stringstream ss;
				ss << *issueIter;
				LOG(_interpreter.getLogger(), USCXML_WARN) << ss.str();
			} break;
			}
		}
		if (bIsError && _options.TerminateNotValidated)
			throw std::runtime_error("Fatal SCXML validation error(s)! Interpreter stopped!");
	}
		
	_monitor_ptr.reset(new SequenceCheckingMonitor(this, _lang.logger));
	_monitor_ptr->copyToInvokers(true);
	_interpreter.addMonitor(_monitor_ptr.get());
	
	_AppTimer.start();
	
	/* 
		if state chart is to large and you do not want to wait while it reaches the stable state,
		you may use 'AsyncStart' option 
	*/
	if (!_options.AsyncStart) {
		/* synchronized load */
		auto state_temp = USCXML_UNDEF;
		const std::set<InterpreterState> AStableStates{ USCXML_FINISHED, USCXML_CANCELLED, USCXML_MACROSTEPPED };
		do {
			state_temp = _interpreter.step();
			setState(state_temp);
		} while (AStableStates.find(state_temp) == AStableStates.end());
	}	

	/* check that state chart is not finished yet */
	const std::set<InterpreterState> AFinishedStates{ USCXML_FINISHED, USCXML_CANCELLED };
	if (AFinishedStates.find(getState()) == AFinishedStates.end()) {
		_interpreter_thread_ptr = new std::thread(std::bind(&ScxmlBase::blockingQueue, this));
	}
	else {
		if (_OnInterpreterStopped) {
			_OnInterpreterStopped(this, _OnInterpreterStoppedUser);
		}
	}
}

void ScxmlBase::close()
{
	const std::set<InterpreterState> ASkippedStates{ USCXML_UNDEF, USCXML_CANCELLED, USCXML_FINISHED };
	auto state = this->getState();
	if (_interpreter && _interpreter_thread_ptr && ASkippedStates.find(state) == ASkippedStates.end()) {
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
	// while interpreter is not initialized EventQueue is NULL !
	if (_interpreter && getState()!=USCXML_UNDEF) {
		_interpreter.receive(event);
	}
	else
		throw std::runtime_error("Interpereter is not ready to receive event [" + event.name + "]");
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
	{
		std::unique_lock<std::shared_mutex> lock(_mutex);

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
	}
	
	if (_OnInterpreterGlobalDataChange) {
		_OnInterpreterGlobalDataChange(this, sScxmlName.c_str(), sPath.c_str(),
			_OnInterpreterGlobalDataChangeAtomOrJson ? data.atom.c_str() : data.asJSON().c_str(),
			_OnInterpreterGlobalDataChangeAtomOrJson, iType, _OnInterpreterGlobalDataChangeUser);
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
		_endpoint = udp::endpoint(boost::asio::ip::address::from_string(_ScxmlBase->_options.RemoteMonitorHost),
			_ScxmlBase->_options.RemoteMonitorPort);
		_out_socket.open(_endpoint.protocol());

		CLOG(INFO) << "Remote monitor started on host:[" << _ScxmlBase->_options.RemoteMonitorHost 
			<< "] port:[" << _ScxmlBase->_options.RemoteMonitorPort << "]";


		_out_socket.send_to(boost::asio::buffer("@@@"), _endpoint);
	}
}

void SequenceCheckingMonitor::sendMessage(const std::string &sInterpreterName, const std::string &sMsg, const TScxmlMsgType AType, const std::string &sId) {
	_out_socket.send_to(boost::asio::buffer(uscxml::fromLocaleToUtf8(
		std::to_string((long long)AType) + "@" + sInterpreterName + "@" + sMsg + (sId.empty() ? sId : ("@" + sId))
	)), _endpoint);
}

void SequenceCheckingMonitor::sendClearInterpreterMessage(const std::string & sInterpreterName, const std::string &sId)
{
	std::stringstream ss;
	ss << "@@@" << sInterpreterName;
	if (!sId.empty()) {
		ss << "@" << sId;
	}

	_out_socket.send_to(boost::asio::buffer(uscxml::fromLocaleToUtf8(ss.str())), _endpoint);
}

std::string SequenceCheckingMonitor::getInvokeIdFromInterpreter(Interpreter& interpreter) {
	std::string sInvokeId = "";
	if (interpreter) {
		const auto impl = interpreter.getImpl();
		if (impl) {
			sInvokeId = impl->getInvokeId();
		}
	}

	return sInvokeId;
}

void SequenceCheckingMonitor::beforeExitingState(Interpreter& interpreter, const XERCESC_NS::DOMElement* state)
{
	const bool bMonitor = _ScxmlBase->_options.Monitor &&
		(_ScxmlBase->_options.MonitorMsgTypes.find(smttBeforeExit) != _ScxmlBase->_options.MonitorMsgTypes.end() ||
		_ScxmlBase->_options.MonitorMsgTypes.find(smttMAXSIZE) != _ScxmlBase->_options.MonitorMsgTypes.end());
	if (bMonitor || _ScxmlBase->_OnInterpreterEnterExit) {
		
		std::lock_guard<std::recursive_mutex> lock(_mutex);

		const std::string sId = HAS_ATTR(state, kXMLCharId) ? ATTR(state, kXMLCharId) : DOMUtils::xPathForNode(state);
		
		if (bMonitor) {
			LOG(_logger, USCXML_VERBATIM) << "Exiting: " << sId << ", interpreter [" << interpreter.getImpl()->getName() << "]" << std::endl;

			if (_ScxmlBase->useRemoteMonitor()) {
				sendMessage(interpreter.getImpl()->getName(), sId, smttBeforeExit,
					getInvokeIdFromInterpreter(interpreter));
			}
		}		

		if (_ScxmlBase->_OnInterpreterEnterExit) {
			_ScxmlBase->_OnInterpreterEnterExit(_ScxmlBase, 
				interpreter.getImpl()->getName().c_str(), sId.c_str(), false/*exit*/, _ScxmlBase->_OnInterpreterEnterExitUser);
		}
	}	
}

void SequenceCheckingMonitor::afterExitingState(Interpreter& interpreter, const XERCESC_NS::DOMElement* state)
{
	const bool bMonitor = _ScxmlBase->_options.Monitor &&
		(_ScxmlBase->_options.MonitorMsgTypes.find(smttAfterExit) != _ScxmlBase->_options.MonitorMsgTypes.end() ||
		_ScxmlBase->_options.MonitorMsgTypes.find(smttMAXSIZE) != _ScxmlBase->_options.MonitorMsgTypes.end());
	if (bMonitor) {

		std::lock_guard<std::recursive_mutex> lock(_mutex);

		const std::string sId = HAS_ATTR(state, kXMLCharId) ? ATTR(state, kXMLCharId) : DOMUtils::xPathForNode(state);

		LOG(_logger, USCXML_VERBATIM) << "After Exiting: " << sId << ", interpreter [" << interpreter.getImpl()->getName() << "]" << std::endl;
		
		if (_ScxmlBase->useRemoteMonitor()) {
			sendMessage(interpreter.getImpl()->getName(), sId, smttAfterExit, getInvokeIdFromInterpreter(interpreter));
		}
	}	
}

void SequenceCheckingMonitor::beforeEnteringState(Interpreter& interpreter, const XERCESC_NS::DOMElement* state)
{
	const bool bMonitor = _ScxmlBase->_options.Monitor &&
		(_ScxmlBase->_options.MonitorMsgTypes.find(smttBeforeEnter) != _ScxmlBase->_options.MonitorMsgTypes.end() ||
		_ScxmlBase->_options.MonitorMsgTypes.find(smttMAXSIZE) != _ScxmlBase->_options.MonitorMsgTypes.end());
	if (bMonitor || _ScxmlBase->_OnInterpreterEnterExit) {

		std::lock_guard<std::recursive_mutex> lock(_mutex);

		const std::string sId = HAS_ATTR(state, kXMLCharId) ? ATTR(state, kXMLCharId) : DOMUtils::xPathForNode(state);

		if (bMonitor) {
			LOG(_logger, USCXML_VERBATIM) << "Entering: " << sId << ", interpreter [" << interpreter.getImpl()->getName() << "]" << std::endl;

			if (_ScxmlBase->useRemoteMonitor()) {
				sendMessage(interpreter.getImpl()->getName(), sId, smttBeforeEnter, getInvokeIdFromInterpreter(interpreter));
			}
		}		

		if (_ScxmlBase->_OnInterpreterEnterExit) {
			_ScxmlBase->_OnInterpreterEnterExit(_ScxmlBase,
				interpreter.getImpl()->getName().c_str(), sId.c_str(), true/*enter*/, _ScxmlBase->_OnInterpreterEnterExitUser);
		}
	}
}

void SequenceCheckingMonitor::afterEnteringState(Interpreter& interpreter, const XERCESC_NS::DOMElement* state)
{
	const bool bMonitor = _ScxmlBase->_options.Monitor &&
		(_ScxmlBase->_options.MonitorMsgTypes.find(smttAfterEnter) != _ScxmlBase->_options.MonitorMsgTypes.end() ||
		_ScxmlBase->_options.MonitorMsgTypes.find(smttMAXSIZE) != _ScxmlBase->_options.MonitorMsgTypes.end());
	if (bMonitor) {

		std::lock_guard<std::recursive_mutex> lock(_mutex);

		const std::string sId = HAS_ATTR(state, kXMLCharId) ? ATTR(state, kXMLCharId) : DOMUtils::xPathForNode(state);

		LOG(_logger, USCXML_VERBATIM) << "After Entering: " << sId << ", interpreter [" << interpreter.getImpl()->getName() << "]" << std::endl;
		
		if (_ScxmlBase->useRemoteMonitor()) {
			sendMessage(interpreter.getImpl()->getName(), sId, smttAfterEnter, getInvokeIdFromInterpreter(interpreter));
		}		
	}	
}

void SequenceCheckingMonitor::beforeExecutingContent(Interpreter& interpreter, const XERCESC_NS::DOMElement* execContent)
{
	if (_ScxmlBase->_options.Monitor &&
		(_ScxmlBase->_options.MonitorMsgTypes.find(smttBeforeExecContent) != _ScxmlBase->_options.MonitorMsgTypes.end() ||
		_ScxmlBase->_options.MonitorMsgTypes.find(smttMAXSIZE) != _ScxmlBase->_options.MonitorMsgTypes.end())) {

		StateTransitionMonitor::beforeExecutingContent(interpreter, execContent);

		if (_ScxmlBase->useRemoteMonitor()) {
			std::lock_guard<std::recursive_mutex> lock(_mutex);
			sendMessage(interpreter.getImpl()->getName(), DOMUtils::xPathForNode(execContent), smttBeforeExecContent, getInvokeIdFromInterpreter(interpreter));
		}			
	}
}

void SequenceCheckingMonitor::afterExecutingContent(Interpreter& interpreter, const XERCESC_NS::DOMElement* execContent)
{
	if (_ScxmlBase->_options.Monitor &&
		(_ScxmlBase->_options.MonitorMsgTypes.find(smttAfterExecContent) != _ScxmlBase->_options.MonitorMsgTypes.end() ||
		_ScxmlBase->_options.MonitorMsgTypes.find(smttMAXSIZE) != _ScxmlBase->_options.MonitorMsgTypes.end())) {

		StateTransitionMonitor::afterExecutingContent(interpreter, execContent);
		
		if (_ScxmlBase->useRemoteMonitor()) {
			std::lock_guard<std::recursive_mutex> lock(_mutex);
			sendMessage(interpreter.getImpl()->getName(), DOMUtils::xPathForNode(execContent), smttAfterExecContent, 
				getInvokeIdFromInterpreter(interpreter));
		}			
	}
}

void SequenceCheckingMonitor::beforeUninvoking(Interpreter& interpreter, const XERCESC_NS::DOMElement* invokeElem, const std::string& invokeid)
{
	const bool bMonitor = _ScxmlBase->_options.Monitor &&
		(_ScxmlBase->_options.MonitorMsgTypes.find(smttBeforeUnInvoke) != _ScxmlBase->_options.MonitorMsgTypes.end() ||
		_ScxmlBase->_options.MonitorMsgTypes.find(smttMAXSIZE) != _ScxmlBase->_options.MonitorMsgTypes.end());
	if (bMonitor || _ScxmlBase->_OnInterpreterInvoke) {

		std::lock_guard<std::recursive_mutex> lock(_mutex);			

		if (bMonitor) {
			LOG(_logger, USCXML_VERBATIM) << "Uninvoking: " << invokeid << ", interpreter [" << interpreter.getImpl()->getName() << "]" << std::endl;

			if (_ScxmlBase->useRemoteMonitor()) {
				auto impl = interpreter.getImpl();
				if (impl) {
					const std::string &sSubName = impl->getInvokedScxmlName(invokeid);
					sendMessage(interpreter.getImpl()->getName(), sSubName, smttBeforeUnInvoke, invokeid);
					sendClearInterpreterMessage(sSubName, invokeid);
				}
			}
		}		

		if (_ScxmlBase->_OnInterpreterInvoke) {
			_ScxmlBase->_OnInterpreterInvoke(_ScxmlBase,
				interpreter.getImpl()->getName().c_str(), invokeid.c_str(), false/*UnInvoke*/, _ScxmlBase->_OnInterpreterInvokeUser);
		}
	}
}

void SequenceCheckingMonitor::afterUninvoking(Interpreter& interpreter, const XERCESC_NS::DOMElement* invokeElem, const std::string& invokeid)
{
	if (_ScxmlBase->_options.Monitor &&
		(_ScxmlBase->_options.MonitorMsgTypes.find(smttAfterUnInvoke) != _ScxmlBase->_options.MonitorMsgTypes.end() ||
		_ScxmlBase->_options.MonitorMsgTypes.find(smttMAXSIZE) != _ScxmlBase->_options.MonitorMsgTypes.end())) {

		StateTransitionMonitor::afterUninvoking(interpreter, invokeElem, invokeid);
		
		if (_ScxmlBase->useRemoteMonitor()) {
			std::lock_guard<std::recursive_mutex> lock(_mutex);
			sendMessage(interpreter.getImpl()->getName(), invokeid, smttAfterUnInvoke, invokeid);
		}			
	}
}

void SequenceCheckingMonitor::beforeInvoking(Interpreter& interpreter, const XERCESC_NS::DOMElement* invokeElem, const std::string& invokeid)
{
	const bool bMonitor = _ScxmlBase->_options.Monitor &&
		(_ScxmlBase->_options.MonitorMsgTypes.find(smttBeforeInvoke) != _ScxmlBase->_options.MonitorMsgTypes.end() ||
		_ScxmlBase->_options.MonitorMsgTypes.find(smttMAXSIZE) != _ScxmlBase->_options.MonitorMsgTypes.end());
	if (bMonitor || _ScxmlBase->_OnInterpreterInvoke) {

		std::lock_guard<std::recursive_mutex> lock(_mutex);

		if (bMonitor) {
			LOG(_logger, USCXML_VERBATIM) << "Invoking: " << invokeid << ", interpreter [" << interpreter.getImpl()->getName() << "]" << std::endl;

			if (_ScxmlBase->useRemoteMonitor()) {
				sendMessage(interpreter.getImpl()->getName(), invokeid, smttBeforeInvoke, invokeid);
			}
		}		

		if (_ScxmlBase->_OnInterpreterInvoke) {
			_ScxmlBase->_OnInterpreterInvoke(_ScxmlBase,
				interpreter.getImpl()->getName().c_str(), invokeid.c_str(), true/*Invoke*/, _ScxmlBase->_OnInterpreterInvokeUser);
		}
	}
}

void SequenceCheckingMonitor::afterInvoking(Interpreter& interpreter, const XERCESC_NS::DOMElement* invokeElem, const std::string& invokeid)
{
	if (_ScxmlBase->_options.Monitor &&
		(_ScxmlBase->_options.MonitorMsgTypes.find(smttAfterInvoke) != _ScxmlBase->_options.MonitorMsgTypes.end() ||
		_ScxmlBase->_options.MonitorMsgTypes.find(smttMAXSIZE) != _ScxmlBase->_options.MonitorMsgTypes.end())) {

		StateTransitionMonitor::afterInvoking(interpreter, invokeElem, invokeid);

		if (_ScxmlBase->useRemoteMonitor()) {
			std::lock_guard<std::recursive_mutex> lock(_mutex);
			sendMessage(interpreter.getImpl()->getName(), invokeid, smttAfterInvoke, invokeid);
		}			
	}
}

void SequenceCheckingMonitor::beforeTakingTransition(Interpreter& interpreter, const XERCESC_NS::DOMElement* transition)
{
	if (_ScxmlBase->_options.Monitor &&
		(_ScxmlBase->_options.MonitorMsgTypes.find(smttBeforeTakingTransition) != _ScxmlBase->_options.MonitorMsgTypes.end() ||
		_ScxmlBase->_options.MonitorMsgTypes.find(smttMAXSIZE) != _ScxmlBase->_options.MonitorMsgTypes.end())) {

		StateTransitionMonitor::beforeTakingTransition(interpreter, transition);
		
		if (_ScxmlBase->useRemoteMonitor()) {
			std::lock_guard<std::recursive_mutex> lock(_mutex);
			std::string s_event = "*";
			if (transition->hasAttribute(X("event"))) {
				s_event = X(transition->getAttribute(X("event")));
			}
			sendMessage(interpreter.getImpl()->getName(), s_event, smttBeforeTakingTransition,
				getInvokeIdFromInterpreter(interpreter));
		}		
	}
}

void SequenceCheckingMonitor::afterTakingTransition(Interpreter& interpreter, const XERCESC_NS::DOMElement* transition)
{
	if (_ScxmlBase->_options.Monitor &&
		(_ScxmlBase->_options.MonitorMsgTypes.find(smttAfterTakingTransition) != _ScxmlBase->_options.MonitorMsgTypes.end() ||
		_ScxmlBase->_options.MonitorMsgTypes.find(smttMAXSIZE) != _ScxmlBase->_options.MonitorMsgTypes.end())) {

		StateTransitionMonitor::afterTakingTransition(interpreter, transition);
		
		if (_ScxmlBase->useRemoteMonitor()) {
			std::lock_guard<std::recursive_mutex> lock(_mutex);
			std::string s_event = "*";
			if (transition->hasAttribute(X("event"))) {
				s_event = X(transition->getAttribute(X("event")));
			}
			sendMessage(interpreter.getImpl()->getName(), s_event, smttAfterTakingTransition,
				getInvokeIdFromInterpreter(interpreter));
		}
	}
}

void SequenceCheckingMonitor::beforeProcessingEvent(Interpreter & interpreter, const uscxml::Event & event)
{
	if (_ScxmlBase->_OnInterpreterEvent) {
		std::lock_guard<std::recursive_mutex> lock(_mutex);		
		if (event.params.size()) {
			for (const auto &it : event.params) {
				const std::string s_eventname_with_param = event.name + "." + it.first;
				_ScxmlBase->_OnInterpreterEvent(_ScxmlBase, interpreter.getImpl()->getName().c_str(), s_eventname_with_param.c_str(),
					_ScxmlBase->_OnInterpreterEventAtomOrJson ? it.second.atom.c_str() : it.second.asJSON().c_str(),
					_ScxmlBase->_OnInterpreterEventAtomOrJson,
					_ScxmlBase->_OnInterpreterEventUser);
			}
		}
		else {
			_ScxmlBase->_OnInterpreterEvent(_ScxmlBase, interpreter.getImpl()->getName().c_str(), event.name.c_str(),
				_ScxmlBase->_OnInterpreterEventAtomOrJson ? event.data.atom.c_str() : event.data.asJSON().c_str(),
				_ScxmlBase->_OnInterpreterEventAtomOrJson,
				_ScxmlBase->_OnInterpreterEventUser);
		}
		
	}
}
