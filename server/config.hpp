#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <type_traits>

#include <boost/program_options.hpp>
#include <boost/log/trivial.hpp>

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

    using boost::log::trivial::severity_level;

    namespace defaults
    {
      static const severity_level default_log_level     = severity_level::info;
      static const uint16_t       default_listen_port   = 8080;
      static const int            default_keep_alive    = 100;
      static const uint16_t       default_thread_count  = std::thread::hardware_concurrency();
    } // namecpace <defaults>

    typedef std::vector<std::string> string_array;
    string_array collect(int ac, char* av[]);

    struct settings
    {
      settings();
      // [logging]
      std::string           log_path;
      severity_level        log_level;

      // [network]
      uint16_t              listen_port;
      int                   keep_alive;

      // [multithreading]
      unsigned int          thread_count;

      // [static path]
      std::string           root;

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

