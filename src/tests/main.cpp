#define CATCH_CONFIG_RUNNER
#define SDL_MAIN_HANDLED


#include "tests/catch.hpp"

#include "save/Data.h"
#include "save/YAML.h"

#include "mystrings.h"

#include <iostream>

#include "platform/Platform.h"
#include "yaml-cpp/node/detail/node_data.h"
std::string YAML::detail::node_data::empty_scalar;

class PlatformWin : public Platform
{
public:
  Path getResourcePath() const override
  {
    return "C:\\Users\\Jack\\Documents\\dev\\openmom";
  }

  bool exists(const Path& path) const override
  {
    struct stat buffer;
    return stat(path.c_str(), &buffer) == 0;
  }

  Path absolute(const Path& path) const override
  {
    // TODO
    return path;
  }
};

Platform* Platform::instance()
{
  static PlatformWin platform;
  return &platform;
}

int main( int argc, char* argv[] )
{
  yaml::parse();
  int result = Catch::Session().run( argc, argv );

#ifdef _WIN32
  std::getchar();
#endif
  return result < 0xff ? result : 0xff;
}