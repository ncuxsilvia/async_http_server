#include "response.hpp"

namespace server {
  namespace http {

    response response::default_response(const status_code code)
    {
      response resp;
      resp.status.assign(status_string[code].first);
      resp.content.assign (status_string[code].second);
      resp.headers["Content-Type"]    = "text/html";
      resp.headers["Content-Length"]  = std::to_string (resp.content.size ());
      return resp;
    }

    std::string response::merge()
    {
      std::string answer;
      answer += status;
      for (auto& h: headers)
        answer += h.first + special::pair_sep + h.second + special::crlf;
      answer += special::crlf;
      answer += content;
      return answer;
    }
  }
}
