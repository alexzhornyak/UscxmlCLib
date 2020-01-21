#pragma once

#if defined(_USRDLL)
	#define USCXMLTOVCLAPI		__declspec(dllexport)	
#else
	#define USCXMLTOVCLAPI		__declspec(dllimport)
#endif

#define USTOVCL_SUCCESS							0
#define ERROR_USTOVCL_INIT_LOG					1
#define ERROR_USTOVCL_INIT_USCXML_EVENT			2
#define ERROR_USTOVCL_INIT_USCXML_STANDARD		3

extern "C" const char USCXMLTOVCLAPI * ustovcl_GetLastError(void);

extern "C" int USCXMLTOVCLAPI ustovcl_OpenInterpreter(const unsigned int Handle, const char *chScxmlName, 
	const char **chCMD, const int nCMD,
	const bool bCheckIssues);
extern "C" int USCXMLTOVCLAPI ustovcl_CloseInterpreter();
extern "C" int USCXMLTOVCLAPI ustovcl_Log(const unsigned int nSeverity, const char *chMessage);
extern "C" int USCXMLTOVCLAPI ustovcl_TriggerEvent(const char *chEvent, const int Data);
extern "C" int USCXMLTOVCLAPI ustovcl_TriggerStringEvent(const char *chEvent, const char *Data);
extern "C" int USCXMLTOVCLAPI ustovcl_TriggerDoubleEvent(const char *chEvent, const double Data);
extern "C" int USCXMLTOVCLAPI ustovcl_FreeArrayPointer(void *ptr);

