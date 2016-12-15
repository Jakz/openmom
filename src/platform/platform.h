#pragma once

#include <string>

class Platform
{
public:
  
  std::string getResourcePath();
  
  static Platform* instance();
};
