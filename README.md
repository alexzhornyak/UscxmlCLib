# USCXML C-Style Wrapper (for C++ Builder, Delphi)
![intro](https://github.com/alexzhornyak/UscxmlCLib/blob/master/Examples/Images/TrafficLight.gif)

## State Machines in C++ Builder, Delphi
The main objective of the project is to execute dynamic SCXML state charts in C++ Builder by using such a great library as
[USCXML](https://github.com/tklab-tud/uscxml). But the problem is that USCXML source code can not be compiled by Borland C++ Compiler (not full C11 support, compiler bugs etc). We have an option to use [uscxml-transformer application](https://github.com/tklab-tud/uscxml/blob/master/src/apps/uscxml-transform.cpp) for transpiling SCXML source code to ANSI-C. But in this case you have to compile application every time once you've made corrections to state chart. The solution seems to have a common library wrapper with only C-functions, which could be executed on all versions of C++ Builder.

**UscxmlCLib** consists of only 1 lib and 1 header which should be added to C++ Builder to start executing state charts.

## Supported Operating System
Windows 7, Windows 8, Windows 10

## Supported Compilers for building UscxmlCLib from source code
MSVC 2015 SP3

## Supported Compilers for using UscxmlCLib.dll
**UscxmlCLib\Lib\UscxmlCLib_borland.lib** - for Borland C++ Compiler (tests were performed on **C++ Builder 2010**, **C++ Builder XE7**)

**UscxmlCLib\Lib\UscxmlCLib.lib** - for MSVC 2015 SP3

## Installation
1. Install **[Visual C++ Redistributable for Visual Studio 2015 SP3](https://www.microsoft.com/en-us/download/details.aspx?id=48145)**

2. Put **UscxmlCLib\Bin** binaries to Windows search path or add dir with binaries to PATH environment settings

## Build from source code
There will be special article

## Getting Started
Let's start ['Hello, world' example](https://github.com/alexzhornyak/SCXML-tutorial#hello-world) in C++ Builder

![hello_world](https://github.com/alexzhornyak/SCXML-tutorial/blob/master/Images/1%20-%20Hello%20world.gif)

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

## Licensing
UscxmlCLib itself is distributed under under the terms of the BSD 3-Clause License but it include external components with their own licensies

| Project | License | Comment |
|---------|---------|---------|
| [uSCXML](https://github.com/tklab-tud/uscxml) | [Simplified BSD license](https://github.com/tklab-tud/uscxml/blob/master/License.md) | This wrapper mostly based on [uSCXML original SDK](https://github.com/tklab-tud/uscxml) |
| [libcurl](https://curl.haxx.se/libcurl/) | [MIT/X derivate](https://curl.haxx.se/docs/copyright.html) | Used in uSCXML to fetch remote content |
| [Xerces-C++](https://xerces.apache.org/xerces-c/) | [Apache v2](http://www.apache.org/licenses/LICENSE-2.0.html) | XML parser and DOM implementation |
| [libevent](http://libevent.org) | [3-clause BSD](http://libevent.org/LICENSE.txt) | Delayed event queues |
| [uriparser](http://uriparser.sourceforge.net) | [New BSD](https://sourceforge.net/p/uriparser/git/ci/master/tree/COPYING) | Referring and resolving URIs |
| [Lua](https://www.lua.org/) | [Under the terms of the MIT license](https://www.lua.org/license.html) | Lua Data Model |

## SCXML Standard Compliance
Almost the same as uSCXML original with some restrictions. Run W3C [testing application](https://github.com/alexzhornyak/UscxmlCLib/tree/master/Examples/BCB/TesterW3C) to get more info

## DataModels supported
1. NULL
2. Lua
3. Luavia (Lua datamodel with some non-standard extensions to increase state chart performance)

## Examples
After [installation](https://github.com/alexzhornyak/UscxmlCLib/blob/master/README.md#installation) you are able to start [ready-to-use examples](https://github.com/alexzhornyak/UscxmlCLib/tree/master/Examples): [HelloWorld](https://github.com/alexzhornyak/UscxmlCLib/tree/master/Examples/BCB/HelloWorld), [Traffic Light](https://github.com/alexzhornyak/UscxmlCLib/tree/master/Examples/BCB/TrafficLight) etc.

### Take special look at [KT76C Transponder Example](https://github.com/alexzhornyak/UscxmlCLib/tree/master/Examples/BCB/KT76CSim)\!
It has **ZERO** lines of device logic in C++! Just connected GUI controls!

![KT76C](https://github.com/alexzhornyak/UscxmlCLib/blob/master/Examples/Images/KT76C_App_Example.png)

![KT76C_Logic](https://github.com/alexzhornyak/UscxmlCLib/blob/master/Examples/Images/KT76C.png)
