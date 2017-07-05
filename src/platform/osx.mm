#include "platform.h"

#include <sys/stat.h>

#include <Foundation/Foundation.h>

class PlatformOSX : public Platform
{
public:
  std::string getResourcePath() const override
  {
    NSString* path = [[NSBundle mainBundle] resourcePath];
    return std::string([path UTF8String]);
  }
  
  bool exists(const path& path) const override
  {
    struct stat buffer;
    return stat(path.c_str(), &buffer) == 0;
  }
};

Platform* Platform::instance()
{
  static PlatformOSX platform;
  return &platform;
}

