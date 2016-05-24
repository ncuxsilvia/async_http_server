#include <iostream>
#include <boost/system/error_code.hpp>
#include <boost/asio.hpp>

namespace server
{
  namespace config
  {
    namespace asio    = boost::asio;
    namespace system  = boost::system;
    namespace po      = boost::program_options;
    namespace 

    settings()
      : netaddr(0)
    {
      asio::io_service ios;

      system::error_code dummy;
      hostname = asio::ip::host_name(dummy);
      asio::ip::tcp::resolver::query query(
        asio::ip::tcp::v4(),
        hostname,
        ""
      )


    }
  }
}