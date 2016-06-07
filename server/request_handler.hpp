#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#include <string>
#include <memory>
#include <fstream>

#include <boost/system/error_code.hpp>

namespace server {
  namespace http {
    namespace system = boost::system;

    struct response;
    struct request;

    typedef std::shared_ptr<std::ifstream> file_pointer;

    class request_handler
    {
    public:      
      typedef std::shared_ptr<request_handler> pointer;

      request_handler(const request_handler&) = delete;
      request_handler& operator=(const request_handler&) = delete;

      explicit
      request_handler()
      {}

      file_pointer
      handle(
        request& req,
        response& resp,
        system::error_code& ecode
      );

      void set_root(std::string root)
      {static_root_.assign(root);}


    private:
      std::string           static_root_;
    };

  }
}

#endif // REQUEST_HANDLER_HPP
