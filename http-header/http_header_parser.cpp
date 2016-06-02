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

BOOST_FUSION_ADAPT_STRUCT(server::http::http_request_header_type, method, uri, http_version, header_fields)

namespace server {
  namespace http {
    namespace qi = boost::spirit::qi;
    typedef std::string::const_iterator iterator;

    template <typename iterator_type>
    struct http_url_grammar: qi::grammar <iterator_type, http_url_type()> {
      http_url_grammar() : http_url_grammar::base_type(uri, "http url grammar")
      {
        host   = +qi::char_("0-9a-zA-Z-.");
        port   = +qi::digit;
        path   = +~qi::char_('?');
        query_key     = +qi::char_("0-9a-zA-Z-");
        query_value   = +~qi::char_('&');
        queries = *(query_key >> -qi::omit['='] >> -query_value >> -qi::char_('&'));

        uri = -qi::lexeme["http://"] >> host >> -qi::omit[':'] >>-port >> -path >> -qi::omit['?'] >> -queries;
//#ifdef _DEBUG
//        BOOST_SPIRIT_DEBUG_NODES((host)(port)(path)(query_key)(query_value)(queries)(uri))
//#endif
      }

      private:
        typedef qi::rule<iterator_type, std::string()>                    rule_type;
        typedef qi::rule<iterator_type, http_url_type::query_map_type()>  query_rule_type;
        typedef qi::rule<iterator_type, http_url_type()>                  main_rule_type;

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
    struct http_header_grammar: qi::grammar <iterator, http_request_header_type(), skipper> {
      http_header_grammar() : http_header_grammar::base_type(http_header, "HttpHeaderGrammar Grammar")
      {
        method        = +qi::alpha;
        uri           = +qi::graph;
        http_ver      = "HTTP/" >> +qi::char_("0-9.");

        field_key     = +qi::char_("0-9a-zA-Z-");
        field_value   = +~qi::char_("\r\n");

        fields = *(field_key >> ':' >> field_value >> qi::lexeme["\r\n"]);

        http_header = method >> uri >> http_ver >> qi::lexeme["\r\n"] >> fields >> -qi::lexeme["\r\n"];
#ifdef _DEBUG
        BOOST_SPIRIT_DEBUG_NODES((method)(uri)(http_ver)(fields)(http_header))
#endif
      }

      private:
        typedef qi::rule<iterator, std::string()> rule_type;

        qi::rule<iterator, http_request_header_type::field_map_type(), skipper> fields;
        qi::rule<iterator, http_request_header_type(), skipper> http_header;
        // lexemes
        rule_type method;
        rule_type uri;
        rule_type http_ver;
        rule_type field_key;
        rule_type field_value;
    };

    parse_http_header(const std::string& str, http_request_header_type& http_request, iterator finish)
    {
      http_header_grammar<iterator> grammar;

      iterator first = str.begin();
      iterator last = str.end();



      bool result = qi::phrase_parse(first, last, grammar, qi::ascii::blank, http_request);

      return  result && first == last;
    }


    bool parse_http_url (const std::string &str, http_url_type &http_url)
    {
      typedef std::string::const_iterator iterator;
      http_url_grammar<iterator> url_grammar;

      iterator first = str.begin();
      iterator last = str.end();

      return qi::parse(first, last, url_grammar, http_url) && first == last;
    }
  } // namespace <server>
} // namespace <http>

