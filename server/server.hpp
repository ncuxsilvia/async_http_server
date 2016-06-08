#ifndef SERVER_HPP
#define SERVER_HPP

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include "config.hpp"
#include "session.hpp"
#include "session_manager.hpp"
#include "request_handler.hpp"

namespace server {
  namespace system  = boost::system;
  namespace asio    = boost::asio;
  namespace ip      = asio::ip;

  class xserver
  {
  public:
    // main section
    xserver(const xserver&) = delete;
    xserver& operator=(const xserver&) = delete;

    explicit
    xserver(config::string_array const& args);

    static
    int main(int ac, char* av[]);
    int start();

  private:
    void handle_accept(const system::error_code& ec);
    void handle_stop();

  private:
    int init();
    int listen();
    void do_accept();

    bool read_options();
    void apply_options();

  private:
    config::string_array    args_;
    config::settings        conf_;
    asio::io_service        ios_;
    ip::tcp::acceptor       acceptor_;
    ip::tcp::socket         socket_;
    asio::signal_set        signals_;
    http::session_manager   manager_;
    http::request_handler   rhandler_;

  };
} // namespace <server>

#endif // SERVER_HPP
