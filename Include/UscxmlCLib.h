#pragma once

#if defined(USCXMLCLIBSRC_EXPORTS)
#define USCXMLCLIBAPI		__declspec(dllexport)	
#else
#define USCXMLCLIBAPI		__declspec(dllimport)
#endif

#define USCLIB_SUCCESS							0
#define ERROR_USCLIB_INIT_LOG					1
#define ERROR_USCLIB_OPEN_INTERPRETER			2
#define ERROR_USCLIB_CLOSE_INTERPRETER			3
#define ERROR_USCLIB_REGISTER_CALLBACK			4
#define ERROR_USCLIB_START_INTERPRETER			5
#define ERROR_USCLIB_INIT_HTTP					6
#define ERROR_USCLIB_GLOBAL_CLEANUP				7
#define ERROR_USCLIB_PAUSE_INTERPRETER			8
#define ERROR_USCLIB_RESUME_INTERPRETER			9
#define ERROR_USCLIB_STOP_INTERPRETER			10
#define ERROR_USCLIB_WAIT_FOR_STOPPED			11
#define ERROR_USCLIB_IS_IN_STATE				12
#define ERROR_USCLIB_TRIGGER					100
#define ERROR_USCLIB_TRIGGER_INT				101
#define ERROR_USCLIB_TRIGGER_DBL				102
#define ERROR_USCLIB_TRIGGER_STR				103
#define ERROR_USCLIB_TRIGGER_JSON				104


#define USCLIB_LOG_EMERGENCY					0
#define USCLIB_LOG_ALERT						1
#define USCLIB_LOG_CRITICAL						2
#define USCLIB_LOG_ERROR						3
#define USCLIB_LOG_WARN							4
#define USCLIB_LOG_NOTICE						5
#define USCLIB_LOG_INFO							6
#define USCLIB_LOG_DEBUG						7


#define USCLIB_MSG_UNKNOWN						0
#define USCLIB_MSG_AFTER_ENTER					2
#define USCLIB_MSG_BEFORE_ENTER					4
#define USCLIB_MSG_AFTER_EXIT					8
#define USCLIB_MSG_BEFORE_EXIT					16
#define USCLIB_MSG_STEP							32
#define USCLIB_MSG_BEFORE_EXECCONTENT			64
#define USCLIB_MSG_AFTER_EXECCONTENT			128
#define USCLIB_MSG_BEFORE_INVOKE				256
#define USCLIB_MSG_AFTER_INVOKE					512
#define USCLIB_MSG_BEFORE_UNINVOKE				1024
#define USCLIB_MSG_AFTER_UNINVOKE				2048
#define USCLIB_MSG_BEFORE_TAKINGTRANSITION		4096
#define USCLIB_MSG_AFTER_TAKINGTRANSITION		8192
#define USCLIB_MSG_MAXSIZE						16384

#define USCLIB_SCXML_EDITOR_MSG_TYPES			(USCLIB_MSG_BEFORE_ENTER | USCLIB_MSG_BEFORE_EXIT | USCLIB_MSG_BEFORE_TAKINGTRANSITION | USCLIB_MSG_BEFORE_INVOKE | USCLIB_MSG_BEFORE_UNINVOKE)

typedef void	UsclibInterpreter;

typedef struct {	
	bool LocalMonitor;
	const char *RemoteMonitorHost;
	int  RemoteMonitorPort;
	bool CheckIssues;
	int  MonitorMsgTypes;
} UsclibInterpreterOptions;

/* 
if (UsclibInterpreterOptions *AInterpreterOptions == NULL) then
	LocalMonitor =			true
	RemoteMonitorHost =		127.0.0.1
	RemoteMonitorPort =		0 (disabled)
	CheckIssues =			true
	MonitorMsgTypes =		USCLIB_SCXML_EDITOR_MSG_TYPES

P.S. call 'usclib_GetDefaultInterpreterOptions' to retrieve default values
*/

typedef void(__stdcall *CALLBACK_USCLIB_INTERPRETER_LOG)
	(const UsclibInterpreter *AInterpreter, const int nSeverity, const char *chMessage, void *AUser);
typedef void(__stdcall *CALLBACK_USCLIB_INTERPRETER_NOTIFY)
	(const UsclibInterpreter *AInterpreter, void *AUser);
typedef void(__stdcall *CALLBACK_USCLIB_INTERPRETER_ENTER)
	(const UsclibInterpreter *AInterpreter, const char *chStateMachineName, const char *chStateName, const bool bEnter, void *AUser);
typedef void(__stdcall *CALLBACK_USCLIB_INTERPRETER_INVOKE)
	(const UsclibInterpreter *AInterpreter, const char *chStateMachineName, const char *chInvokeName, const bool bInvoke, void *AUser);
typedef void(__stdcall *CALLBACK_USCLIB_INTERPRETER_EVENT)
	(const UsclibInterpreter *AInterpreter, const char *chStateMachineName, const char *chEventName, const char *chAtomOrJsonData, const bool bIsAtomOrJson, void *AUser);

/* 'BasicHTTP' support (called once at the begin of App) */
extern "C" int USCXMLCLIBAPI usclib_InitHTTP(const int iHttpListenPort, const int iHttpWebsocketPort);

extern "C" const char USCXMLCLIBAPI * usclib_GetLastError(void);
extern "C" int USCXMLCLIBAPI usclib_GetDefaultInterpreterOptions(UsclibInterpreterOptions *AInterpreterOptions);

/* Logging */
extern "C" int USCXMLCLIBAPI usclib_InitLogging(const char *chLogFileName); // called once at the beginning
extern "C" int USCXMLCLIBAPI usclib_Log(const unsigned int nSeverity, const char *chMessage);

extern "C" int USCXMLCLIBAPI usclib_OpenInterpreter(UsclibInterpreter **AInterpreter,
	const char **chCMD, const int nCMD,	const UsclibInterpreterOptions *AInterpreterOptions);
extern "C" int USCXMLCLIBAPI usclib_CloseInterpreter(UsclibInterpreter *AInterpreter);

extern "C" int USCXMLCLIBAPI usclib_RegisterLogCallback(UsclibInterpreter *AInterpreter, CALLBACK_USCLIB_INTERPRETER_LOG ACallback, void *AUser);
extern "C" int USCXMLCLIBAPI usclib_RegisterInterpreterEnterCallback(UsclibInterpreter *AInterpreter, CALLBACK_USCLIB_INTERPRETER_ENTER ACallback, void *AUser);
extern "C" int USCXMLCLIBAPI usclib_RegisterInterpreterInvokeCallback(UsclibInterpreter *AInterpreter, CALLBACK_USCLIB_INTERPRETER_INVOKE ACallback, void *AUser);
extern "C" int USCXMLCLIBAPI usclib_RegisterInterpreterEventCallback(UsclibInterpreter *AInterpreter, CALLBACK_USCLIB_INTERPRETER_EVENT ACallback, const bool bAtomOrJson, void *AUser);
extern "C" int USCXMLCLIBAPI usclib_RegisterInterpreterStoppedCallback(UsclibInterpreter *AInterpreter, CALLBACK_USCLIB_INTERPRETER_NOTIFY ACallback, void *AUser);

extern "C" int USCXMLCLIBAPI usclib_StartInterpreter(UsclibInterpreter *AInterpreter, const char *chScxmlTextOrFile, const bool bIsText);
extern "C" int USCXMLCLIBAPI usclib_PauseInterpreter(UsclibInterpreter *AInterpreter);
extern "C" int USCXMLCLIBAPI usclib_ResumeInterpreter(UsclibInterpreter *AInterpreter);
extern "C" int USCXMLCLIBAPI usclib_StopInterpreter(UsclibInterpreter *AInterpreter);
extern "C" int USCXMLCLIBAPI usclib_WaitForInterpreterStopped(UsclibInterpreter *AInterpreter);

extern "C" int USCXMLCLIBAPI usclib_IsInterpreterInState(const UsclibInterpreter *AInterpreter, const char* chState, bool *bInState);

extern "C" int USCXMLCLIBAPI usclib_TriggerEvent(UsclibInterpreter *AInterpreter, const char *chEvent);
extern "C" int USCXMLCLIBAPI usclib_TriggerIntEvent(UsclibInterpreter *AInterpreter, const char *chEvent, const int Data);
extern "C" int USCXMLCLIBAPI usclib_TriggerStringEvent(UsclibInterpreter *AInterpreter, const char *chEvent, const char *Data);
extern "C" int USCXMLCLIBAPI usclib_TriggerDoubleEvent(UsclibInterpreter *AInterpreter, const char *chEvent, const double Data);
extern "C" int USCXMLCLIBAPI usclib_TriggerJsonEvent(UsclibInterpreter *AInterpreter, const char *chEvent, const char *JsonData);

/* 'Factory Cleanup' (called once at the end of App) */
extern "C" int USCXMLCLIBAPI usclib_GlobalCleanup(void);

