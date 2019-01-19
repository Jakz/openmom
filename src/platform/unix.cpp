#include "Platform.h"

#include <sys/stat.h>
#include <dirent.h>


#include "common/mystrings.h"
#include "Path.h"

class PlatformUnix : public Platform
{
public:
  Path getResourcePath() const override
  {
    return "";
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
  
  Path findFile(const Path& folder, const std::string& name) override
  {
    DIR *dir;
    struct dirent *ent;
    
    if ((dir = opendir (folder.c_str())) != NULL)
    {
      while ((ent = readdir (dir)) != NULL)
      {
        if (strings::compareInsensitive(name, ent->d_name))
          return folder + ent->d_name;
      }
      closedir (dir);
    } else {
      /* could not open directory */
    }
    
    return folder + name;
  }
};

Platform* Platform::instance()
{
  static PlatformUnix platform;
  return &platform;
}

