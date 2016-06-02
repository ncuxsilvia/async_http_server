#include <iostream>
#include <string>
#include "catch.hpp"
#include "http-header/http_header_parser.hpp"
#include "test_data.hpp"

namespace server {
  namespace test {

    using server::http::http_request_header;
    using server::http::http_url_type;
    using namespace data;

    TEST_CASE("Тестим парсер заголовков, complete ", "[complete]" ) {

      http_request_header       header;

      SECTION("Результат парсинга, complete") {

        bool result;
        std::string::const_iterator at;
        std::tie(result, at) = parse_http_header (complete_header::text, header);

        REQUIRE(result == true);
        CHECK(at == complete_header::text.end());

        CHECK(header.method == complete_header::method);
        CHECK(header.uri == complete_header::uri);
        CHECK(header.http_version == complete_header::version);

        REQUIRE(header.header_fields.size() == complete_header::key_value.size());

        for (auto pair: header.header_fields)
        {
          key_value_type::const_iterator it = complete_header::key_value.find(pair.first);
          if (it != complete_header::key_value.end())
            REQUIRE(pair.second == it->second);
          else
            FAIL("Header field \"" << pair.first << "\" not exists!");
        }


      }

    }

    TEST_CASE("Тестим парсер заголовков, alternative ", "[alternative]" ) {

      http_request_header       header;
      http_url_type             url;

      SECTION("Результат парсинга, alternative") {
        bool result;
        std::string::const_iterator at;
        std::tie(result, at) = http::parse_http_header (alternative_header::text, header);

        REQUIRE(result == true);
        CHECK(at == alternative_header::text.end());

        CHECK(header.method == alternative_header::method);
        CHECK(header.uri == alternative_header::uri);
        CHECK(header.http_version == alternative_header::version);

        REQUIRE(header.header_fields.size() == alternative_header::key_value.size());

        for (auto pair: header.header_fields)
        {
          key_value_type::const_iterator it = alternative_header::key_value.find(pair.first);
          if (it != alternative_header::key_value.end())
            REQUIRE(pair.second == it->second);
          else
            FAIL("Header field \"" << pair.first << "\" not exists!");
        }
      }

    }

    TEST_CASE("Тестим парсер заголовков, small ", "[small]" ) {

      http_request_header       header;

      SECTION("Результат парсинга, small") {
        bool result;
        std::string::const_iterator at;
        std::tie(result, at) = parse_http_header (small_header::text, header);

        REQUIRE(result == true);
        CHECK(at == small_header::text.end());

        CHECK(header.method == small_header::method);
        CHECK(header.uri == small_header::uri);
        CHECK(header.http_version == small_header::version);

        REQUIRE(header.header_fields.size() == small_header::key_value.size());

        for (auto pair: header.header_fields)
        {
          key_value_type::const_iterator it = small_header::key_value.find(pair.first);
          if (it != small_header::key_value.end())
            REQUIRE(pair.second == it->second);
          else
            FAIL("Header field \"" << pair.first << "\" not exists!");
        }
      }

    }

    TEST_CASE("Тестим парсер заголовков, bad ", "[bad]" ) {

      http_request_header       header;

      SECTION("Результат парсинга, bad") {
        bool result;
        std::string::const_iterator at;
        std::tie(result, at) = parse_http_header (bad_header::text, header);

        REQUIRE(result == false);
        CHECK(at == bad_header::text.begin());
      }

    }

    TEST_CASE("Тестим парсер заголовков, extra ", "[extra]" ) {

      http_request_header       header;

      SECTION("Результат парсинга, extra") {
        bool result;
        std::string::const_iterator at;
        std::tie(result, at) = parse_http_header (extra_header::text, header);

        REQUIRE(result == true);
        CHECK(at == extra_header::at);
        CHECK(header.method == extra_header::method);
        CHECK(header.uri == extra_header::uri);
        CHECK(header.http_version == extra_header::version);
        REQUIRE(header.header_fields.size() == extra_header::key_value.size());

        for (auto pair: header.header_fields)
        {
          key_value_type::const_iterator it = extra_header::key_value.find(pair.first);
          if (it != extra_header::key_value.end())
            REQUIRE(pair.second == it->second);
          else
            FAIL("Header field \"" << pair.first << "\" not exists!");
        }
      }
    }

    TEST_CASE("Тестим парсер заголовков, extra 1", "[extra 1]" ) {

      http_request_header       header;

      SECTION("Результат парсинга, extra 1") {
        bool result;
        std::string::const_iterator at;
        std::tie(result, at) = parse_http_header (extra_header_1::text, header);

        REQUIRE(result == true);
        CHECK(at == extra_header_1::at);
        CHECK(header.method == extra_header_1::method);
        CHECK(header.uri == extra_header_1::uri);
        CHECK(header.http_version == extra_header_1::version);
        REQUIRE(header.header_fields.size() == extra_header_1::key_value.size());

        for (auto pair: header.header_fields)
        {
          key_value_type::const_iterator it = extra_header_1::key_value.find(pair.first);
          if (it != extra_header_1::key_value.end())
            REQUIRE(pair.second == it->second);
          else
            FAIL("Header field \"" << pair.first << "\" not exists!");
        }
      }
    }

  } // namespace <http>
} // namespace <server>
