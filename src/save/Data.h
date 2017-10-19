#pragma once

#include "common/Common.h"

#include <string>
#include <map>
#include <unordered_map>
#include <list>

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
    using key_type = typename inner_type::key_type;
    using value_type = typename inner_type::value_type;
    using map_iterator = typename inner_type::const_iterator;
    using ordered_iterator = typename decltype(order)::const_iterator;
    
    class const_iterator
    {
    public:
      using inner_it = typename std::vector<std::reference_wrapper<const key_type>>::const_iterator;
      
      using value_type = T; //std::pair<const std::string&, T>;
      using difference_type = ptrdiff_t;
      using pointer = T*;
      using reference = T&;
      using iterator_category = std::random_access_iterator_tag;
      
    private:
      const insertion_ordered_map<T>& data;
      inner_it it;
      
    public:
      const_iterator(const insertion_ordered_map<T>& data, inner_it it) : data(data), it(it) { }
      
      const_iterator& operator+=(size_t i) { it += i; return *this; }
      
      inline bool operator!=(const const_iterator& other) const { return &data != &other.data || it != other.it; }
      inline const const_iterator& operator++() { ++it; return *this; }
      T operator*() const { return data.find((*it).get())->second; }
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

  using unit_dependency_map_t = std::unordered_map<const UnitSpec*, const Building*>;
  
  using skill_replacement_map_t = std::unordered_map<const Skill*, const Skill*>;
  
  static experience_levels normalUnitLevels, heroLevels;
  
private:
  template<typename T> static map_t<T>& containerFor();
  
  static unit_dependency_map_t unitDependsOnBuilding;
  
  static skill_replacement_map_t skillReplacementMap;
  
public:  
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
      LOGD("Data value not found for '%s' of type '%s'", ident.c_str(), nameForDataType<T>());
      assert(false);
    }
  }
  
  template<typename T> static void isValidKey(const key_type& ident)
  {
    const auto& map = containerFor<T>();
    const auto it = map.find(ident);
    LOGD("Identifier not found for %s", ident.c_str());
    assert(it != map.end());
  }
  
  static const experience_levels& experienceLevelsForUnits() { return normalUnitLevels; }
  static const experience_levels& experienceLevelsForHeroes() { return heroLevels; }
  
  static const Skill* skill(const key_type& ident) { return get<const Skill*>(ident); }
  static const Building* building(const key_type& ident) { return get<const Building*>(ident); }
  static const UnitSpec* unit(const key_type& ident) { return get<const UnitSpec*>(ident); }
  static const Spell* spell(const key_type& ident) { return get<const Spell*>(ident); }
  static const Race* race(const key_type& ident) { return get<const Race*>(ident); }
  static const Retort* retort(const key_type& ident) { return get<const Retort*>(ident); }
  static const Wizard* wizard(const key_type& ident) { return get<const Wizard*>(ident); }
  
  template <typename T> static const map_t<T>& values() {
    const auto& map = containerFor<T>();
    return map;
  }
    
  static std::vector<const RaceUnitSpec*> unitsForRace(const Race* race);
  
  static std::pair<unit_dependency_map_t::const_iterator, unit_dependency_map_t::const_iterator> requiredBuildingsForUnit(const UnitSpec* unit)
  {
    return unitDependsOnBuilding.equal_range(unit);
  }
  
#if defined(DEBUG)
  template<typename T> static const char* nameForDataType() { return "unnamed"; }
  
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
