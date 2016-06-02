#include "request_handler.hpp"

namespace server {
  namespace http {

    request_handler::request_handler(std::string& root)
      : static_root_(root)
    {
    }

    void request_handler::handle(http_request_header header)
    {
      std::string path;
      if (!url::decode(header.uri, path))
      {

      }
      //else if
    }




  }
}
