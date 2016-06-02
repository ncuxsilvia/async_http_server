#include <string>
#include <map>

namespace server {
  namespace test {
    namespace data {

      typedef std::map<std::string, std::string> key_value_type;

      namespace complete_header {
        std::string  scope_name = "complete header";

        std::string  text = 
          "GET http://net.tutsplus.com:8080/path/to/file?query1=1&query2=2 HTTP/1.1\r\n"        
          "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.5) Gecko/20091102 Firefox/3.5.5 (.NET CLR 3.5.30729)\r\n"
          "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
          "Accept-Language: en-us,en;q=0.5\r\n"                                                     
          "Accept-Encoding: gzip,deflate\r\n"                                                                                             
          "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n"
          "Keep-Alive: 300\r\n"
          "Connection: keep-alive\r\n"
          "Cookie: PHPSESSID=r2t5uvjq435r4q7ib3vtdjq120\r\n"
          "Pragma: no-cache\r\n"
          "Cache-Control: no-cache\r\n";

        std::string method("GET");
        std::string uri("http://net.tutsplus.com:8080/path/to/file?query1=1&query2=2");
        std::string version("1.1");
        std::map<std::string, std::string> key_value 
          {
            { "User-Agent"      , "Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.5) Gecko/20091102 Firefox/3.5.5 (.NET CLR 3.5.30729)" },
            { "Accept"          , "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8" },
            { "Accept-Language" , "en-us,en;q=0.5" },
            { "Accept-Encoding" , "gzip,deflate" },
            { "Accept-Charset"  , "ISO-8859-1,utf-8;q=0.7,*;q=0.7" },
            { "Keep-Alive"      , "300" },
            { "Connection"      , "keep-alive" },
            { "Cookie"          , "PHPSESSID=r2t5uvjq435r4q7ib3vtdjq120" },
            { "Pragma"          , "no-cache" },
            { "Cache-Control"   , "no-cache" },
          };

      }

      namespace alternative_header {
        std::string  scope_name = "alternative header";

        std::string  text = 
          "GET /path/to/file?query1=1&query2=2 HTTP/1.1\r\n"        
          "Host: net.tutsplus.com\r\n"
          "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.5) Gecko/20091102 Firefox/3.5.5 (.NET CLR 3.5.30729)\r\n"
          "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
          "Accept-Language: en-us,en;q=0.5\r\n"                                                     
          "Accept-Encoding: gzip,deflate\r\n"                                                                                             
          "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n"
          "Keep-Alive: 300\r\n"
          "Connection: keep-alive\r\n"
          "Cookie: PHPSESSID=r2t5uvjq435r4q7ib3vtdjq120\r\n"
          "Pragma: no-cache\r\n"
          "Cache-Control: no-cache\r\n";

        std::string method("GET");
        std::string uri("/path/to/file?query1=1&query2=2");
        std::string version("1.1");
        std::map<std::string, std::string> key_value 
          {
            { "Host"            , "net.tutsplus.com"},
            { "User-Agent"      , "Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.5) Gecko/20091102 Firefox/3.5.5 (.NET CLR 3.5.30729)" },
            { "Accept"          , "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8" },
            { "Accept-Language" , "en-us,en;q=0.5" },
            { "Accept-Encoding" , "gzip,deflate" },
            { "Accept-Charset"  , "ISO-8859-1,utf-8;q=0.7,*;q=0.7" },
            { "Keep-Alive"      , "300" },
            { "Connection"      , "keep-alive" },
            { "Cookie"          , "PHPSESSID=r2t5uvjq435r4q7ib3vtdjq120" },
            { "Pragma"          , "no-cache" },
            { "Cache-Control"   , "no-cache" },
          };
      }

      namespace small_header {
        std::string  scope_name = "small header";

        std::string  text = 
          "GET yandex.ru HTTP/1.1\r\n";

        std::string method("GET");
        std::string uri("yandex.ru");
        std::string version("1.1");
        std::map<std::string, std::string> key_value;
      }

      namespace bad_header {
        std::string  scope_name = "alternative header";

        std::string  text = 
          "GET /path/to/file?query1=1&query2=2 HTTP/1.1"        
          "Host: net.tutsplus.com\r\n"
          "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.5) Gecko/20091102 Firefox/3.5.5 (.NET CLR 3.5.30729)\r\n"
          "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
          "Accept-Language: en-us,en;q=0.5\r\n"                                                     
          "Accept-Encoding: gzip,deflate\r\n"                                                                                             
          "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n"
          "Keep-Alive: 300\r\n"
          "Connection: keep-alive\r\n"
          "Cookie: PHPSESSID=r2t5uvjq435r4q7ib3vtdjq120\r\n"
          "Pragma: no-cache\r\n"
          "Cache-Control: no-cache\r\n";

        std::string method("GET");
        std::string uri("/path/to/file?query1=1&query2=2");
        std::string version("1.1");
        std::map<std::string, std::string> key_value 
          {
            { "Host"            , "net.tutsplus.com"},
            { "User-Agent"      , "Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.5) Gecko/20091102 Firefox/3.5.5 (.NET CLR 3.5.30729)" },
            { "Accept"          , "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8" },
            { "Accept-Language" , "en-us,en;q=0.5" },
            { "Accept-Encoding" , "gzip,deflate" },
            { "Accept-Charset"  , "ISO-8859-1,utf-8;q=0.7,*;q=0.7" },
            { "Keep-Alive"      , "300" },
            { "Connection"      , "keep-alive" },
            { "Cookie"          , "PHPSESSID=r2t5uvjq435r4q7ib3vtdjq120" },
            { "Pragma"          , "no-cache" },
            { "Cache-Control"   , "no-cache" },
          };
      }

      namespace extra_header {
        std::string  scope_name = "alternative header";

        std::string  text =
          "GET /path/to/file?query1=1&query2=2 HTTP/1.1\r\n"
          "Host: net.tutsplus.com\r\n"
          "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.5) Gecko/20091102 Firefox/3.5.5 (.NET CLR 3.5.30729)\r\n"
          "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
          "Accept-Language: en-us,en;q=0.5\r\n"
          "Accept-Encoding: gzip,deflate\r\n"
          "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n"
          "Keep-Alive: 300\r\n"
          "Connection: keep-alive\r\n"
          "Cookie: PHPSESSID=r2t5uvjq435r4q7ib3vtdjq120\r\n"
          "Pragma: no-cache\r\n"
          "Cache-Control: no-cache\r\n\r\n"
           "abcdefghijklmnopqrtsuvwxyz";

        std::string method("GET");
        std::string uri("/path/to/file?query1=1&query2=2");
        std::string version("1.1");
        std::map<std::string, std::string> key_value
          {
            { "Host"            , "net.tutsplus.com"},
            { "User-Agent"      , "Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.5) Gecko/20091102 Firefox/3.5.5 (.NET CLR 3.5.30729)" },
            { "Accept"          , "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8" },
            { "Accept-Language" , "en-us,en;q=0.5" },
            { "Accept-Encoding" , "gzip,deflate" },
            { "Accept-Charset"  , "ISO-8859-1,utf-8;q=0.7,*;q=0.7" },
            { "Keep-Alive"      , "300" },
            { "Connection"      , "keep-alive" },
            { "Cookie"          , "PHPSESSID=r2t5uvjq435r4q7ib3vtdjq120" },
            { "Pragma"          , "no-cache" },
            { "Cache-Control"   , "no-cache" },
          };

        std::string::size_type fail_pos = text.find("abcdefghijklmnopqrtsuvwxyz");
        std::string::const_iterator at = text.begin() + fail_pos;
      }

      namespace extra_header_1{
        std::string  scope_name = "alternative header";

        std::string  text =
          "GET /path/to/file?query1=1&query2=2 HTTP/1.1\r\n"
          "Host: net.tutsplus.com\r\n"
          "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.5) Gecko/20091102 Firefox/3.5.5 (.NET CLR 3.5.30729)\r\n"
          "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
          "Accept-Language: en-us,en;q=0.5\r\n"
          "Accept-Encoding: gzip,deflate\r\n"
          "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n"
          "Keep-Alive: 300\r\n"
          "Connection: keep-alive\r\n"
          "Cookie: PHPSESSID=r2t5uvjq435r4q7ib3vtdjq120\r\n"
          "Pragma: no-cache\r\n"
          "Cache-Control: no-cache\r\n"
          "GET /path/to/file?query1=1&query2=2 HTTP/1.1\r\n\r\n";

        std::string method("GET");
        std::string uri("/path/to/file?query1=1&query2=2");
        std::string version("1.1");
        std::map<std::string, std::string> key_value
          {
            { "Host"            , "net.tutsplus.com"},
            { "User-Agent"      , "Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.5) Gecko/20091102 Firefox/3.5.5 (.NET CLR 3.5.30729)" },
            { "Accept"          , "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8" },
            { "Accept-Language" , "en-us,en;q=0.5" },
            { "Accept-Encoding" , "gzip,deflate" },
            { "Accept-Charset"  , "ISO-8859-1,utf-8;q=0.7,*;q=0.7" },
            { "Keep-Alive"      , "300" },
            { "Connection"      , "keep-alive" },
            { "Cookie"          , "PHPSESSID=r2t5uvjq435r4q7ib3vtdjq120" },
            { "Pragma"          , "no-cache" },
            { "Cache-Control"   , "no-cache" },
          };

        std::string::size_type fail_pos = text.find("GET /path/to/file?query1=1&query2=2 HTTP/1.1\r\n\r\n");
        std::string::const_iterator at = text.begin() + fail_pos;
      }

    } // namespace <data>
  } // namespace <test>
} // namespace <server>
