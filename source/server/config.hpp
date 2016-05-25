#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <type_traits>
#include <boost/program_options.hpp>

namespace server
{
  namespace config
  {
    namespace action
    {
      enum type {
        error,
        exit,
        run
      }; // enum <type>
    } // namespace <action>

    // log::severity - this is a temporary dummy
    namespace log
    {
      enum class
      severity : unsigned
      {
        trace,
        debug,
        info,
        warning,
        error,
        fatal
      };
    } // namespace log

    namespace defaults
    {
      static log::severity  default_log_level     = log::severity::info;
      static std::string    default_host_name     = "";
      static std::string    default_listen_addr   = "127.0.0.1";
      static uint16_t       default_listen_port   = 80;
      static uint16_t       default_thread_count  = std::thread::hardware_concurrency();
    } // namecpace <defaults>

    typedef std::vector<std::string> string_array;
    string_array collect(int ac, char* av[]);

    struct settings
    {
      settings();
      // [logging]
      //std::string           log_path;
      log::severity         log_level;

      // [network]
      std::string           host_name;
      unsigned int          netaddr;
      std::string           listen_addr;
      uint16_t              listen_port;

      // [multithreading]
      unsigned int          thread_count;

      void reset();      
    };

    namespace describe
    {
      boost::program_options::options_description& server_actions(
        boost::program_options::options_description& desc
      );

      boost::program_options::options_description& server_runopts(
        boost::program_options::options_description& desc,
        settings* conf = nullptr
      );
    } // namespace <describe>

    action::type invoke_action(
          string_array const& args,
          string_array&       others
          );
  } // namespace <config>
} // namespace <server>

#endif // CONFIG_HPP

