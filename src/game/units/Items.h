#ifndef _ITEMS_H_
#define _ITEMS_H_

#include "common/Common.h"

#include <vector>
#include <numeric>
#include <array>
#include <string>

class Skill;
class SkillEffect;
enum class Property : u8;

namespace items
{
  constexpr size_t TYPES_COUNT = 10;
  
  enum class Class : u8 { MELEE = 0, RANGED, MELEE_STAFF, STAFF_WAND, ARMOR, MISC };
  enum TypeID : u8 { SWORD = 0, MACE, AXE, BOW, STAFF, WAND, MISC, SHIELD, CHAIN, PLATE };
  
  class Item
  {
    
  public:
    
    struct TypeGroup
    {
      I18 name;
      u16 size;
    };
    
    struct Type
    {
      const TypeID type;
      const Class iclass;
      const std::vector<TypeGroup> groups;
      
      Type(TypeID type, Class iclass, std::initializer_list<TypeGroup> groups) : type(type), iclass(iclass), groups(groups) { }
      
      u16 count() const { return std::accumulate(groups.begin(), groups.end(), 0, [](s16 a, const TypeGroup& g) { return a + g.size; }); }
      
      TypeGroup typeForIndex(s16 index)
      {
        for (auto &g : groups)
          if (index < g.size) return g;
          else  index -= g.size;
        
        return groups[0];
      }
    };
    
    static const Item::Type* typeForItem(TypeID type);
    constexpr static u32 MAX_SLOTS = 3;
  };
  
  class Slots
  {
  public:
    const std::array<Class, 3> types;
    Slots(Class c1, Class c2, Class c3) : types({c1,c2,c3}) { }
  };
  
  class PropertyAffixSpec
  {
    const Property property;
    std::vector<s16> values;
    std::vector<u16> costs;
    std::string _name; //TODO: localize
    
  public:
    PropertyAffixSpec(const PropertyAffixSpec& other, size_t limit) :
    property(other.property), _name(other._name), values(other.values.begin(), other.values.begin()+limit), costs(other.costs.begin(), other.costs.begin()+limit)
    {
      
    }
    
    PropertyAffixSpec(const PropertyAffixSpec& other) : PropertyAffixSpec(other, other.size()) { }
    
    PropertyAffixSpec(Property property, const std::string& name, const std::initializer_list<s16>& values, const std::initializer_list<u16>& costs) :
    property(property), _name(name), values(values), costs(costs)
    {
      assert(values.size() == costs.size());
    }
    
    size_t sizeForCost(u16 max) const
    {
      if (max == 0) return size();
      
      size_t i = 0;
      for (i = 0; i < costs.size(); ++i)
        if (costs[i] > max)
          return i;
      
      return size();
    }
    
    size_t size() const { return values.size(); }
    s16 valueAt(size_t index) const { return values[index]; }
    u16 costAt(size_t index) const { return costs[index]; }
    const std::string& name() const { return _name; }
  };
  
  struct Affixes
  {
    const std::vector<PropertyAffixSpec>& properties;
    
    static Affixes forType(TypeID type);
  };
  
}


#endif
