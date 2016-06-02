#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#include <string>
#include <boost/asio.hpp>
#include "http-header/http_header_parser.hpp"

namespace server {
  namespace http {

    class request_handler
      : boost::noncopyable
    {
    public:
      explicit request_handler(std::string& root);

      void handle(http_request_header header/*, reply_type& reply*/);

    private:
      std::string   static_root_;
    };

  }
}

#endif // REQUEST_HANDLER_HPP
