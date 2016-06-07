#ifndef HTTP_HEADER_HPP
#define HTTP_HEADER_HPP

#include <string>
#include <map>
#include <tuple>
#include <iomanip>

#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/std_pair.hpp>

namespace server{
  namespace http {
    namespace qi = boost::spirit::qi;

    typedef std::map<std::string, std::string> map_type;
    typedef std::string::iterator iterator;
    typedef std::tuple<bool, iterator> result_type;

    struct request
    {
      std::string     method;
      std::string     uri;
      std::string     http_version;
      map_type        header_fields;
    };

    result_type parse_http_header(std::string& str, request& http_request);

    namespace url {
      bool encode(const std::string &url, std::string& out);
      bool decode(const std::string &url, std::string& out);
    }

  } // namespace <http>
} // namespace <server>

#endif
