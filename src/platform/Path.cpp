#include "Path.h"

#include "Platform.h"

#include <algorithm>

static constexpr const char SEPARATOR = '/';
static constexpr const char WIN_SEPARATOR = '\\';

bool endsWith(const std::string& str, char c)
{
  return str.back() == c;
}

bool startsWith(const std::string& str, char c)
{
  return str.front() == c;
}

Path Path::convertSeparatorToWindows() const
{
  std::string ndata = data;
  std::replace(ndata.begin(), ndata.end(), SEPARATOR, WIN_SEPARATOR);
  return Path(ndata);
}

Path Path::append(const Path& path) const
{
  if (data.empty())
    return path;
  else if (!endsWith(data,SEPARATOR) && !startsWith(path.data, SEPARATOR))
    return Path(data + SEPARATOR + path.data);
  else if (endsWith(data, SEPARATOR) && startsWith(path.data, SEPARATOR))
    return Path(data + path.data.substr(1));
  else
    return Path(data + path.data);
}

Path Path::appendExt(const std::string &ext) const
{
  if (ext.empty()) return Path(*this);
  else if (ext[0] == '.') return Path(data + ext);
  else return Path(data + "." + ext);
}

bool Path::exists() const
{
  return Platform::instance()->exists(*this);
}
