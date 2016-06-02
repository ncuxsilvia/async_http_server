#ifdef _DEBUG
#define BOOST_SPIRIT_DEBUG
#endif

#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <map>
#include <iostream>
#include <string>
#include <tuple>

#include "http_header_parser.hpp"

BOOST_FUSION_ADAPT_STRUCT(server::http::http_url_type, host, port, path, query_map)

BOOST_FUSION_ADAPT_STRUCT(server::http::http_request_header, method, uri, http_version, header_fields)

namespace server {
  namespace http {
    namespace qi = boost::spirit::qi;

    template <typename iterator>
    struct http_url_grammar: qi::grammar <iterator, http_url_type()> {
      http_url_grammar() : http_url_grammar::base_type(uri, "http url grammar")
      {
        host   = +qi::char_("0-9a-zA-Z-.");
        port   = +qi::digit;
        path   = +~qi::char_('?');
        query_key     = +qi::char_("0-9a-zA-Z-");
        query_value   = +~qi::char_('&');
        queries = *(query_key >> -qi::omit['='] >> -query_value >> -qi::char_('&'));

        uri = -qi::lexeme["http://"] >> -host >> -qi::omit[':'] >> -port >> -path >> -qi::omit['?'] >> -queries;
#ifdef _DEBUG
        BOOST_SPIRIT_DEBUG_NODES((host)(port)(path)(query_key)(query_value)(queries)(uri))
#endif
      }

      private:
        typedef qi::rule<iterator, std::string()>      rule_type;
        typedef qi::rule<iterator, map_type()>         query_rule_type;
        typedef qi::rule<iterator, http_url_type()>    main_rule_type;

        query_rule_type         queries;
        main_rule_type          uri;
        // qi::lexemes
        rule_type   host;
        rule_type   port;
        rule_type   path;
        rule_type   query_key;
        rule_type   query_value;
    };


    template <typename iterator, typename skipper = qi::ascii::blank_type>
    struct http_header_grammar: qi::grammar <iterator, http_request_header(), skipper> {
      http_header_grammar() : http_header_grammar::base_type(http_header, "HttpHeaderGrammar Grammar")
      {
        method        = +qi::alpha;
        uri           = +qi::graph;
        http_ver      = "HTTP/" >> +qi::char_("0-9.");

        field_key     = +qi::char_("0-9a-zA-Z-");
        field_value   = +~qi::char_("\r\n");

        fields = *(field_key >> ':' >> field_value >> qi::lexeme["\r\n"]);

        http_header = method >> uri >> http_ver >> qi::lexeme["\r\n"] >> fields >> -qi::lexeme["\r\n"];
//#ifdef _DEBUG
//        BOOST_SPIRIT_DEBUG_NODES((method)(uri)(http_ver)(fields)(http_header))
//#endif
      }

      private:
        typedef qi::rule<iterator, std::string()> rule_type;

        qi::rule<iterator, map_type(), skipper> fields;
        qi::rule<iterator, http_request_header(), skipper> http_header;
        // lexemes
        rule_type method;
        rule_type uri;
        rule_type http_ver;
        rule_type field_key;
        rule_type field_value;
    };

    result_type parse_http_header(std::string& request, http_request_header& http_request)
    {
      http_header_grammar<iterator> grammar;
      iterator first = request.begin();
      iterator last = request.end();
      bool result = qi::phrase_parse(first, last, grammar, qi::ascii::blank, http_request);
      return std::make_tuple(result, first);
    }

    result_type parse_http_url (std::string& url, http_url_type &http_url)
    {
      typedef std::string::const_iterator iterator;
      http_url_grammar<iterator> url_grammar;
      iterator first = url.begin();
      iterator last = url.end();
      bool result = qi::parse(first, last, url_grammar, http_url) && first == last;
      return std::make_tuple(result, first);
    }

    namespace url {

      std::string char_to_hex(unsigned char c)
      {
        short i = c;
        std::stringstream s;
        s << "%" << std::setw(2) << std::setfill('0') << std::hex << i;
        return s.str();
      }

      unsigned char hex_to_char(const std::string &str) {
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
              (c >= 48    && c <= 57)  ||       // 0-9
              (c >= 65    && c <= 90)  ||       // A-Z
              (c >= 97    && c <= 122) ||       // a-z
              (c == 45)                ||       // hyphen
              (c == 95)                ||       // underscore
              (c == 126)                        // tilde
          ) {
            o << *it;
          } else {
            o << char_to_hex(c);
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
                o << hex_to_char(esc);
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

