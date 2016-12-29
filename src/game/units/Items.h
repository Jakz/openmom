#ifndef _ITEMS_H_
#define _ITEMS_H_

#include "Common.h"

#include <vector>
#include <numeric>
#include <array>

class Skill;
class SkillEffect;
enum class Property : u8;

namespace items
{
  constexpr size_t TYPES_COUNT = 10;
  
  class Item
  {
    
  public:
    enum class Class : u8 { MELEE = 0, RANGED, MELEE_STAFF, STAFF_WAND, ARMOR, MISC };
    
    struct TypeGroup
    {
      I18 name;
      u16 size;
    };
    
    enum TypeID : u8 { SWORD = 0, MACE, AXE, BOW, STAFF, WAND, MISC, SHIELD, CHAIN, PLATE };
    
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
    enum class Type : u8
    {
      RANGED = 0,
      BATTLE_MAGE,
      MAGE,
      WARRIOR
    };
    
    const std::array<std::vector<Item::Class>, 3> types;
    const std::array<u16, 3> sprites;
    
    Slots(std::array<std::vector<Item::Class>, 3> types, std::array<u16, 3> sprites) : types(types), sprites(sprites) { }
    
    static const Slots* slotsFor(Slots::Type type);
  };
  
  class PropertyAffix
  {
    const Property property;
    std::vector<s16> values;
    std::vector<u16> costs;
    
  public:
    PropertyAffix(const PropertyAffix& other, size_t limit) :
    property(other.property), values(other.values.begin(), other.values.begin()+limit), costs(other.costs.begin(), other.costs.begin()+limit)
    {
      
    }
    
    PropertyAffix(Property property, const std::initializer_list<s16>& values, const std::initializer_list<u16>& costs) :
    property(property), values(values), costs(costs)
    {
      assert(values.size() == costs.size());
    }
  };
  
}


#endif
