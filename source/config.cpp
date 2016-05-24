#include <iostream>
#include <boost/system/error_code.hpp>
#include <boost/asio.hpp>

#include "config.hpp"

namespace server
{
  namespace config
  {
    namespace asio    = boost::asio;
    namespace system  = boost::system;
    namespace po      = boost::program_options;
    namespace 

    string_array collect(int ac, char* av[])
    {
      string_array args;
      try
      {
        po::options_description empty;
        po::parsed_options parsed = po::command_line_parser(ac, av)
          .allow_unregistered()
          .options(empty)
          .run();
        args = po::collect_unrecognized(
          parsed.options,
          po::include_positional
        );
      }
      catch(po::error const& ex)
      { // rethrow error...
        util::throw_error<config::error>(ex.what());
      }
      return args;
    }

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
