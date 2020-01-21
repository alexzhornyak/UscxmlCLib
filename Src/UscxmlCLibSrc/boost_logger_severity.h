#pragma once

#include <iostream>
#include <boost/log/sinks/syslog_backend.hpp>

//Here we define our application severity levels.
enum class SysLogSeverity
{
	slEMERGENCY = boost::log::sinks::syslog::emergency,                
	slALERT = boost::log::sinks::syslog::alert,                    
	slCRITICAL = boost::log::sinks::syslog::critical,                 
	slERROR = boost::log::sinks::syslog::error,                    
	slWARN = boost::log::sinks::syslog::warning,                  
	slNOTICE = boost::log::sinks::syslog::notice,                   
	slINFO = boost::log::sinks::syslog::info,                     
	slDEBUG = boost::log::sinks::syslog::debug                     
};

// The operator is used for regular stream formatting
std::ostream& operator<< (std::ostream& strm, SysLogSeverity level);

std::istream& operator>>(std::istream& i, SysLogSeverity &level);