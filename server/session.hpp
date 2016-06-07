#ifndef SESSION_HPP
#define SESSION_HPP

#include <array>
#include <memory>
#include <mutex>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include "config.hpp"
#include "request_handler.hpp"
#include "http/parser.hpp"
#include "http/response.hpp"


namespace server {
  namespace http {
    namespace system  = boost::system;
    namespace asio    = boost::asio;
    namespace ph      = asio::placeholders;

    const int buf_size = 65535; // 64 Kb

    class session_manager;
    class request_parser;
    class response;

    class session
      : public std::enable_shared_from_this<session>,
        boost::noncopyable
    {
    public:
      typedef std::shared_ptr<session> pointer;
      friend class request_handler;
    public:
      explicit session(
          boost::asio::ip::tcp::socket socket,
          session_manager& manager,
          request_handler& handler,
          config::settings& conf
      );

      void start();
      void stop();

      void stop(
        const system::error_code& ecode
      );

    private:
      void do_read();
      void do_write();

      // handlers
      void on_read(
        const system::error_code &ecode,
        std::size_t bytes
      );

      void on_write(
        const system::error_code& ecode
      );

      void on_timer(
        const system::error_code& ecode
      );

      void init_send_file();
      void read_part();
      void reset_file();

      void on_send_part(
        const system::error_code& ecode
      );

      void on_read_until(
        const system::error_code& ecode,
        std::size_t bytes_transferred
      );

      void set_timer();
      void reset_timer();

      std::string
      connection_info();

    private:
      typedef std::array<char, buf_size> buffer_type;

      boost::asio::ip::tcp::socket  socket_;
      session_manager&              session_manager_;
      asio::streambuf               request_buffer_;
      buffer_type                   response_buffer_;

      request_handler&              handler_;
      response                      response_;
      request                       request_;

      asio::io_service::strand      strand_;
      asio::deadline_timer          io_timer_;

      config::settings&             conf_;

      std::ifstream::pos_type       file_size_;
      std::ifstream::pos_type       readed_bytes_;
      file_pointer                  file_to_send_;

    };
  } // namespace <http>
} // namespace <server>

#endif // SESSION_HPP
