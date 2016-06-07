#define BOOST_LOG_DYN_LINK
#include "session_manager.hpp"

namespace server {
  namespace http {
    session_manager::session_manager()
    {}

    void session_manager::start(session::pointer s)
    {
      BOOST_LOG_TRIVIAL(trace) << "session manager: connection start";
      std::lock_guard<std::mutex> lock(mtx_);
      sessions_.insert(s);
      s->start ();
    }

    void session_manager::stop(session::pointer s)
    {
      std::lock_guard<std::mutex> lock(mtx_);
      sessions_.erase(s);
      s->stop ();
    }

    void session_manager::stop_all()
    {
      std::lock_guard<std::mutex> lock(mtx_);
      for (auto s: sessions_)
        s->stop();
      sessions_.clear();
    }
  } // namespace <http>
} // namespace <server>
