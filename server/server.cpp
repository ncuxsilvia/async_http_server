#define BOOST_LOG_DYN_LINK

#include <boost/bind.hpp>
#include <boost/filesystem.hpp>


#include "server.hpp"
#include "config.hpp"
#include "logging.hpp"


#include <boost/date_time/posix_time/posix_time.hpp>

namespace server
{
  namespace po    = boost::program_options;
  namespace fs    = boost::filesystem;

  xserver::xserver(const config::string_array& args)
    : args_(args),
      signals_(ios_, SIGINT, SIGTERM),
      acceptor_(ios_),
      socket_(ios_)
    // ...
  {
    signals_.async_wait(
      boost::bind(&xserver::handle_stop, this)
    );
  }

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

      int rcode = 0;
      if (action_code == config::action::run)
      {
        xserver srv(runopts);
        rcode = srv.start();
      }

      return rcode;
    }
    catch(std::exception const& ex) // add processing different exception (e.g.: config, server, log, etc..)
    {
      std::cerr << "[fatal] " << ex.what () << std::endl;
    }
    catch(...)
    {
      std::cerr << "[fatal] Unknown error" << std::endl;
      throw;  // if need core dump
    }

    return -1;
  }

  int xserver::start()
  {
    // here may be put some code for forking (if necessary) ...
    int error = init();
    if (!error) error = listen();
    //if (!error) error =


    //if

    return error;
  }

  void xserver::handle_stop()
  {
    std::cout << "[info] Server stopping...";
  }

  int xserver::init()
  {    
    if (!read_options())
      return -1;
  }

  int xserver::listen()
  {
    system::error_code ecode;

    asio::ip::tcp::resolver         resolver(ios_);
    asio::ip::tcp::resolver::query  query(conf_.listen_addr, conf_.listen_port);

    asio::ip::tcp::endpoint         endpoint = *resolver.resolve(query, ecode);
    if (ecode)
    {
      BOOST_LOG_TRIVIAL(error) << ecode.message();
      return ecode.value();
    }

    acceptor_.open(endpoint.protocol(), ecode);
    if (ecode)
    {
      BOOST_LOG_TRIVIAL(error) << ecode.message();
      return ecode.value();
    }

    acceptor_.set_option(asio::ip::tcp::acceptor::reuse_address(true));
    if (ecode)
    {
      BOOST_LOG_TRIVIAL(error) << ecode.message();
      return ecode.value();
    }

    acceptor_.bind(endpoint, ecode);
    if (ecode)
    {
      BOOST_LOG_TRIVIAL(error) << ecode.message();
      return ecode.value();
    }

    acceptor_.listen(asio::socket_base::max_connections, ecode);
    if (ecode)
    {
      BOOST_LOG_TRIVIAL(error) << ecode.message();
      return ecode.value();
    }

    return 0;
  }

  void xserver::do_accept()
  {
    acceptor_.async_accept(socket_,
      [this](system::error_code ecode)
      {
        if (!acceptor_.is_open())
          return;

        if (!ecode)
        {
          // connection manager start processing connection, but we continue ...
        }

        do_accept();
      });
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

      apply_options();
    }
    catch(po::error const& ex)
    {
      std::cerr << "[fatal] Error reading command line argument list, reason: " << ex.what() << ".";
      return false;
    }
    catch(std::exception const& ex)
    {
      std::cerr << "[fatal] " << ex.what();
      return false;
    }

    return true;
  }

  void xserver::apply_options()
  {    
    if (fs::is_directory(conf_.log_path))
    {
      if (!conf_.log_path.empty() && (*conf_.log_path.rbegin() != '/'))
        conf_.log_path += '/';

      log::init_log(conf_.log_path, log::trivial::debug);
    }
    else
    {
      std::cerr << "[error] '--log-path' is not exists or is not directory!";
    }

  }
}
