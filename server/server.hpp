#ifndef SERVER_HPP
#define SERVER_HPP

#include "config.hpp"

namespace server
{
  class xserver
  {
  public:

    static
    int main(int ac, char* av[]);

    xserver(config::string_array const& args);


    // ...

  private:

    bool read_options();

    config::string_array    args_;
    config::settings        conf_;
  };
}

#endif // SERVER_HPP
