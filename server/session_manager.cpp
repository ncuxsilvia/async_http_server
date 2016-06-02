#include "session_manager.hpp"

namespace server {
  namespace http {
    session_manager::session_manager()
    {

    }

    void session_manager::start(session::pointer s)
    {
      sessions_.insert(s);
      s->start ();
    }

    void session_manager::stop(session::pointer s)
    {
      sessions_.erase(s);
      s->stop ();
    }

    void session_manager::stop_all()
    {
      for (auto s: sessions_)
        s->stop();
      sessions_.clear();
    }
  } // namespace <http>
} // namespace <server>
