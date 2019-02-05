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