#include "server.hpp"
#include "config.hpp"

namespace server
{
  int xserver::main(int ac, char *av[])
  {
    try
    { //collect command line arguments
      config::string_array args(
        config::collect(ac, av)
        );
      // read active action code
      config::string_array runopts;
      config::action::type action_code = config::invoke_action(args, runopts);

      if (action_code == config::action::run)
      {
        std::cout << "run server ..." << std::endl;
          // create server class, and start them
      }
      return 0;
    }
    catch(std::exception const& ex) // add processing different exception (e.g.: config, server, log, etc..)
    {
      std::cout << ex.what () << std::endl;
    }
    return -1;
  }
}
