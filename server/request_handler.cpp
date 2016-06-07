#include <fstream>
#include <iostream>

#include <boost/system/error_code.hpp>
#include <boost/algorithm/string.hpp>

#include "http/parser.hpp"
#include "http/response.hpp"
#include "request_handler.hpp"

namespace server {
  namespace http {

    file_pointer
    request_handler::handle(
      request& req,
      response& resp,
      system::error_code& ecode
    )
    {
      std::string path;
      if (!url::decode(req.uri, path))
      {
        resp = response::default_response(bad_request);
        return nullptr;
      }

      if (req.method != "GET")
      {
        resp = response::default_response (not_implemented);
        return nullptr;
      }

      if (path.empty()    ||
          path[0] != '/'  ||
          path.find ("..") != std::string::npos)
      {
        resp = response::default_response (bad_request);
        return nullptr;
      }

      if (path[path.size() - 1] == '/')
      {
        resp = response::default_response (not_implemented);
      }

      std::string full_path = static_root_ + path;
      file_pointer file(
        std::make_shared<std::ifstream>(
          full_path.c_str (),
          std::ios::binary | std::ios::ate
        )
      );

      if (!file->is_open ())
      {
        ecode.assign(errno, boost::system::system_category ());

        if (ecode.value () == system::errc::too_many_files_open)
          resp = response::default_response (unavailable);
        else if (ecode.value () == system::errc::no_such_file_or_directory)
          resp = response::default_response (not_found);
        else
          resp = response::default_response (not_implemented);

        return nullptr;
      }

      resp.status = status_string[ok].first;
      resp.headers["Accept-Ranges"]   = "bytes";
      resp.headers["Content-Type"]    = "application/octet-stream";
      resp.headers["Content-Length"]  = std::to_string (file->tellg ());
      resp.headers["Connection"]      = "close";



      return file;
    }
  }
}
