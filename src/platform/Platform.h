#pragma once

#include <string>

class Path;

class FileSystem
{
public:
  virtual bool exists(const Path& path) const = 0;
  virtual Path absolute(const Path& path) const = 0;
  virtual Path findFile(const Path& folder, const std::string& name) const = 0;
};

#pragma mark Platform
class Platform : public FileSystem
{
public:  
  virtual Path getResourcePath() const = 0;
  
  static Platform* instance();
};


