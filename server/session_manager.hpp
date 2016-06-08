#ifndef SESSION_MANAGER_HPP
#define SESSION_MANAGER_HPP

#include <set>
#include <mutex>
#include "session.hpp"

namespace server {
  namespace http {
    namespace asio = boost::asio;

    class session_manager
    {
    public:
      session_manager(const session_manager&) = delete;
      session_manager& operator=(const session_manager&) = delete;

      session_manager();

      void start(session::pointer s);
      void stop(session::pointer s);
      void stop_all();

    private:
      std::set<session::pointer> sessions_;
      std::mutex                 mtx_;
    };
  } // namespace <http>
} // namespace <server>


#endif
