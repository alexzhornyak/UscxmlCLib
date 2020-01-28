/**********************************************************************************************
	 USCXML Copyright (https://github.com/tklab-tud/uscxml)
***********************************************************************************************
*    Copyright 2017 Stefan Radomski Jens Heuschkel
*    
*    Redistribution and use in source and binary forms, with or without modification,
*    are permitted provided that the following conditions are met :
*    
*    1. Redistributions of source code must retain the above copyright notice,
*    this list of conditions and the following disclaimer.
*    
*    2. Redistributions in binary form must reproduce the above copyright notice,
*    this list of conditions and the following disclaimer in the documentation
*    and / or other materials provided with the distribution.
*    
*    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
*    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
*    THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
*    IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
*    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
*    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
*    WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE)
*    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**********************************************************************************************
	 UscxmlCLib Copyright (https://github.com/alexzhornyak/UscxmlCLib)
***********************************************************************************************
*    BSD 3 - Clause License
*    
*    Copyright(c) 2020, Alex Zhornyak
*    All rights reserved.
*    
*    Redistribution and use in source and binary forms, with or without
*    modification, are permitted provided that the following conditions are met :
*    
*    1. Redistributions of source code must retain the above copyright notice, this
*    list of conditions and the following disclaimer.
*    
*    2. Redistributions in binary form must reproduce the above copyright notice,
*    this list of conditions and the following disclaimer in the documentation
*    and / or other materials provided with the distribution.
*    
*    3. Neither the name of the copyright holder nor the names of its
*    contributors may be used to endorse or promote products derived from
*    this software without specific prior written permission.
*    
*    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
*    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
*    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*    DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
*    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
*    DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
*    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
*    OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

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
#define ERROR_USCLIB_GLOBAL_DATA				13
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

#define USCLIB_SCXMLEDITOR_MSG_TYPES			(USCLIB_MSG_BEFORE_ENTER | USCLIB_MSG_BEFORE_EXIT | USCLIB_MSG_BEFORE_TAKINGTRANSITION | USCLIB_MSG_BEFORE_INVOKE | USCLIB_MSG_BEFORE_UNINVOKE)

#define USCLIB_DISABLE_REMOTE_MONITOR				0
#define USCLIB_DEFAULT_SCXMLEDITOR_MONITOR		11005

typedef void	UsclibInterpreter;

typedef struct {						/* DEFAULT */	
	bool Monitor;						// true							
	const char *RemoteMonitorHost;		// 127.0.0.1
	int  RemoteMonitorPort;				// 0 (disabled)
	bool Validate;						// true						
	bool TerminateNotValidated;			// true
	int  MonitorMsgTypes;				// USCLIB_SCXML_EDITOR_MSG_TYPES
	bool DisableGlobalData;				// false
	bool AsyncStart;					// false
} UsclibInterpreterOptions;

typedef void(__stdcall *CALLBACK_USCLIB_GLOBAL_LOG)
	(const int nSeverity, const char *chMessage, void *AUser);
typedef void(__stdcall *CALLBACK_USCLIB_INTERPRETER_LOG)
	(const UsclibInterpreter *AInterpreter, const int nSeverity, const char *chMessage, void *AUser);
typedef void(__stdcall *CALLBACK_USCLIB_INTERPRETER_NOTIFY)
	(const UsclibInterpreter *AInterpreter, void *AUser);
typedef void(__stdcall *CALLBACK_USCLIB_INTERPRETER_ENTER_EXIT)
	(const UsclibInterpreter *AInterpreter, const char *chStateMachineName, const char *chStateName, const bool bEnter, void *AUser);
typedef void(__stdcall *CALLBACK_USCLIB_INTERPRETER_INVOKE)
	(const UsclibInterpreter *AInterpreter, const char *chStateMachineName, const char *chInvokeName, const bool bInvoke, void *AUser);
typedef void(__stdcall *CALLBACK_USCLIB_INTERPRETER_EVENT)
	(const UsclibInterpreter *AInterpreter, const char *chStateMachineName, const char *chEventName,
		const char *chAtomOrJsonData, const bool bIsAtomOrJson, void *AUser);
typedef void(__stdcall *CALLBACK_USCLIB_GET_GLOBAL_DATA)
	(const UsclibInterpreter *AInterpreter, const char *chName, const char *chPath,
		const char *chAtomOrJsonData, const bool bIsAtomOrJson, void *AUser);
typedef void(__stdcall *CALLBACK_USCLIB_GLOBAL_DATA_CHANGE)
	(const UsclibInterpreter *AInterpreter, const char *chName, const char *chPath,
		const char *chAtomOrJsonData, const bool bIsAtomOrJson, const int iType, void *AUser);

/* 'BasicHTTP' support (called once at the begin of App) */
extern "C" int USCXMLCLIBAPI usclib_InitHTTP(const int iHttpListenPort, const int iHttpWebsocketPort);

extern "C" const char USCXMLCLIBAPI * usclib_GetLastError(void);
extern "C" int USCXMLCLIBAPI usclib_GetDefaultInterpreterOptions(UsclibInterpreterOptions *AInterpreterOptions);

/* Logging */
extern "C" int USCXMLCLIBAPI usclib_InitLogging(const char *chLogFileName, CALLBACK_USCLIB_GLOBAL_LOG ACallback, void *AUser); // called once at the beginning
extern "C" int USCXMLCLIBAPI usclib_Log(const unsigned int nSeverity, const char *chMessage);

/* Intepreter */
extern "C" int USCXMLCLIBAPI usclib_OpenInterpreter(UsclibInterpreter **AInterpreter,
	const char **chCMD, const int nCMD,	const UsclibInterpreterOptions *AInterpreterOptions);
extern "C" int USCXMLCLIBAPI usclib_CloseInterpreter(UsclibInterpreter *AInterpreter);

/* Registration must be complete before interpreter was started */
extern "C" int USCXMLCLIBAPI usclib_RegisterInterpreterLogCallback(UsclibInterpreter *AInterpreter, CALLBACK_USCLIB_INTERPRETER_LOG ACallback, void *AUser);
extern "C" int USCXMLCLIBAPI usclib_RegisterInterpreterEnterCallback(UsclibInterpreter *AInterpreter, CALLBACK_USCLIB_INTERPRETER_ENTER_EXIT ACallback, void *AUser);
extern "C" int USCXMLCLIBAPI usclib_RegisterInterpreterInvokeCallback(UsclibInterpreter *AInterpreter, CALLBACK_USCLIB_INTERPRETER_INVOKE ACallback, void *AUser);
extern "C" int USCXMLCLIBAPI usclib_RegisterInterpreterEventCallback(UsclibInterpreter *AInterpreter, CALLBACK_USCLIB_INTERPRETER_EVENT ACallback, const bool bAtomOrJson, void *AUser);
extern "C" int USCXMLCLIBAPI usclib_RegisterInterpreterStoppedCallback(UsclibInterpreter *AInterpreter, CALLBACK_USCLIB_INTERPRETER_NOTIFY ACallback, void *AUser);
extern "C" int USCXMLCLIBAPI usclib_RegisterInterpreterGlobalDataChangeCallback(UsclibInterpreter *AInterpreter, const bool bIsAtomOrJson, CALLBACK_USCLIB_GLOBAL_DATA_CHANGE ACallback, void *AUser);


extern "C" int USCXMLCLIBAPI usclib_StartInterpreter(UsclibInterpreter *AInterpreter, const char *chScxmlTextOrFile, const bool bIsText);
extern "C" int USCXMLCLIBAPI usclib_PauseInterpreter(UsclibInterpreter *AInterpreter);
extern "C" int USCXMLCLIBAPI usclib_ResumeInterpreter(UsclibInterpreter *AInterpreter);
extern "C" int USCXMLCLIBAPI usclib_StopInterpreter(UsclibInterpreter *AInterpreter);
extern "C" int USCXMLCLIBAPI usclib_WaitForInterpreterStopped(UsclibInterpreter *AInterpreter);

extern "C" int USCXMLCLIBAPI usclib_IsInterpreterInState(const UsclibInterpreter *AInterpreter, const char* chState, bool *bInState);

extern "C" int USCXMLCLIBAPI usclib_TriggerEvent(UsclibInterpreter *AInterpreter, const char *chEvent);
extern "C" int USCXMLCLIBAPI usclib_TriggerIntEvent(UsclibInterpreter *AInterpreter, const char *chEvent, const int iData);
extern "C" int USCXMLCLIBAPI usclib_TriggerStringEvent(UsclibInterpreter *AInterpreter, const char *chEvent, const char *chData);
extern "C" int USCXMLCLIBAPI usclib_TriggerDoubleEvent(UsclibInterpreter *AInterpreter, const char *chEvent, const double dData);
extern "C" int USCXMLCLIBAPI usclib_TriggerJsonEvent(UsclibInterpreter *AInterpreter, const char *chEvent, const char *chJsonData);

/* Non-Standard extensions */
/* Global Data can be accessed from any invoked intepreter */
extern "C" int USCXMLCLIBAPI usclib_GetGlobalData(UsclibInterpreter *AInterpreter,
	const char *chName, const char *chPath, CALLBACK_USCLIB_GET_GLOBAL_DATA ACallback, void *AUser);
extern "C" int USCXMLCLIBAPI usclib_GetGlobalJsonData(UsclibInterpreter *AInterpreter,
	const char *chName, const char *chPath, CALLBACK_USCLIB_GET_GLOBAL_DATA ACallback, void *AUser);
extern "C" int USCXMLCLIBAPI usclib_SetGlobalIntData(UsclibInterpreter *AInterpreter,
	const char *chName, const char *chPath, const int iData, const int iType);
extern "C" int USCXMLCLIBAPI usclib_SetGlobalDoubleData(UsclibInterpreter *AInterpreter,
	const char *chName, const char *chPath, const double dData, const int iType);
extern "C" int USCXMLCLIBAPI usclib_SetGlobalStringData(UsclibInterpreter *AInterpreter,
	const char *chName, const char *chPath, const char *chData, const int iType);
extern "C" int USCXMLCLIBAPI usclib_SetGlobalJsonData(UsclibInterpreter *AInterpreter,
	const char *chName, const char *chPath, const char *chJsonData, const int iType);


/* 'Factory Cleanup' (called once at the end of App) */
extern "C" int USCXMLCLIBAPI usclib_GlobalCleanup(void);

