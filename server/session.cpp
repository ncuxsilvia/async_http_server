#define BOOST_LOG_DYN_LINK

#include <array>
#include <tuple>
#include <memory>
#include <fstream>
#include <sstream>
#include <iostream>
#include <thread>
#include <chrono>

#include <boost/log/trivial.hpp>
#include <boost/bind.hpp>

#include "session.hpp"
#include "session_manager.hpp"
#include "http/response.hpp"

namespace server {
  namespace http {
    using socket = asio::ip::tcp::socket;
    namespace time = boost::posix_time;

    session::session(
        asio::ip::tcp::socket socket,
        session_manager& manager,
        request_handler& handler,
        config::settings& conf
    ) : socket_(std::move(socket)),
        session_manager_(manager),
        handler_(handler),
        strand_(socket_.get_io_service ()),
        io_timer_(socket_.get_io_service ()),
        conf_(conf),
        request_buffer_(buf_size)
    { reset_file(); }

    void session::start()
    {
      do_read ();
    }

    void session::stop()
    {
      io_timer_.cancel ();
      if (socket_.is_open ())
        socket_.close ();
    }

    void session::do_read()
    {
      BOOST_LOG_TRIVIAL(trace) << "wait for new data ...";
      asio::async_read_until(
        socket_,
        request_buffer_,
        special::crlfcrlf,
        strand_.wrap(
          boost::bind(
            &session::on_read_until,
            shared_from_this (),
            asio::placeholders::error,
            asio::placeholders::bytes_transferred
          )
        )
      );
      BOOST_LOG_TRIVIAL(trace) << "set wait timer for async_read_until";
      set_timer ();
    }

    void session::do_write()
    {
      BOOST_LOG_TRIVIAL(debug) << "write response: " << response_.status ;
      socket_.async_write_some(
        asio::buffer(response_.merge()),
        strand_.wrap(
          boost::bind(
            &session::on_write,
            shared_from_this(),
            boost::asio::placeholders::error
          )
        )
      );
      set_timer();
    }



    void session::on_write(const system::error_code &ecode)
    {
      reset_timer();
      if (!ecode)
      {
        boost::system::error_code ignored_ec;
        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
      }
      if (ecode != boost::asio::error::operation_aborted)
      {
        session_manager_.stop(shared_from_this());
      }      
    }

    void session::on_timer(const system::error_code &ecode)
    {
      if(ecode == asio::error::operation_aborted)
        return ;

      if (io_timer_.expires_at() > asio::deadline_timer::traits_type::now())
        return;

      std::string message = "keep-alive timeout (close).";

      BOOST_LOG_TRIVIAL (debug) << "connection: " << message;

      session_manager_.stop(shared_from_this());
    }

    void session::init_send_file()
    {
      socket_.async_write_some(
        asio::buffer(response_.merge ()),
        strand_.wrap(
          boost::bind(
            &session::on_send_part,
            shared_from_this(),
            asio::placeholders::error
          )
        )
      );
      set_timer();
    }

    void session::read_part()
    {
      if (file_to_send_->is_open())
      {
        file_to_send_->read(response_buffer_.data(), buf_size);
        std::size_t now_readed = file_to_send_->gcount();
        if (now_readed)
        {
          readed_bytes_ += now_readed;
          socket_.async_write_some(
            asio::buffer(response_buffer_, now_readed),
            strand_.wrap(
              boost::bind(
                &session::on_send_part,
                shared_from_this (),
                asio::placeholders::error
              )
            )
          );
          set_timer();
        }
      }
    }

    void session::reset_file()
    {
      if (!file_to_send_)
        file_to_send_.reset();
      file_size_ = 0;
      readed_bytes_ = 0;
    }

    void session::on_send_part(const system::error_code &ecode)
    {
      reset_timer();
      if (!ecode)
      {

        if (readed_bytes_ == file_size_)
        {
            BOOST_LOG_TRIVIAL(debug)
                << "file sended (ok) "
                << connection_info ();
            reset_file();

            boost::system::error_code ignored_ec;
            socket_.shutdown(asio::ip::tcp::socket::shutdown_both, ignored_ec);

            session_manager_.stop(shared_from_this());
        }
        else if (readed_bytes_ < file_size_)
        {
            BOOST_LOG_TRIVIAL(trace)
                << "transferred " << readed_bytes_ << " of " << file_size_
                << connection_info ();
            read_part();
        }
        else {
          session_manager_.stop(shared_from_this());
        }
      }
      else
      {
        BOOST_LOG_TRIVIAL(error)
            << "read request complete with error, reason:"
            << ecode.message ();
        session_manager_.stop (shared_from_this());
      }
    }

    void session::on_read_until(
      const system::error_code &ecode,
      std::size_t bytes_transferred
    )
    {
      reset_timer();
      if (!ecode)
      {
        BOOST_LOG_TRIVIAL(trace) << "readed " << bytes_transferred << "bytes";

        bool result;
        std::stringstream ss;
        ss << &request_buffer_;
        std::string raw_data(ss.str());

        std::tie(result, std::ignore) = parse_http_header(raw_data, request_);
        if (result)
        {
          system::error_code err;
          file_to_send_ = handler_.handle(request_, response_, err);

          if (err)
          {
            if (err.value () == system::errc::too_many_files_open) {
              BOOST_LOG_TRIVIAL(warning) << "opened max descriptors ...";
            }
            else {
              BOOST_LOG_TRIVIAL(debug) << err.message();
            }
            do_write();
          }

          if (file_to_send_)
          {
            file_size_ = file_to_send_->tellg();
            file_to_send_->seekg(0, std::ios::beg);
            init_send_file();
          }
          else
          {
            response_ = response::default_response (internal_error);
            do_write();
          }
        }
        else
        {
          response_ = response::default_response(bad_request);
          do_write();
        }
      }
      else {
          BOOST_LOG_TRIVIAL(error)
            << "read request complete with notice, warning:"
            << ecode.message ();

          session_manager_.stop (shared_from_this());
      }
    }

    void session::set_timer()
    {
      if (conf_.keep_alive < 1)
        return;

      io_timer_.expires_from_now(
        time::seconds(
          conf_.keep_alive
        )
      );

      io_timer_.async_wait(
        strand_.wrap(
          boost::bind(
            &session::on_timer,
            this->shared_from_this(),
            boost::asio::placeholders::error
          )
        )
      );
    }

    void session::reset_timer()
    {
      if (conf_.keep_alive > 0)
        io_timer_.cancel();
    }

    std::string session::connection_info()
    {
      std::string info = "(connection lost)";
      if (socket_.is_open ())
      {
        info = "";
        system::error_code ecode;
        asio::ip::tcp::endpoint ep =  socket_.remote_endpoint(ecode);
        if (!ecode)
        {
          info += "( ";
          info += socket_.remote_endpoint().address().to_string();
          info += ":";
          info += std::to_string(socket_.remote_endpoint().port());
          info += " )";
        }
      }
      return info;
    }

  } // namespace <http>
} // namespace <server>
