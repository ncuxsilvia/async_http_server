#define BOOST_LOG_DYN_LINK

#include <array>
#include <boost/bind.hpp>
#include <fstream>

#include "session.hpp"
#include "session_manager.hpp"
#include "boost/log/trivial.hpp"
#include "http-header/http_header_parser.hpp"



namespace server {
  namespace http {
    using socket = asio::ip::tcp::socket;


    struct file_io_handler{
      typedef std::array<char, 4096> io_buffer;
      typedef std::function<void(const system::error_code&)> io_handler;

      // ctor
      file_io_handler(
        socket&         sock,
        io_buffer&      buffer,
        file_pointer&   file
      )
        : offset_(0),
          socket_(socket_),
          buffer_(buffer_),
          file_(file)
          //handler_(std::move(handler_))
      {}

      void operator()(const system::error_code& error, std::size_t bytes)
      {
         if (!!error)
           complete(/*error*/);

         file_->read (buffer_.data (), buffer_.size ());
      };

      void complete()
      {

      }

    private:
      std::size_t     offset_;
      socket&         socket_;
      io_buffer       buffer_;
      file_pointer    file_;
    };

    session::session(
        asio::ip::tcp::socket socket,
        session_manager& manager
    ) : socket_(std::move(socket)),
        session_manager_(manager),
        file_(0)
        //, ...
    {}

    void session::start()
    {
      do_read ();
    }

    void session::stop()
    {
      socket_.close ();
    }

    void session::do_read()
    {
      socket_.async_read_some(
        asio::buffer (buffer_),
        boost::bind(
          &session::handle_request,
          shared_from_this (),
          ph::error,
          ph::bytes_transferred
        )
      );
    }

    void session::do_write()
    {

    }

    void session::handle_request(const system::error_code &ecode, std::size_t bytes_transferred)
    {
      if (!ecode)
      {
        http_request_header_type header;
        if (parse_http_header (std::string(buffer_.data (), buffer_.data() + bytes_transferred), header))
        {
          do_read();
        }
        else
        {
          ///
        }
      }
      else if (ecode != asio::error::operation_aborted)
      {
        BOOST_LOG_TRIVIAL(error)
            << "read request complete with error, reason:"
            << ecode.message ();
        session_manager_.stop (shared_from_this());
      }
    }
  } // namespace <http>
} // namespace <server>
