#pragma once

#include <string>

class Path;

#pragma mark Platform
class Platform
{
public:
  virtual bool exists(const Path& path) const = 0;
  virtual Path getResourcePath() const = 0;
  
  static Platform* instance();
};


