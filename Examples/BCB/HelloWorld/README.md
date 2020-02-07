# HelloWorld

![hello_world](https://github.com/alexzhornyak/SCXML-tutorial/blob/master/Images/1%20-%20Hello%20world.gif)

Simple example from [SCXML tutorial](https://github.com/alexzhornyak/SCXML-tutorial#hello-world)

1. Add library to project
```
#include "UscxmlCLib.h"

#pragma comment(lib,"UscxmlCLib_borland.lib")
```

2. Set callbacks from interpreter and notifiers for main thread
```
class TLogNotify : public TIdNotify {
	const UnicodeString FMessage;

protected:
	virtual void __fastcall DoNotify(void) {
		FormHelloWorld->Memo1->Lines->Add(FMessage);
	}

public:
	__fastcall TLogNotify(const UnicodeString &sMsg) : FMessage(sMsg) {
	}
};

void __stdcall OnInterpreterLogCallback(const UsclibInterpreter *AInterpreter, const int nSeverity, const char *chMessage, void *AUser) {
	(new TLogNotify(chMessage))->Notify();
}
```

3. Work with interprter
```
if (USCLIB_SUCCESS != usclib_OpenInterpreter(&g_Interpreter, 0, 0, 0))
	throw Exception(usclib_GetLastError());

usclib_RegisterLogCallback(g_Interpreter, OnInterpreterLogCallback, NULL);

if (USCLIB_SUCCESS != usclib_StartInterpreter(g_Interpreter, g_chScxml, USCLIB_SCXML_AS_TEXT))
	throw Exception(usclib_GetLastError());

if (USCLIB_SUCCESS != usclib_CloseInterpreter(g_Interpreter))
	throw Exception(usclib_GetLastError());
```
### Output
![image](https://user-images.githubusercontent.com/18611095/73010483-2ee4b100-3e1b-11ea-86cc-ad7be2ba65e1.png)
