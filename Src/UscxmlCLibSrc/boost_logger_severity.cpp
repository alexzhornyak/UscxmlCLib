#include "boost_logger_severity.h"

// The operator is used for regular stream formatting
std::ostream& operator<< (std::ostream& strm, SysLogSeverity level)
{
	static const char* strings[] =
	{
		"EMERGENCY",
		"ALERT",
		"CRITICAL",
		"ERROR",
		"WARNING",
		"NOTICE",
		"INFO",
		"DEBUG"
	};

	if (static_cast< std::size_t >(level) < sizeof(strings) / sizeof(*strings))
		strm << strings[static_cast<std::size_t>(level)];
	else
		strm << level;

	return strm;
}

std::istream& operator>>(std::istream& i, SysLogSeverity &level) {
	std::string s_tmp;
	i >> s_tmp;

	static const char* strings[] =
	{
		"EMERGENCY",
		"ALERT",
		"CRITICAL",
		"ERROR",
		"WARNING",
		"NOTICE",
		"INFO",
		"DEBUG"
	};

	bool bFound = false;
	const std::size_t i_array_size = sizeof(strings) / sizeof(*strings);
	for (int k = 0; k < i_array_size ; k++)
	{
		if (s_tmp == std::string(strings[k]))
		{
			level = SysLogSeverity(k);
			bFound = true;
			break;
		}
	}

	if (!bFound)
	{
		level = SysLogSeverity(std::stoul(s_tmp));
	}

	return i;
}