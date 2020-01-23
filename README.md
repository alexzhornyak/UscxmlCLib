# USCXML C-Style Wrapper (for C++ Builder, Delphi)
![intro](https://github.com/alexzhornyak/UscxmlCLib/blob/master/Examples/StateCharts/TrafficLight.gif)

## State Machines in C++ Builder, Delphi
[USCXML](https://github.com/tklab-tud/uscxml) is a great library and I always wanted to use it in C++ Builder, but unfortunately Borland C++ Compiler is quite a specific thing. After many unsuccessful attempts to compile it with Borland C++ Compiler I decided to create wrapper which would be easy to install. Solution was to create C library which would be definetely built on all versions of Borland C++ Compiler. Also I decided to simplify some things which seemed to be redundant for using in C++ Builder.

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

3. Start inperpreter
```
if (USCLIB_SUCCESS != usclib_OpenInterpreter(&g_Interpreter, 0, 0, 0))
	throw Exception(usclib_GetLastError());

usclib_RegisterLogCallback(g_Interpreter, OnInterpreterLogCallback, NULL);

if (USCLIB_SUCCESS != usclib_StartInterpreter(g_Interpreter, g_chScxml, true))
	throw Exception(usclib_GetLastError());

if (USCLIB_SUCCESS != usclib_CloseInterpreter(g_Interpreter))
	throw Exception(usclib_GetLastError());
```
### Output
![image](https://user-images.githubusercontent.com/18611095/73010483-2ee4b100-3e1b-11ea-86cc-ad7be2ba65e1.png)

## Examples
After [installation](https://github.com/alexzhornyak/UscxmlCLib/blob/master/README.md#installation) you are able to start [ready-to-use examples](https://github.com/alexzhornyak/UscxmlCLib/tree/master/Examples): [HelloWorld](https://github.com/alexzhornyak/UscxmlCLib/tree/master/Examples/BCB/CBuilder2010/HelloWorld), [Traffic Light](https://github.com/alexzhornyak/UscxmlCLib/tree/master/Examples/BCB/CBuilder2010/TrafficLight) etc.

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
Almost the same as uSCXML original with some restrictions. Run W3C [testing application](https://github.com/alexzhornyak/UscxmlCLib/tree/master/Examples/BCB/CBuilder2010/TesterW3C) to get more info

## DataModels supported
1. NULL
2. Lua
3. Luavia (Lua datamodel with some non-standard extensions to increase state chart performance)
