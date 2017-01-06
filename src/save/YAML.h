#pragma once

#include "Common.h"
#include <string>

namespace YAML { class Node; }

namespace yaml
{
  using N = YAML::Node;
  
  N parse(const std::string& fileName);
  std::string yamlPath(const std::string& fileName);
  
  template<typename T> void parse(const N& node, std::vector<T>& dest);
  template<typename T> T optionalParse(const N& node, T def);
  template<typename T> void parse(const N& node, T& value);
  
  template<typename T, typename std::enable_if<!std::is_pointer<T>::value, int>::type = 0> T parse(const N& node);
  template<typename T, typename std::enable_if<std::is_pointer<T>::value, int>::type = 0> T parse(const N& node);
  
  void parse();
  void parseLocalization();
  void parseSkills();
};
