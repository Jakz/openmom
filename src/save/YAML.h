#pragma once

#include "Common.h"
#include <string>

namespace YAML { class Node; }

class yaml
{
public:
  class Node;
  using N = yaml::Node;
  
private:
  static N parse(const std::string& fileName);
  static std::string yamlPath(const std::string& fileName);
  
  template<typename T> static void parse(const N& node, std::vector<T>& dest);
  template<typename T> static T optionalParse(const N& node, const char* key, T def);
  template<typename T> static void parse(const N& node, T& value);
  
  template<typename T, typename std::enable_if<!std::is_pointer<T>::value, int>::type = 0> static T parse(const N& node);
  template<typename T, typename std::enable_if<std::is_pointer<T>::value, int>::type = 0> static T parse(const N& node);
  
  static const std::string& getIdentifier(const N& node, const char* key = "identifier");
  
public:
  static void parse();
  static void parseLocalization();
  static void parseSkills();
  static void parseSpells();
  static void parseUnits();
  static void parseRaces();
  static void parseLevels();
};
