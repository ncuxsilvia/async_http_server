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

    xserver(config::string_array const& args) : args_(args) {};


    // ...

  private:
    int init();

    bool read_options();
    void apply_options();

    config::string_array    args_;
    config::settings        conf_;
  };
}

#endif // SERVER_HPP
