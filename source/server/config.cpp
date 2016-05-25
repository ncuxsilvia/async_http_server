#include <iostream>
#include <exception>
#include <boost/system/error_code.hpp>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>

#include "config.hpp"

namespace server
{
  namespace config
  {
    namespace asio    = boost::asio;
    namespace system  = boost::system;
    namespace po      = boost::program_options;
    using               boost::asio::ip::tcp;

    string_array collect(int ac, char* av[])
    {
      string_array args;
      try
      {
        po::options_description empty;
        po::parsed_options parsed = po::command_line_parser(ac, av)
          .allow_unregistered()
          .options(empty)
          .run();
        args = po::collect_unrecognized(
          parsed.options,
          po::include_positional
        );
      }
      catch(po::error const& ex)
      { // rethrow error...
        throw ex;
          //util::throw_error<config::error>(ex.what());
      }
      return args;
    }

    settings::settings()
      : netaddr(0)
    {      
      reset();
    }

    void settings::reset()
    {
      log_level = defaults::default_log_level;
      host_name = defaults::default_host_name;
      listen_addr = defaults::default_listen_addr;
      listen_port = defaults::default_listen_port;
      thread_count = defaults::default_thread_count;
    }

    namespace describe
    {
      po::options_description& server_actions(po::options_description &desc)
      {
        desc.add_options()
            ("help,h", "Display this help and exit.")
            ("run,R", "Run server.");

        return desc;
      }

      po::options_description& server_runopts(po::options_description &desc, settings *conf)
      {
        desc.add_options()
//            (
//              "log-level,l",
//              po::value<log::severity>(conf ? &conf->log_level : 0)->default_value(defaults::default_log_level, "info"),
//              "Logging level.\nPossible values: critical, error, warning, info, debug, trace."
//            )
            (
              "listen-address,A",
              po::value<std::string>(conf ? &conf->listen_addr : 0),
              "HTTP server listen address."
            )
            (
              "port,p",
              po::value<uint16_t>(conf ? &conf->listen_port : 0)->default_value(defaults::default_listen_port),
              "HTTP server listen port."
            )
            (
              "thread-count,t",
              po::value<unsigned int>(conf ? &conf->thread_count : 0)->default_value(defaults::default_thread_count),
              "How many threads will be run.\nNotice: it is not recommended to specify more than N*2 threads, where N is the number of cores."
            );

        return desc;
      }
    }

    inline
      action::type invoke_help()
    {
      po::options_description actions("Server actions");
      po::options_description options("Server options");

      po::options_description user_options;
      user_options
          .add(describe::server_actions(actions))
          .add(describe::server_runopts(options));

      std::cout << user_options << std::endl;
      return action::exit;
    }

    action::type invoke_action(
      string_array const& args,
      string_array& others
      )
    {
      try
      {
        po::options_description actions;
        describe::server_actions(actions);
        po::parsed_options parsed = po::command_line_parser(args)
            .options(actions)
            .allow_unregistered()
            .run();

        others = po::collect_unrecognized(
              parsed.options,
              po::include_positional
              );

        po::variables_map opts;
        po::store(parsed, opts);
        po::notify(opts);

        enum {
          make_run   = 1,
          make_help    = 2
        };

        int swch = (
            (opts.count("help") > 0 ? make_help : 0)
            |(opts.count("run") > 0 ? make_run : 0)
          );

        if(swch == 0)
        { // print help
          swch = make_help;
        }

        switch(swch)
        {
          case make_help:
            return invoke_help();
          case make_run:
            return action::run;
        }

        // error: multiple actions at the same time
        bool empty = true;
        std::string emessage("Actions ambiguity detected. Which actions to apply ");
        if (swch & make_help)
        {
          emessage += "'--help'";
          empty = false;
        }
        if(swch & make_run)
        {
          if(!empty)
          {
            emessage += ", ";
          }
          emessage += "'--run'";
          empty = false;
        }
        emessage += "? Only one is allowed at the same time.";
        throw std::runtime_error(emessage);
      }
      catch(po::error const& ex)
      {
        // something actions for process exception
        std::cout << ex.what() << std::endl;
      }
      return action::error;
    }
  } // namespace <config>
} // namespace <server>
