#include "Platform.h"

#include <sys/stat.h>

#include <Foundation/Foundation.h>

#include "Path.h"

class PlatformOSX : public Platform
{
public:
  Path getResourcePath() const override
  {
#ifdef CMAKE_BUILD
    return "../..";
#else
    NSString* nspath = [[NSBundle mainBundle] resourcePath];
    return Path([nspath UTF8String]);
#endif
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
  static PlatformOSX platform;
  return &platform;
}

