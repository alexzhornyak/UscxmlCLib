# USCXML C-Style Wrapper (for C++ Builder, Delphi, etc.)
![intro](https://github.com/alexzhornyak/UscxmlCLib/blob/master/Examples/StateCharts/TrafficLight.gif)

## State Machines in C++ Builder, Delphi and others
[USCXML](https://github.com/tklab-tud/uscxml) is a great library and I always wanted to use it in C++ Builder, but unfortunately Borland C++ Compiler is quite a specific thing. After many unsuccessful attempts to compile it with Borland C++ Compiler I decided to create wrapper which would be easy to install. Solution was to create C library which would be definetely built on all versions of Borland C++ Compiler. Also I decided to simplify some things which seemed to be redundant for using in C++ Builder.

## Supported Operating System
Windows 7, Windows 8, Windows 10

## Supported Compilers for building USCXMLCLib from source code
MSVC 2015 SP3

## Supported Compilers for using USCXMLCLib.dll
**UscxmlCLib\Lib\UscxmlCLib_borland.lib** - Borland C++ Compiler (tests were performed on **C++ Builder 2010**, **C++ Builder XE7**)
**UscxmlCLib\Lib\UscxmlCLib.lib** - MSVC 2015 SP3

## Installation
1. Install **[Visual C++ Redistributable for Visual Studio 2015 SP3](https://www.microsoft.com/en-us/download/details.aspx?id=48145)**

2. Put **UscxmlCLib\Bin** binaries to Windows search path or add it in PATH environment settings

## Build from source code
There will be special article

