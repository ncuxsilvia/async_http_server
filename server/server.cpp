//#define BOOST_LOG_DYN_LINK



#include "server.hpp"
#include "config.hpp"
#include "logging.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>

namespace server
{
  namespace po    = boost::program_options;

  int xserver::main(int ac, char *av[])
  {
    try
    { //collect command line arguments
      config::string_array args(
        config::collect(ac, av)
      );

      // read active action code
      config::string_array runopts;
      config::action::type action_code = config::invoke_action(args, runopts);

      if (action_code == config::action::run)
      {
        xserver srv(runopts);
        int result = srv.init();
      }

      return 0;
    }
    catch(std::exception const& ex) // add processing different exception (e.g.: config, server, log, etc..)
    {
      std::cout << ex.what () << std::endl;
      throw;
    }
    return -1;
  }

  int xserver::init()
  {
    log::init_log(conf_.log_path, server::log::trivial::debug);
    return read_options() ? 0 : -1;
  }

  bool xserver::read_options ()
  {
    po::options_description desc;
    po::variables_map general_opts;

    config::describe::server_runopts(desc, &conf_);

    try {
      po::store(
        po::command_line_parser(args_)
        .options(desc)
        .run(),
        general_opts
      );

      po::notify(general_opts);

    }
    catch(po::error const& ex)
    {
      std::cerr << "[fatal] Error reading command line argument list, reason: " << ex.what() << ".";
      return false;
    }
    catch(...)
    {
      std::cerr << "[fatal] Unknown error.";
      return false;
    }

    return true;
  }

  void xserver::apply_options()
  {
    //if (conf_.log_level)
  }
}
