#ifndef LOGGING_HPP
#define LOGGING_HPP



namespace server {
  namespace log {
    namespace trivial = boost::log::trivial;
    void init_log(std::string path, trivial::severity_level lvl);
  }

}

#endif
