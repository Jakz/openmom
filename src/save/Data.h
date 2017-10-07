#pragma once

#include "common/Common.h"

#include <string>
#include <map>
#include <unordered_map>
#include <list>

class UnitSpec;
class RaceUnitSpec;
class Race;
class Building;
class Skill;
class Spell;
class Level;

using experience_levels = std::vector<std::unique_ptr<const Level>>;

class Data
{
public:
  using key_type = std::string;
  template<typename T> using map_t = std::unordered_map<key_type, T>;
  
  using unit_dependency_map_t = std::unordered_map<const UnitSpec*, const Building*>;
  
  using skill_replacement_map_t = std::unordered_map<const Skill*, const Skill*>;
  
  static experience_levels normalUnitLevels, heroLevels;
  
private:
  template<typename T> static std::unordered_map<key_type, T>& containerFor();
  
  static unit_dependency_map_t unitDependsOnBuilding;
  
  static skill_replacement_map_t skillReplacementMap;
  
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
    {
      LOGD("Data value not found for %s", ident.c_str());
      assert(false);
    }
  }
  
  static const experience_levels& experienceLevelsForUnits() { return normalUnitLevels; }
  static const experience_levels& experienceLevelsForHeroes() { return heroLevels; }
  
  static const Skill* skill(const key_type& ident) { return get<const Skill*>(ident); }
  static const Building* building(const key_type& ident) { return get<const Building*>(ident); }
  static const UnitSpec* unit(const key_type& ident) { return get<const UnitSpec*>(ident); }
  static const Spell* spell(const key_type& ident) { return get<const Spell*>(ident); }
  static const Race* race(const key_type& ident) { return get<const Race*>(ident); }
  
  static const std::unordered_map<key_type, const UnitSpec*> units();
  
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
