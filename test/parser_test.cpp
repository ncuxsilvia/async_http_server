#include <iostream>
#include <string>
#include "catch.hpp"
#include "http-header/http_header_parser.hpp"

namespace server {
  namespace http {

    enum {
      host = 0,
      user_agent,
      accept,
      accept_language,
      accept_encoding,
      accept_charset,
      keep_alive,
      connection,
      cookie,
      pragma,
      cache_control
    };

    std::vector<std::string> field_list
    {
      {"Host"},
      {"User-Agent"},
      {"Accept"},
      {"Accept-Language"},
      {"Accept-Encoding"},
      {"Accept-Charset"},
      {"Keep-Alive"},
      {"Connection"},
      {"Cookie"},
      {"Pragma"},
      {"Cache-Control"}
    };

    std::vector<std::string> value_list
    {
      {"net.tutsplus.com"},
      {"Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.5) Gecko/20091102 Firefox/3.5.5 (.NET CLR 3.5.30729)"},
      {"text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"},
      {"en-us,en;q=0.5"},
      {"gzip,deflate"},
      {"ISO-8859-1,utf-8;q=0.7,*;q=0.7"},
      {"300"},
      {"keep-alive"},
      {"PHPSESSID=r2t5uvjq435r4q7ib3vtdjq120"},
      {"no-cache"},
      {"no-cache"}
    };

    http_request_header_type header;
    http_url_type            url;

    std::string header_text =
        "GET http://www.example.com:8080/path/to/file?query1=1&query2=2 HTTP/1.1\r\n"                                                     // method, uri, http version
        "Host: net.tutsplus.com\r\n"                                                                                                      // field 1
        "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.5) Gecko/20091102 Firefox/3.5.5 (.NET CLR 3.5.30729)\r\n"   // field 2
        "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"                                                     // field 3
        "Accept-Language: en-us,en;q=0.5\r\n"                                                                                             // field 4
        "Accept-Encoding: gzip,deflate\r\n"                                                                                               // field 5
        "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n"                                                                              // field 6
        "Keep-Alive: 300\r\n"                                                                                                             // field 7
        "Connection: keep-alive\r\n"                                                                                                      // field 8
        "Cookie: PHPSESSID=r2t5uvjq435r4q7ib3vtdjq120\r\n"                                                                                // field 9
        "Pragma: no-cache\r\n"                                                                                                            // field 10
        "Cache-Control: no-cache\r\n";                                                                                                    // field 11

    TEST_CASE("Передадим http-заголовок в текстовом виде парсеру ...") {

      SECTION("Результат теста парсера http заголовков ...") {
        bool result = parse_http_header (header_text, header);
        CHECK(result);
      }

      SECTION("Проверим, получили ли мы метод (GET, POST, ...) "){
        CHECK_FALSE(header.method.empty ());
        CHECK(header.method == "GET");
      }

      SECTION("Проверка наличия uri ..."){
        CHECK_FALSE(header.uri.empty ());
        CHECK(header.uri == "http://www.example.com:8080/path/to/file?query1=1&query2=2");
      }

      SECTION("Проверка версии http-протокола ..."){
        CHECK_FALSE(header.http_version.empty ());
        CHECK(header.http_version == "1.1");
      }

      SECTION("Проверка полей заголовка ...") {
        http_request_header_type::field_map_type& fields = header.header_fields;

        REQUIRE(fields.size () == 11);

        for (size_t i = 0; i < field_list.size(); i++)
        {
          REQUIRE(fields.find (field_list[i]) != fields.end());
          REQUIRE(fields[field_list[i]] == value_list[i]);
        }
      }
    } // TEST_CASE

    TEST_CASE("Парсинг url-адреса ..."){

      SECTION(""){
        CHECK(parse_http_url (header.uri, url));
      }

      SECTION("Проверка полей url-структуры ...") {
        CHECK(url.host == "www.example.com");
        CHECK(url.path == "/path/to/file");
        CHECK(url.port == "8080");
        CHECK(url.query_map["query1"] == "1");
        CHECK(url.query_map["query2"] == "2");
      }

    }

  } // namespace <http>
} // namespace <server>
