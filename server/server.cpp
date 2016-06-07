#define BOOST_LOG_DYN_LINK

#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <utility>

#include "server.hpp"
#include "config.hpp"
#include "logging.hpp"


#include <boost/date_time/posix_time/posix_time.hpp>

namespace server {
  namespace po    = boost::program_options;
  namespace fs    = boost::filesystem;
  namespace ph    = boost::asio::placeholders;
  using asio::ip::tcp;

  xserver::xserver(const config::string_array& args)
    : args_(args),
      signals_(ios_, SIGINT, SIGTERM),
      acceptor_(ios_),
      socket_(ios_)
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
    catch(std::exception const& ex)
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
    BOOST_LOG_TRIVIAL(trace) << "start litening ...";
    if (!error) error = listen();

    if (error) return error;

    boost::thread_group grp;
    for (size_t i = 0; i < conf_.thread_count; i++) {
      BOOST_LOG_TRIVIAL(trace) << "start thread ..." << (i+1);
      grp.create_thread(
            boost::bind(&asio::io_service::run, &ios_)
      );
    }

    do_accept ();
    grp.join_all ();

    BOOST_LOG_TRIVIAL(trace) << "all stopped ...";


    return error;
  }

  void xserver::handle_accept(const system::error_code &ecode)
  {
    BOOST_LOG_TRIVIAL(trace) << "accept new connection";
    if (!acceptor_.is_open())
    {
      BOOST_LOG_TRIVIAL(trace) << "acceptor not available";
      return;
    }

    if (!ecode)
    {
      BOOST_LOG_TRIVIAL(debug) << "connecting from " << socket_.remote_endpoint ().address ().to_string ();
      manager_.start (
        std::make_shared<http::session> (
          std::move(socket_),
          manager_,
          rhandler_,
          conf_
        )
      );
    }
    else if (ecode != system::errc::too_many_files_open){
      BOOST_LOG_TRIVIAL(error)
        << "Could not connect from remote host, reason:"
        << ecode.message();
    }

    do_accept();
  }

  void xserver::handle_stop()
  {
    manager_.stop_all();
    std::cout << "[info] Server stopping..." << std::endl;
    ios_.stop ();
  }

  int xserver::init()
  {
    BOOST_LOG_TRIVIAL(trace) << "read server options";
    if (!read_options())
      return -1;
    return 0;
  }

  int xserver::listen()
  {

    system::error_code ecode;

    tcp::endpoint endpoint = tcp::endpoint(asio::ip::tcp::v4 (), conf_.listen_port);

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
    BOOST_LOG_TRIVIAL(trace) << "waiting for new connection ...";
    acceptor_.async_accept(
      socket_,
      boost::bind(&xserver::handle_accept, this, ph::error)
    );
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
      BOOST_LOG_TRIVIAL(fatal) << ex.what() << ".";
      return false;
    }
    catch(std::exception const& ex)
    {
      BOOST_LOG_TRIVIAL(fatal) << ex.what();
      return false;
    }

    return true;
  }

  void xserver::apply_options()
  {
    BOOST_LOG_TRIVIAL(trace) << "applying options ...";
    if (fs::is_directory(conf_.log_path))
    {
      if (!conf_.log_path.empty() && (*conf_.log_path.rbegin() != '/'))
        conf_.log_path += '/';

      log::init_log(conf_.log_path, conf_.log_level);
      BOOST_LOG_TRIVIAL(trace) << "continue ...";
    }
    else
      throw po::error("'--log-path' is not exists or is not directory!");

    if (fs::is_directory(conf_.root))
    {
      if (!conf_.root.empty() && fs::is_directory (conf_.root))
        rhandler_.set_root (conf_.root);
    }
    else
      throw po::error("'--static-root' is not exists or is not directory!");

  }
} // namespace <serevr>
