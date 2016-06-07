#include <iostream>
#include <string>

#include "catch.hpp"
#include "http/parser.hpp"
#include "test_data.hpp"

namespace server {
  namespace test {

    using server::http::request;
    using namespace data;

    TEST_CASE("Тестим парсер заголовков, alternative ", "[alternative]" ) {

      request header;

      SECTION("Результат парсинга, alternative") {
        bool result;
        std::tie(result, std::ignore) = parse_http_header(alternative_header::text, header);

        REQUIRE(result == true);

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

      request header;

      SECTION("Результат парсинга, small") {
        bool result;
        std::tie(result, std::ignore) = parse_http_header (small_header::text, header);

        REQUIRE(result == true);
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

      request header;

      SECTION("Результат парсинга, bad") {
        bool result;
        std::string::const_iterator at;
        std::tie(result, at) = parse_http_header (bad_header::text, header);

        REQUIRE(result == false);
        CHECK(at == bad_header::text.begin());
      }

    }

    TEST_CASE("Тестим парсер заголовков, extra ", "[extra]" ) {

      request header;

      SECTION("Результат парсинга, extra") {
        bool result;
        std::tie(result, std::ignore) = parse_http_header (extra_header::text, header);

        REQUIRE(result == true);
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

      request header;

      SECTION("Результат парсинга, extra 1") {
        bool result;
        std::tie(result, std::ignore) = parse_http_header (extra_header_1::text, header);

        REQUIRE(result == false);
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

    TEST_CASE("Тестим парсер заголовков, error request", "error_request" ) {

      request header;

      SECTION("Результат парсинга, error request") {
        bool result;
        std::tie(result, std::ignore) = parse_http_header (error_request::text, header);

        REQUIRE_FALSE(result == true);
        CHECK_FALSE(header.method == error_request::method);
        CHECK(header.uri == error_request::uri);
        CHECK(header.http_version == error_request::version);
        REQUIRE(header.header_fields.size() == error_request::key_value.size());

        for (auto pair: header.header_fields)
        {
          key_value_type::const_iterator it = error_request::key_value.find(pair.first);
          if (it != error_request::key_value.end())
            REQUIRE(pair.second == it->second);
          else
            FAIL("Header field \"" << pair.first << "\" not exists!");
        }
      }
    }

  } // namespace <http>
} // namespace <server>
