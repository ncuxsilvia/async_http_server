#ifndef SERVER_HPP
#define SERVER_HPP

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include "config.hpp"
#include "session.hpp"
#include "session_manager.hpp"

namespace server {
//  namespace http {
    namespace system  = boost::system;
    namespace asio    = boost::asio;
    namespace ip      = asio::ip;

    class xserver : boost::noncopyable
    {
    public:
      // main section
      xserver(config::string_array const& args);

    static
      int main(int ac, char* av[]);
      int start();

      //

      // handlers section
      void handle_accept(const system::error_code& ec);
      void handle_stop();

    private:
      int init();
      //int run();
      int listen();
      void do_accept();

      bool read_options();
      void apply_options();

    private:
      // config part
      config::string_array    args_;
      config::settings        conf_;

      // network part
      asio::io_service        ios_;
      ip::tcp::acceptor       acceptor_;


      ip::tcp::socket         socket_;

      // signals part
      asio::signal_set        signals_;

      http::session_manager         manager_;
    };
//  } // namespace <http>
} // namespace <server>

#endif // SERVER_HPP
