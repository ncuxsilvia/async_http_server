#ifndef SESSION_HPP
#define SESSION_HPP

#include <array>
#include <memory>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <boost/asio/deadline_timer.hpp>

#include "http-header/http_header_parser.hpp"

namespace server {
  namespace http {
    namespace system  = boost::system;
    namespace asio    = boost::asio;
    namespace ph      = asio::placeholders;

    typedef std::shared_ptr<std::ifstream> file_pointer;

    class session_manager;

    class session
      : public std::enable_shared_from_this<session>,
        boost::noncopyable
    {
    public:
      typedef std::shared_ptr<session> pointer;

    public:
//      session(const session&) = delete;
//      session& operator=(const session&) = delete;

      explicit session(
          boost::asio::ip::tcp::socket socket,
          session_manager& manager
      );

      void start();
      void stop();

    private:
      void do_read();
      void do_write();

      // handlers
      void on_read(
        const system::error_code &ecode,
        std::size_t byte_count
      );



    private:
      typedef std::array<char, 16384> buffer_type;

      boost::asio::ip::tcp::socket  socket_;
      session_manager&              session_manager_;
      buffer_type                   buffer_;
      file_pointer                  file_;

      asio::deadline_timer          io_timer_;

    };



  } // namespace <http>
} // namespace <server>

#endif // SESSION_HPP
