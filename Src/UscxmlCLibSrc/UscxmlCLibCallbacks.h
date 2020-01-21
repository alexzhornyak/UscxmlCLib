#pragma once

typedef void(__stdcall *OnInterpreterLog)(const void *AInterpreter, const int nSeverity, const char *chMessage, void *AUser);
typedef void(__stdcall *OnInterpreterEnter)(const void *AInterpreter, const char *chStateMachineName, const char *chStateName, const bool bEnter, void *AUser);
typedef void(__stdcall *OnInterpreterInvoke)(const void *AInterpreter, const char *chStateMachineName, const char *chInvokeName, const bool bInvoke, void *AUser);
typedef void(__stdcall *OnInterpreterEvent)(const void *AInterpreter, const char *chStateMachineName, const char *chEventName, const char *chAtomOrJsonData, const bool bIsAtomOrJson, void *AUser);