#include "platform/Path.h"
#include "platform/platform.h"

class PlatformWin : public Platform
{
public:
  Path getResourcePath() const override
  {
    return "D:/dev/openmom";
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