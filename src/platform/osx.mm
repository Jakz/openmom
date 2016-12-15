#include "platform.h"

#include <Foundation/Foundation.h>

Platform* Platform::instance()
{
  static Platform platform;
  return &platform;
}

std::string Platform::getResourcePath()
{
  NSString* path = [[NSBundle mainBundle] resourcePath];
  return std::string([path UTF8String]);
}
