#pragma once

#include "common/Common.h"

#include <string>
#include <map>
#include <unordered_map>
#include <list>
#include <memory>

#if defined(DEBUG)
#include <sstream>
#endif

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
  
  template<typename T>
  struct insertion_ordered_map
  {
  private:
    using inner_type = std::unordered_map<key_type, T>;
    inner_type mapping;
    std::vector<std::reference_wrapper<const key_type>> order;
    
  public:
    using value_type = typename inner_type::value_type;
    using map_iterator = typename inner_type::const_iterator;
    using ordered_iterator = typename decltype(order)::const_iterator;
    
    class const_iterator
    {
    public:
      using inner_it = typename std::vector<std::reference_wrapper<const key_type>>::const_iterator;      

    private:
      const insertion_ordered_map<T>& data;
      inner_it it;
      
    public:
      const_iterator(const insertion_ordered_map<T>& data, inner_it it) : data(data), it(it) { }
      
      const_iterator& operator+=(size_t i) { it += i; return *this; }
      
      inline bool operator!=(const const_iterator& other) const { return it != other.it; }
      inline bool operator==(const const_iterator& other) const { return it == other.it; }

      inline const const_iterator& operator++() { ++it; return *this; }
      T operator*() const { return data.find((*it).get())->second; }

      using value_type = T;
      using difference_type = typename map_iterator::difference_type;
      using pointer = const T* ;
      using reference = const T& ;
      using iterator_category = typename map_iterator::iterator_category;
    };
    
    insertion_ordered_map() { }
    insertion_ordered_map(const std::initializer_list<value_type>& values)
    {
      for (const auto& pair : values)
      {
        auto it = mapping.insert(pair);
        order.push_back(std::cref(it.first->first));
      }
    }

    class value_iterator
    {
    private:
      map_iterator it;

    public:
      value_iterator(map_iterator it) : it(it) { }

      inline value_iterator& operator++() { ++it; return *this; }

      inline bool operator!=(const value_iterator& other) const { return it != other.it; }
      inline bool operator==(const value_iterator& other) const { return it == other.it; }
      inline T operator*() const { return it->second; }

      using difference_type = typename map_iterator::difference_type;
      using value_type = T;
      using pointer = const T*;
      using reference = const T&;
      using iterator_category = typename map_iterator::iterator_category;
    };
    
    insertion_ordered_map(const insertion_ordered_map& other) = delete;
    insertion_ordered_map operator=(const insertion_ordered_map& other) = delete;
    
    size_t size() const { return mapping.size(); }

    std::pair<map_iterator, bool> insert(value_type&& pair)
    {
      auto it = mapping.insert(pair);
      order.push_back(std::cref(it.first->first));
      return it;
    }
    map_iterator find(const typename inner_type::key_type& key) const { return mapping.find(key); }
    map_iterator begin() const { return mapping.begin(); }
    map_iterator end() const { return mapping.end(); }
    
    const_iterator obegin() const { return const_iterator(*this, order.begin()); }
    const_iterator oend() const { return const_iterator(*this, order.end()); }
    const T& operator[](size_t i) const { return mapping.find(order[i].get())->second; }
    
  };
  
  
  template<typename T> using map_t = insertion_ordered_map<T>; //std::unordered_map<key_type, T>;

  using building_dependency_map_t = std::unordered_multimap<const Building*, const Building*>;
  using building_replacement_map_t = std::unordered_map<const Building*, const Building*>;
  using unit_dependency_map_t = std::unordered_map<const UnitSpec*, const Building*>;
  using item_power_requirements_map_t = std::unordered_map<const Skill*, school_value_map>;
  
  static experience_levels normalUnitLevels, heroLevels;
  
private:
  template<typename T> static map_t<T>& containerFor();
  
  static building_dependency_map_t buildingDependsOnBuilding;
  static building_replacement_map_t buildingReplacedByBuilding;
  static unit_dependency_map_t unitDependsOnBuilding;
  static item_power_requirements_map_t _itemPowerRequirements;
  
public:
  /* register data associated with an identifier, requires containerFor<T> to be istantiated */
  template<typename T> static bool registerData(const key_type& ident, const T data)
  {
    auto& container = containerFor<T>();
    if (container.find(ident) != container.end())
      return false;
    
    container.insert(std::make_pair(ident, data));
    return true;
  }
  
  /* get data associated with an identifier, requires containerFor<T> to be istantiated */
  template<typename T> static const T get(const key_type& ident)
  {
    const auto& map = containerFor<T>();
    const auto it = map.find(ident);
    if (it != map.end())
      return it->second;
    else
    {
      LOGD("Data value not found for '%s' of type '%s'", ident.c_str(), nameForDataType<T>());
      assert(false);
      return nullptr;
    }
  }
  
  template<typename T> static void isValidKey(const key_type& ident)
  {
    const auto& map = containerFor<T>();
    const auto it = map.find(ident);
    LOGD("Identifier not found for %s", ident.c_str());
    assert(it != map.end());
  }
  
  static const std::array<School, 5>& schoolsWithoutArcane();
  static const std::array<SpellRarity, 4>& spellRarities();
  
  static const experience_levels& experienceLevelsForUnits() { return normalUnitLevels; }
  static const experience_levels& experienceLevelsForHeroes() { return heroLevels; }
  static const school_value_map* itemPowerRequirements(const Skill* skill) {
    auto it = _itemPowerRequirements.find(skill);
    return it != _itemPowerRequirements.end() ? &it->second : nullptr;
  }
  
  static const Skill* skill(const key_type& ident);
  static const Building* building(const key_type& ident);
  static const UnitSpec* unit(const key_type& ident);
  static const Spell* spell(const key_type& ident);
  static const Race* race(const key_type& ident);
  static const Retort* retort(const key_type& ident);
  static const Wizard* wizard(const key_type& ident);
  
  template <typename T> static const map_t<T>& values() {
    const auto& map = containerFor<T>();
    return map;
  }

  template <typename T> static size_t count() { return containerFor<T>().size(); }
  //TODO: maybe return something which can be used in a foreach loop
  template <typename T> static auto iterators()
  { 
    return std::make_pair(
      typename map_t<T>::value_iterator(containerFor<T>().begin()),
      typename map_t<T>::value_iterator(containerFor<T>().end())
    );
  }

  template <typename T> static auto orderedIterators()
  {
    return std::make_pair(
      containerFor<T>().begin(),
      containerFor<T>().end()
    );
  }

    
  static std::vector<const RaceUnitSpec*> unitsForRace(const Race* race);
  
  static auto requiredBuildingsForUnit(const UnitSpec* unit) { return unitDependsOnBuilding.equal_range(unit); }
  static auto requiredBuildingsForBuilding(const Building* building) { return buildingDependsOnBuilding.equal_range(building); }
  static const Building* buildingThatReplacesBuilding(const Building* building) { auto it = buildingReplacedByBuilding.find(building); return it != buildingReplacedByBuilding.end() ? it->second : nullptr; }

#if defined(DEBUG)
  template<typename T> static const char* nameForDataType();// { return "unnamed"; }
  
  template<typename T> static std::string getInfo(T data)
  {
    char buffer[128];
    sprintf(buffer, "%p", data);
    return buffer;
  }
  
  template<typename T> static void getInfo()
  {
    auto& container = containerFor<T>();
    LOGD("[data] size: %zu", container.size())
    for (const auto& entry : container)
      LOGD("  %s -> %s", entry.first.c_str(), getInfo(entry.second).c_str());
  }
 #endif
  
  friend class yaml;
};

inline const Building*  operator"" _building(const char* key, size_t size) { return Data::building(key); }
inline const Skill*  operator"" _skill(const char* key, size_t size) { return Data::skill(key); }
inline const UnitSpec*  operator"" _unitspec(const char* key, size_t size) { return Data::unit(key); }
//inline const RaceUnitSpec*  operator"" _raceunitspec(const char* key, size_t size) { return Data::unit(key)->as<RaceUnitSpec>(); }

