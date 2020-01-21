#include "UscxmlClibLogger.h"

#include <boost/algorithm/string.hpp>
#include <mutex>

namespace uscxml {

	std::mutex g_LoggerMtx;

	SysLogSeverity CLibLogger::UscxmlLogNamesToBoost(LogSeverity severity) {
		switch (severity) {
		case LogSeverity::USCXML_TRACE:
		case LogSeverity::USCXML_SCXML:
		case LogSeverity::USCXML_LOG: // from log element
		case LogSeverity::USCXML_VERBATIM: // no additional decorations
		case LogSeverity::USCXML_DEBUG: return SysLogSeverity::slDEBUG;
		case LogSeverity::USCXML_INFO: return SysLogSeverity::slINFO;
		case LogSeverity::USCXML_WARN: return SysLogSeverity::slWARN;
		case LogSeverity::USCXML_ERROR: return SysLogSeverity::slERROR;
		case LogSeverity::USCXML_FATAL: return SysLogSeverity::slCRITICAL;
		}
		return SysLogSeverity::slDEBUG;
	}

	CLibLogger::CLibLogger(bool enabled, const void *AScxmlBase, const OnInterpreterLog AOnInterpreterLog, void *AUser) :
		_enabled(enabled), _ScxmlBase(AScxmlBase), _OnInterpreterLog(AOnInterpreterLog), _OnInterpreterLogUser(AUser) {
	
	};

	std::shared_ptr<LoggerImpl> CLibLogger::create() {
		return std::shared_ptr<LoggerImpl>(new CLibLogger(_enabled, _ScxmlBase, _OnInterpreterLog, _OnInterpreterLogUser));
	}

	void CLibLogger::log(LogSeverity severity, const Event& event) {
		if (!_enabled) return;

		std::lock_guard<std::mutex> lock(g_LoggerMtx);
		std::stringstream ss;
		ss << event;
		const std::string &s_out = boost::algorithm::trim_copy(ss.str());
		if (!s_out.empty()) {
			auto nSev = UscxmlLogNamesToBoost(severity);
			BOOST_LOG_SEV(g_LOGGER::get(), nSev) << s_out;
			if (_OnInterpreterLog) {
				_OnInterpreterLog(_ScxmlBase, static_cast<int>(nSev), s_out.c_str(), _OnInterpreterLogUser);
			}
		}
	}

	void CLibLogger::log(LogSeverity severity, const Data& data) {
		if (!_enabled) return;

		std::lock_guard<std::mutex> lock(g_LoggerMtx);
		std::stringstream ss;
		ss << data;
		const std::string &s_out = boost::algorithm::trim_copy(ss.str());
		if (!s_out.empty()) {
			auto nSev = UscxmlLogNamesToBoost(severity);
			BOOST_LOG_SEV(g_LOGGER::get(), UscxmlLogNamesToBoost(severity)) << s_out;
			if (_OnInterpreterLog) {
				_OnInterpreterLog(_ScxmlBase, static_cast<int>(nSev), s_out.c_str(), _OnInterpreterLogUser);
			}
		}
	}

	void CLibLogger::log(LogSeverity severity, const std::string& message) {
		if (!_enabled) return;

		std::lock_guard<std::mutex> lock(g_LoggerMtx);
		std::stringstream ss;
		ss << message;
		const std::string &s_out = boost::algorithm::trim_copy(ss.str());
		if (!s_out.empty()) {
			auto nSev = UscxmlLogNamesToBoost(severity);
			BOOST_LOG_SEV(g_LOGGER::get(), UscxmlLogNamesToBoost(severity)) << s_out;
			if (_OnInterpreterLog) {
				_OnInterpreterLog(_ScxmlBase, static_cast<int>(nSev), s_out.c_str(), _OnInterpreterLogUser);
			}
		}
	}

}