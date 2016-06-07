#include <map>
#include <iostream>
#include <string>
#include <tuple>

#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/std_pair.hpp>

#include "parser.hpp"

BOOST_FUSION_ADAPT_STRUCT(server::http::request, method, uri, http_version, header_fields)

namespace server {
  namespace http {
    namespace qi = boost::spirit::qi;

    template <typename iterator, typename skipper = qi::ascii::blank_type>
    struct http_header_grammar: qi::grammar <iterator, request(), skipper>
    {
      http_header_grammar() : http_header_grammar::base_type(http_request, "HttpHeaderGrammar Grammar")
      {
        method        = +qi::alpha;
        uri           = +qi::graph;
        http_ver      = "HTTP/" >> +qi::char_("0-9.");

        field_key     = +qi::char_("0-9a-zA-Z-");
        field_value   = +~qi::char_("\r\n");

        fields = *(field_key >> ':' >> field_value >> qi::lexeme["\r\n"]);

        http_request = method >> uri >> http_ver >> qi::lexeme["\r\n"] >> fields >> qi::lexeme["\r\n"];
      }

      private:
        typedef qi::rule<iterator, std::string()> rule_type;

        qi::rule<iterator, map_type(), skipper> fields;
        qi::rule<iterator, request(), skipper> http_request;
        // lexemes
        rule_type method;
        rule_type uri;
        rule_type http_ver;
        rule_type field_key;
        rule_type field_value;
    };

    result_type parse_http_header(std::string& raw, request& http_request)
    {
      http_header_grammar<iterator> grammar;
      iterator first = raw.begin();
      iterator last = raw.end();
      bool result = qi::phrase_parse(first, last, grammar, qi::ascii::blank, http_request);
      return std::make_tuple(result, first);
    }

    namespace url {

      bool is_alpha(char c)
      {
        if (
          (c >= 65    && c <= 90)  || // A-Z
          (c >= 97    && c <= 122)    // a-z
        )
          return true;
        return false;
      }

      bool is_digit(unsigned char c)
      {
        if (c >= 48 && c <= 57)       // 0-9
          return true;
        return false;
      }

      bool is_special(unsigned char c)
      {
        // '-' , '_' , '~'
        if ((c == 45) || (c == 95) || (c == 126))
          return true;
        return false;
      }

      std::string to_hex(unsigned char c)
      {
        short i = c;
        std::stringstream s;
        s << "%" << std::setw(2) << std::setfill('0') << std::hex << i;
        return s.str();
      }

      unsigned char to_char(const std::string &str) {
          short c = 0;

          if(!str.empty()) {
            std::istringstream in(str);
            in >> std::hex >> c;
            if(in.fail()) {
                throw std::runtime_error("stream decode failure");
            }
          }
        return static_cast<unsigned char>(c);
      }

      bool encode(const std::string &url, std::string& out)
      {
        bool result = false;
        std::ostringstream o;
        std::string::const_iterator it = url.begin();

        for( ; it != url.end(); ++it) {
          unsigned char c = static_cast<unsigned char>(*it);
          if(
              is_alpha(c) ||
              is_digit(c) ||
              is_special(c)
          ) {
            o << *it;
          } else {
            o << to_hex(c);
          }
        }

        out.assign(o.str());
        result = true;
        return result;
      }

      bool decode(const std::string &url, std::string& out)
      {
        std::ostringstream o;
        bool result = true;
        try {
          for(std::string::size_type i=0; i < url.length(); ++i) {
              if(url.at(i) == '%')
              {
                std::string esc(url.substr(i+1, 2));
                o << to_char(esc);
                i += 2;
              } else {
                o << url.at(i);
              }
          }
          out.assign(o.str());
        }
        catch(...)
        {
          result = false;
        }
        return result;
      }
    }
  } // namespace <http>
} // namespace <server>

