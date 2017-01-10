#pragma once

#include "Common.h"

#include <map>
#include <unordered_map>
#include <list>

class UnitSpec;
class RaceUnitSpec;
class Race;
class Building;

class Data
{
public:
  using key_type = std::string;
  template<typename T> using map_t = std::unordered_map<key_type, T>;
  
  using unit_dependency_map_t = std::unordered_map<const UnitSpec*, const Building*>;
  
private:
  template<typename T> static std::unordered_map<key_type, T>& containerFor();
  
  static unit_dependency_map_t unitDependsOnBuilding;
  
public:
  static const Trait& trait(const TraitID trait);
  static const Wizard& wizard(const WizardID wizard);
  
  template<typename T> static bool registerData(const key_type& ident, const T data)
  {
    auto& container = containerFor<T>();
    if (container.find(ident) != container.end())
      return false;
    
    container.insert(std::make_pair(ident, data));
    return true;
  }
  
  template<typename T> static const T get(const key_type& ident)
  {
    const auto& map = containerFor<T>();
    const auto it = map.find(ident);
    if (it != map.end())
      return it->second;
    else
      assert(false);
  }
  
  static const Building* building(const key_type& ident) { return get<const Building*>(ident); }
  static const UnitSpec* unit(const key_type& ident) { return get<const UnitSpec*>(ident); }
  static const Race* race(const key_type& ident) { return get<const Race*>(ident); }
  
  static std::vector<const RaceUnitSpec*> unitsForRace(const Race* race);
  
  static std::pair<unit_dependency_map_t::const_iterator, unit_dependency_map_t::const_iterator> requiredBuildingsForUnit(const UnitSpec* unit)
  {
    return unitDependsOnBuilding.equal_range(unit);
  }
  
#if defined(DEBUG)
  template<typename T> static void getInfo()
  {
    auto& container = containerFor<T>();
    LOGD("[data] size: %zu", container.size())
    for (const auto& entry : container)
      LOGD("  %s -> %p", entry.first.c_str(), entry.second);
  }
#endif
  
  friend class yaml;
};
