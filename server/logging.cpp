#define BOOST_LOG_DYN_LINK

#include <boost/format.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/attributes.hpp>

#include "logging.hpp"

namespace server {
  namespace log {

    namespace logging = boost::log;
    namespace keywords = boost::log::keywords;
    namespace expr    = boost::log::expressions;

    void init_log(std::string path, trivial::severity_level lvl)
    {
      boost::log::add_common_attributes();
      boost::log::core::get()->add_global_attribute(
          "Scope", boost::log::attributes::named_scope()
      );

      boost::log::core::get()->set_filter(
          boost::log::trivial::severity >= lvl
      );

      auto fmt_time_stamp =
          expr::format_date_time<
              boost::posix_time::ptime
          > ("TimeStamp", "%Y-%m-%d %H:%M:%S.%f");

      auto fmt_thread_id = expr::attr<boost::log::attributes::current_thread_id::value_type>("ThreadID");
      auto fmt_severity = expr::attr<boost::log::trivial::severity_level>("Severity");
      auto fmt_scope = expr::format_named_scope("Scope",
          boost::log::keywords::format = "%n(%f:%l)",
          boost::log::keywords::iteration = boost::log::expressions::forward,
          boost::log::keywords::depth = 3);

      boost::log::formatter log_fmtr =
          expr::format("[%1%] (%2%) [%3%] [%4%] %5%")
          % fmt_time_stamp
          % fmt_thread_id
          % fmt_severity
          % fmt_scope
          % boost::log::expressions::smessage;


      path += "server_%Y-%m-%d_%H-%M-%S.%N.log";

      auto file_sink = logging::add_file_log(
        keywords::file_name  = path,
        keywords::auto_flush = true,
        keywords::open_mode  = (std::ios::out | std::ios::app)
      );


      file_sink->set_formatter(log_fmtr);

      logging::core::get()->set_filter
      (
        trivial::severity >= lvl
      );

      BOOST_LOG_TRIVIAL(debug) << "Logging subsystem initialized.";
    }
  } // namespace <log>
} // namespace <server>
