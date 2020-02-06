//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "JsonVclUtils.h"

#include <boost/regex.hpp>

//---------------------------------------------------------------------------

#pragma package(smart_init)

namespace Jsonutils {

	const UnicodeString JsonTrimSpaces(const UnicodeString &sJson) {
		boost::wregex regEx(L"(\"(?:[^\"\\\\]|\\\\.)*\")|\\s+");
		return boost::regex_replace(std::wstring(sJson.c_str()), regEx, L"$1").c_str();
	}

}
