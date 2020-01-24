#pragma once

/* boost */
#include <boost/log/common.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/from_settings.hpp>
#include <boost/log/utility/setup/from_stream.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>
#include <boost/log/utility/setup/filter_parser.hpp>

/* uscxml */
#include "uscxml/interpreter/LoggingImpl.h"

/* app */
#include "boost_logger_severity.h"
#include "UscxmlCLibCallbacks.h"


//  Global logger declaration
typedef boost::log::sources::severity_logger_mt<SysLogSeverity> logger_t;
BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(g_LOGGER, logger_t);

#define CLOG(VAL)	BOOST_LOG_SEV(g_LOGGER::get(), SysLogSeverity::sl##VAL)

namespace uscxml {

	class CLibLogger : public LoggerImpl {
	private:
		static SysLogSeverity UscxmlLogNamesToBoost(LogSeverity severity);

		const bool _enabled;

		const void *_ScxmlBase;
		const OnInterpreterLog _OnInterpreterLog;
		void *_OnInterpreterLogUser;

	public:
		CLibLogger(bool enabled, const void *AScxmlBase, const OnInterpreterLog AOnInterpreterLog, void *AUser);
		virtual ~CLibLogger() {};

		virtual std::shared_ptr<LoggerImpl> create() override;

		virtual void log(LogSeverity severity, const Event& event) override;

		virtual void log(LogSeverity severity, const Data& data) override;

		virtual void log(LogSeverity severity, const std::string& message) override;
	};

}