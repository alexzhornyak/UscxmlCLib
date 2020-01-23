// UscxmlCLibSrc.cpp : Defines the exported functions for the DLL application.

#include "../../include/UscxmlCLib.h"

#include <memory>
#include <bitset>

#include "uscxml/server/HTTPServer.h"

#include "UscxmlCLibBase.h"
#include "UscxmlClibLogger.h"

using namespace uscxml;

/* Global Library Variables */
std::string g_LAST_ERROR = "";
void SetLastUsclibError(const std::string &sErr) {
	g_LAST_ERROR = sErr;
	CLOG(ERROR) << __FUNCTION__ << "> " << g_LAST_ERROR;
}

bool g_HTTP_ENABLED = false;

std::set<UsclibInterpreter *> g_INTERPRETERS;

#define	CATCH_USCLIB_ALL		catch (uscxml::ErrorEvent &e) {	\
									std::stringstream ss;	\
									ss << e;	\
									SetLastUsclibError(ss.str()); }	\
								catch (std::exception &e) { SetLastUsclibError(e.what()); }	\
								catch (...) { SetLastUsclibError("Unknown exception"); }			

#define CHECK_INTERPRETER_VALID(interpreter)	if (g_INTERPRETERS.find(interpreter)==g_INTERPRETERS.end()) \
												{ std::stringstream ss; ss << "UsclibInterpreter:[0x" <<  std::hex << \
												reinterpret_cast<int>(interpreter) << "] not initialized!"; throw std::exception(ss.str().c_str()); }

const char USCXMLCLIBAPI * usclib_GetLastError(void) {
	return g_LAST_ERROR.c_str();
}

int USCXMLCLIBAPI usclib_InitLogging(const char * chLogFileName)
{
	try
	{
		boost::log::register_simple_formatter_factory<SysLogSeverity, char>(boost::log::aux::default_attribute_names::severity());
		boost::log::register_simple_filter_factory<SysLogSeverity>(boost::log::aux::default_attribute_names::severity());

		boost::log::settings setts;
		setts["Sinks.File.Destination"] = "TextFile";
		setts["Sinks.File.FileName"] = chLogFileName;
		setts["Sinks.File.AutoFlush"] = true;
		setts["Sinks.File.Format"] = "[%TimeStamp%; %Severity%]: %Message%";

		// Read the settings and initialize logging library
		boost::log::init_from_settings(setts);

		boost::log::add_common_attributes();

		return USCLIB_SUCCESS;
	} catch (std::exception& e) {
		g_LAST_ERROR = e.what();
	}
	catch (...) {
		g_LAST_ERROR = "Unknown exception";
	}
	return ERROR_USCLIB_INIT_LOG;
}

int USCXMLCLIBAPI usclib_OpenInterpreter(UsclibInterpreter **AInterpreter,
	const char **chCMD, const int nCMD, const UsclibInterpreterOptions *AInterpreterOptions) {
	try {
		std::vector<std::string> AVecString(chCMD, chCMD + nCMD);

		if (chCMD && nCMD) {
			std::vector<std::string> AVecCmd(chCMD, chCMD + nCMD);
			AVecString.swap(AVecCmd);
		}
		
		std::set<TScxmlMsgType> AMonitorMessages{smttBeforeEnter,
			smttBeforeExit, smttBeforeTakingTransition,
			smttBeforeInvoke, smttBeforeUnInvoke };

		if (AInterpreterOptions) {
			std::set<TScxmlMsgType> AOptionsMsgs;
			std::bitset<smttMAXSIZE+1> ABitMsgs(AInterpreterOptions->MonitorMsgTypes);
			for (std::size_t n = 0; n < ABitMsgs.size(); n++) {
				if (ABitMsgs[n]) {
					AOptionsMsgs.insert(TScxmlMsgType(n));
				}
			}
			AMonitorMessages.swap(AOptionsMsgs);
		}

		std::stringstream ss;
		for (auto it = AMonitorMessages.begin(); it != AMonitorMessages.end(); ++it) {
			if (it != AMonitorMessages.begin())
				ss << "|";
			ss << ScxmlMsgTypeToString(*it);
		}
		CLOG(DEBUG) << "Monitor Types [" << ss.str() << "]";

		ScxmlBase *AScxmlBase = new ScxmlBase(AVecString, 
			AMonitorMessages,
			AInterpreterOptions ? AInterpreterOptions->Monitor : true,
			AInterpreterOptions ? AInterpreterOptions->RemoteMonitorHost : "127.0.0.1",
			AInterpreterOptions? AInterpreterOptions->RemoteMonitorPort : SCXML_DISABLE_REMOTE_MONITOR,
			AInterpreterOptions ? AInterpreterOptions->CheckIssues : false, 
			g_HTTP_ENABLED);

		*AInterpreter = AScxmlBase;
		g_INTERPRETERS.insert(*AInterpreter);

		return USCLIB_SUCCESS;
	}
	CATCH_USCLIB_ALL;

	return ERROR_USCLIB_OPEN_INTERPRETER;
}

int USCXMLCLIBAPI usclib_CloseInterpreter(UsclibInterpreter *AInterpreter) {
	try {
		CHECK_INTERPRETER_VALID(AInterpreter);

		g_INTERPRETERS.erase(AInterpreter);

		ScxmlBase *AScxmlBase = reinterpret_cast<ScxmlBase*>(AInterpreter);
		AScxmlBase->registerOnStopped(nullptr, nullptr);
		delete AScxmlBase;
		
		return USCLIB_SUCCESS;
	}
	CATCH_USCLIB_ALL;

	return ERROR_USCLIB_CLOSE_INTERPRETER;
}

int USCXMLCLIBAPI usclib_RegisterLogCallback(UsclibInterpreter * AInterpreter, CALLBACK_USCLIB_INTERPRETER_LOG ACallback, void *AUser)
{
	try {
		CHECK_INTERPRETER_VALID(AInterpreter);

		ScxmlBase *AScxmlBase = reinterpret_cast<ScxmlBase*>(AInterpreter);
		AScxmlBase->registerOnInterpreterLog(ACallback, AUser);

		return USCLIB_SUCCESS;
	}
	CATCH_USCLIB_ALL;

	return ERROR_USCLIB_REGISTER_CALLBACK;
}

int USCXMLCLIBAPI usclib_RegisterInterpreterEnterCallback(UsclibInterpreter * AInterpreter, CALLBACK_USCLIB_INTERPRETER_ENTER_EXIT ACallback, void * AUser)
{
	try {
		CHECK_INTERPRETER_VALID(AInterpreter);

		ScxmlBase *AScxmlBase = reinterpret_cast<ScxmlBase*>(AInterpreter);
		AScxmlBase->registerOnEnter(ACallback, AUser);

		return USCLIB_SUCCESS;
	}
	CATCH_USCLIB_ALL;

	return ERROR_USCLIB_REGISTER_CALLBACK;
}

int USCXMLCLIBAPI usclib_RegisterInterpreterInvokeCallback(UsclibInterpreter * AInterpreter, CALLBACK_USCLIB_INTERPRETER_INVOKE ACallback, void * AUser)
{
	try {
		CHECK_INTERPRETER_VALID(AInterpreter);

		ScxmlBase *AScxmlBase = reinterpret_cast<ScxmlBase*>(AInterpreter);
		AScxmlBase->registerOnInvoke(ACallback, AUser);

		return USCLIB_SUCCESS;
	}
	CATCH_USCLIB_ALL;

	return ERROR_USCLIB_REGISTER_CALLBACK;
}

int USCXMLCLIBAPI usclib_RegisterInterpreterEventCallback(UsclibInterpreter * AInterpreter, CALLBACK_USCLIB_INTERPRETER_EVENT ACallback, const bool bAtomOrJson, void * AUser)
{
	try {
		CHECK_INTERPRETER_VALID(AInterpreter);

		ScxmlBase *AScxmlBase = reinterpret_cast<ScxmlBase*>(AInterpreter);
		AScxmlBase->registerOnEvent(ACallback, bAtomOrJson, AUser);

		return USCLIB_SUCCESS;
	}
	CATCH_USCLIB_ALL;

	return ERROR_USCLIB_REGISTER_CALLBACK;
}

int USCXMLCLIBAPI usclib_RegisterInterpreterStoppedCallback(UsclibInterpreter * AInterpreter, CALLBACK_USCLIB_INTERPRETER_NOTIFY ACallback, void * AUser)
{
	try {
		CHECK_INTERPRETER_VALID(AInterpreter);

		ScxmlBase *AScxmlBase = reinterpret_cast<ScxmlBase*>(AInterpreter);
		AScxmlBase->registerOnStopped(ACallback, AUser);

		return USCLIB_SUCCESS;
	}
	CATCH_USCLIB_ALL;

	return ERROR_USCLIB_REGISTER_CALLBACK;
}

int USCXMLCLIBAPI usclib_StartInterpreter(UsclibInterpreter * AInterpreter, const char * chScxmlTextOrFile, const bool bIsText)
{	
	try {
		CHECK_INTERPRETER_VALID(AInterpreter);

		ScxmlBase *AScxmlBase = reinterpret_cast<ScxmlBase*>(AInterpreter);
		AScxmlBase->start(chScxmlTextOrFile, bIsText);
		
		return USCLIB_SUCCESS;
	}
	CATCH_USCLIB_ALL;

	return ERROR_USCLIB_START_INTERPRETER;
}

int USCXMLCLIBAPI usclib_PauseInterpreter(UsclibInterpreter * AInterpreter)
{
	try {
		CHECK_INTERPRETER_VALID(AInterpreter);

		ScxmlBase *AScxmlBase = reinterpret_cast<ScxmlBase*>(AInterpreter);
		AScxmlBase->pause();

		return USCLIB_SUCCESS;
	}
	CATCH_USCLIB_ALL;

	return ERROR_USCLIB_PAUSE_INTERPRETER;
}

int USCXMLCLIBAPI usclib_ResumeInterpreter(UsclibInterpreter * AInterpreter)
{
	try {
		CHECK_INTERPRETER_VALID(AInterpreter);

		ScxmlBase *AScxmlBase = reinterpret_cast<ScxmlBase*>(AInterpreter);
		AScxmlBase->resume();

		return USCLIB_SUCCESS;
	}
	CATCH_USCLIB_ALL;

	return ERROR_USCLIB_RESUME_INTERPRETER;
}

int USCXMLCLIBAPI usclib_StopInterpreter(UsclibInterpreter * AInterpreter)
{
	try {
		CHECK_INTERPRETER_VALID(AInterpreter);

		ScxmlBase *AScxmlBase = reinterpret_cast<ScxmlBase*>(AInterpreter);
		AScxmlBase->close();

		return USCLIB_SUCCESS;
	}
	CATCH_USCLIB_ALL;

	return ERROR_USCLIB_STOP_INTERPRETER;
}

int USCXMLCLIBAPI usclib_WaitForInterpreterStopped(UsclibInterpreter * AInterpreter)
{
	try {
		CHECK_INTERPRETER_VALID(AInterpreter);

		ScxmlBase *AScxmlBase = reinterpret_cast<ScxmlBase*>(AInterpreter);
		AScxmlBase->waitForStopped();

		return USCLIB_SUCCESS;
	}
	CATCH_USCLIB_ALL;
	return ERROR_USCLIB_WAIT_FOR_STOPPED;
}

int USCXMLCLIBAPI usclib_IsInterpreterInState(const UsclibInterpreter * AInterpreter, const char* chState, bool * bInState)
{
	try {
		CHECK_INTERPRETER_VALID(const_cast<UsclibInterpreter *>(AInterpreter));

		const ScxmlBase *AScxmlBase = reinterpret_cast<const ScxmlBase*>(AInterpreter);
		auto state = AScxmlBase->getState();
		if (state != USCXML_UNDEF && AScxmlBase->getImpl())
			*bInState = AScxmlBase->getImpl()->isInState(chState);
		else
			*bInState = false;

		return USCLIB_SUCCESS;
	}
	CATCH_USCLIB_ALL;
	return ERROR_USCLIB_IS_IN_STATE;
}

int USCXMLCLIBAPI usclib_InitHTTP(const int iHttpListenPort, const int iHttpWebsocketPort)
{
	try {
		HTTPServer::getInstance(iHttpListenPort, iHttpWebsocketPort);
		g_HTTP_ENABLED = true;
		return USCLIB_SUCCESS;
	}
	CATCH_USCLIB_ALL;

	return ERROR_USCLIB_INIT_HTTP;
}

int USCXMLCLIBAPI usclib_GetDefaultInterpreterOptions(UsclibInterpreterOptions * AInterpreterOptions)
{
	AInterpreterOptions->Monitor = true;
	AInterpreterOptions->RemoteMonitorHost = "127.0.0.1";
	AInterpreterOptions->RemoteMonitorPort = SCXML_DISABLE_REMOTE_MONITOR;
	AInterpreterOptions->CheckIssues = true;
	AInterpreterOptions->MonitorMsgTypes = USCLIB_SCXMLEDITOR_MSG_TYPES;

	return USCLIB_SUCCESS;
}

int USCXMLCLIBAPI usclib_Log(const unsigned int nSeverity, const char *chMessage) {
	
	BOOST_LOG_SEV(g_LOGGER::get(), SysLogSeverity(nSeverity)) << chMessage;
	
	return USCLIB_SUCCESS;
}

int USCXMLCLIBAPI usclib_TriggerEvent(UsclibInterpreter * AInterpreter, const char * chEvent)
{
	try {
		CHECK_INTERPRETER_VALID(AInterpreter);

		ScxmlBase *AScxmlBase = reinterpret_cast<ScxmlBase*>(AInterpreter);

		uscxml::Event AEvent;
		AEvent.eventType = uscxml::Event::EXTERNAL;
		AEvent.name = chEvent;

		AScxmlBase->receive(AEvent);

		return  USCLIB_SUCCESS;
	}
	CATCH_USCLIB_ALL;

	return ERROR_USCLIB_TRIGGER_INT;
}

int USCXMLCLIBAPI usclib_TriggerIntEvent(UsclibInterpreter *AInterpreter, const char *chEvent, const int Data) {
	try {
		CHECK_INTERPRETER_VALID(AInterpreter);

		ScxmlBase *AScxmlBase = reinterpret_cast<ScxmlBase*>(AInterpreter);

		uscxml::Event AEvent;
		AEvent.eventType = uscxml::Event::EXTERNAL;
		AEvent.name = chEvent;
		AEvent.data.atom = std::to_string(Data);
		AEvent.data.type = uscxml::Data::INTERPRETED;

		AScxmlBase->setGlobal(AEvent.name, "", AEvent.data, 0);
		AScxmlBase->receive(AEvent);

		return  USCLIB_SUCCESS;
	}
	CATCH_USCLIB_ALL;

	return ERROR_USCLIB_TRIGGER_INT;
}

int USCXMLCLIBAPI usclib_TriggerStringEvent(UsclibInterpreter *AInterpreter, const char * chEvent, const char * Data)
{
	try {
		CHECK_INTERPRETER_VALID(AInterpreter);

		ScxmlBase *AScxmlBase = reinterpret_cast<ScxmlBase*>(AInterpreter);

		uscxml::Event AEvent;
		AEvent.eventType = uscxml::Event::EXTERNAL;
		AEvent.name = chEvent;
		AEvent.data.atom = Data ? Data : "";
		AEvent.data.type = uscxml::Data::VERBATIM;

		AScxmlBase->setGlobal(AEvent.name, "", AEvent.data, 0);
		AScxmlBase->receive(AEvent);

		return  USCLIB_SUCCESS;
	}
	CATCH_USCLIB_ALL;

	return  ERROR_USCLIB_TRIGGER_STR;
}

int USCXMLCLIBAPI usclib_TriggerDoubleEvent(UsclibInterpreter *AInterpreter, const char * chEvent, const double Data)
{
	try {
		CHECK_INTERPRETER_VALID(AInterpreter);

		ScxmlBase *AScxmlBase = reinterpret_cast<ScxmlBase*>(AInterpreter);

		uscxml::Event AEvent;
		AEvent.eventType = uscxml::Event::EXTERNAL;
		AEvent.name = chEvent;
		AEvent.data.atom = std::to_string(Data);
		AEvent.data.type = uscxml::Data::INTERPRETED;

		AScxmlBase->setGlobal(AEvent.name, "", AEvent.data, 0);
		AScxmlBase->receive(AEvent);

		return  USCLIB_SUCCESS;
	}
	CATCH_USCLIB_ALL;

	return  ERROR_USCLIB_TRIGGER_DBL;
}

int USCXMLCLIBAPI usclib_TriggerJsonEvent(UsclibInterpreter * AInterpreter, const char * chEvent, const char * JsonData)
{
	try {
		CHECK_INTERPRETER_VALID(AInterpreter);

		ScxmlBase *AScxmlBase = reinterpret_cast<ScxmlBase*>(AInterpreter);

		uscxml::Event AEvent;
		AEvent.eventType = uscxml::Event::EXTERNAL;
		AEvent.name = chEvent;
		AEvent.data = uscxml::Data::fromJSON(JsonData);

		AScxmlBase->setGlobal(AEvent.name, "", AEvent.data, 0);
		AScxmlBase->receive(AEvent);

		return  USCLIB_SUCCESS;
	}
	CATCH_USCLIB_ALL;

	return ERROR_USCLIB_TRIGGER_JSON;
}

int USCXMLCLIBAPI usclib_GlobalCleanup(void)
{
	try {
		Factory::getInstance().cleanup();
		return  USCLIB_SUCCESS;
	}
	CATCH_USCLIB_ALL;

	return ERROR_USCLIB_GLOBAL_CLEANUP;
}


