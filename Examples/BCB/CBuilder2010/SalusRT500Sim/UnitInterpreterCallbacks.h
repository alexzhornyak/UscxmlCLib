//---------------------------------------------------------------------------

#ifndef UnitInterpreterCallbacksH
#define UnitInterpreterCallbacksH
//---------------------------------------------------------------------------

#include "UscxmlCLib.h"

extern bool g_Closing; // must be changed only in GUI thread (prevent async nofify after App was closed)

void __stdcall OnInterpreterEnterExit(const UsclibInterpreter *AInterpreter, const char *chStateMachineName, const char *chStateName,
	const bool bEnter, void *AUser);

void __stdcall OnInterpreterEvent(const UsclibInterpreter *AInterpreter, const char *chStateMachineName, const char *chEventName,
	const char *chAtomOrJsonData, const bool bIsAtomOrJson, void *AUser);

#endif
