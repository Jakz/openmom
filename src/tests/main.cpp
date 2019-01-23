#define CATCH_CONFIG_RUNNER
#define SDL_MAIN_HANDLED

#include <iostream>
#include <sys/stat.h>

#include "tests/catch.hpp"
#include "save/YAML.h"

#include "platform/Platform.h"
#include "platform/Path.h"

#ifdef _WIN32
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

  Path findFile(const Path& folder, const std::string& name) const override
  {
    return folder + name;
  }
};

Platform* Platform::instance()
{
  static PlatformWin platform;
  return &platform;
}
#endif

int main( int argc, char* argv[] )
{
  yaml::parse();
  int result = Catch::Session().run( argc, argv );

#ifdef _WIN32
  std::getchar();
#endif
  return result < 0xff ? result : 0xff;
}
