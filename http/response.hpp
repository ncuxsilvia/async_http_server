#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <array>

#include <boost/asio.hpp>

namespace server {
  namespace http {
    namespace asio = boost::asio;
    using const_buffer = boost::asio::const_buffer;

    enum status_code {
      ok              = 200,
      bad_request     = 400,
      not_found       = 404,
      internal_error  = 500,
      not_implemented = 501,
      unavailable     = 503
    };

    namespace special {
      static const char crlfcrlf[]    = "\r\n\r\n";
      static const char crlf[]      = "\r\n" ;
      static const char pair_sep[]  = ": " ;
    }

    typedef std::pair<std::string, std::string> answer_type;

    static std::map<status_code, answer_type>
    status_string {

      {status_code::ok,               { "HTTP/1.0 200 OK\r\n",

                                        "<html>"
                                        "<head><title>Created</title></head>"
                                        "<body><h1>201 Created</h1></body>"
                                        "</html>"
                                      }
      },

      {status_code::bad_request,      { "HTTP/1.0 400 Bad Request\r\n",

                                        "<html>"
                                        "<head><title>Bad Request</title></head>"
                                        "<body><h1>400 Bad Request</h1></body>"
                                        "</html>"
                                      }
      },

      {status_code::not_found,        { "HTTP/1.0 404 Not Found\r\n",

                                        "<html>"
                                        "<head><title>Not Found</title></head>"
                                        "<body><h1>404 Not Found</h1></body>"
                                        "</html>"
                                      }
      },

      {status_code::internal_error,   { "HTTP/1.0 500 Internal Server Error\r\n",

                                        "<html>"
                                        "<head><title>Internal Server Error</title></head>"
                                        "<body><h1>500 Internal Server Error</h1></body>"
                                        "</html>"
                                      }
      },

      {status_code::not_implemented,  { "HTTP/1.0 501 Not Implemented\r\n",

                                        "<html>"
                                        "<head><title>Not Implemented</title></head>"
                                        "<body><h1>501 Not Implemented</h1></body>"
                                        "</html>"
                                      }
      },

      {status_code::unavailable,      { "HTTP/1.0 503 Service Unavailable\r\n",

                                        "<html>"
                                        "<head><title>Service Unavailable</title></head>"
                                        "<body><h1>503 Service Unavailable</h1></body>"
                                        "</html>"
                                      }
      }
    };

    struct response {
      typedef std::shared_ptr<response>   pointer;

      std::string                         status;
      std::map<std::string, std::string>  headers;
      std::string                         content;
      std::string                         path;

      std::string merge();

      static
      response default_response(const status_code code);
    };

  } // namespace <http>
} // namespace <server>


#endif // RESPONSE_HPP
