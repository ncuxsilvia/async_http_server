#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <boost/log/trivial.hpp>
#include <boost/log/attributes/named_scope.hpp>

namespace server {
  namespace log {

    namespace trivial = boost::log::trivial;
    void init_log(std::string path, trivial::severity_level lvl);

  } // namespace <log>
} // namespace <server>

#endif
