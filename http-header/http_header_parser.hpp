#ifndef HTTP_HEADER_HPP
#define HTTP_HEADER_HPP

#include <string>
#include <map>

namespace server{
  namespace http {

    struct http_url_type
    {
      typedef std::map<std::string, std::string> query_map_type;

      std::string     host;
      std::string     port;
      std::string     path;
      query_map_type  query_map;
    };

    struct http_request_header_type
    {
      typedef std::map<std::string, std::string> field_map_type;

      std::string     method;
      std::string     uri;
      std::string     http_version;
      field_map_type  header_fields;
    };

    bool parse_http_url(const std::string& str, http_url_type& http_url);
    bool parse_http_header(const std::string& str, http_request_header_type& header);

  } // namespace <http>
} // namespace <server>

#endif
