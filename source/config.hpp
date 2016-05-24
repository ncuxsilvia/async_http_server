#include <iostream>
#include <memory>
#include <string>
#include <boost/program_options.hpp>
#include <syslog.h>



namespace server
{
  namespace config
  {
    typedef std::vector< std::string > string_array;

    // log::severity - this is a temporary dummy
    namespace log
    {
      enum class
      severity {
        trace,
        debug,
        info,
        warning,
        error,
        fatal
      };
    } // namespace log

    struct settings
    {
      settings();
      // [logging]
      std::string           log_path;
      log::severity         log_level;

      // [network]
      std::string           host_name;
      unsigned int          netaddr;
      std::string           listen_addr;
      uint16_t              listen_port;

      // [multithreading]
      uint16_t              threads_num;

      void reset();      
    }

    string_array collect(int ac, char* av[]);
  }
}

