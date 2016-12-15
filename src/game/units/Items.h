#ifndef _ITEMS_H_
#define _ITEMS_H_

#include "common/Common.h"

#include <vector>
#include <numeric>
#include <array>

class Skill;
class SkillEffect;
enum class Property : u8;

namespace ItemEnchant
{
  class PropertyEnchant
  {
    const Property property;
    const s16 max1, max2;
    const s16 step;
    
    PropertyEnchant(Property property, s16 max1, s16 max2, s16 step) : property(property), max1(max1), max2(max2), step(step) { }
  };
  
  class PowerEnchant
  {
    const School school;
    const Skill* skill;
    u16 minPicks;
    
    PowerEnchant(const Skill* skill, School school, u16 minPicks) : skill(skill), school(school), minPicks(minPicks) { }
  };
  
  class ItemClassSpec
  {
    const std::vector<const SkillEffect*> nativeEffects;
    const std::vector<PropertyEnchant> propertyBonuses;
    
    ItemClassSpec(std::initializer_list<const SkillEffect*> nativeEffects, std::initializer_list<PropertyEnchant> propertyBonuses) : nativeEffects(nativeEffects), propertyBonuses(propertyBonuses) { }
  };
}

class ItemEnchants
{
  static const ItemEnchant::PowerEnchant POWER_BLESS;
  static const ItemEnchant::ItemClassSpec SPEC_SWORD;
};


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
    const u16 startOffset;
    
    Type(TypeID type, Class iclass, u16 startOffset, std::initializer_list<TypeGroup> groups) : type(type), iclass(iclass), groups(groups), startOffset(startOffset) { }
    
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
  const static u32 MAX_SLOTS = 3;
};

class ItemSlots
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

  ItemSlots(std::array<std::vector<Item::Class>, 3> types, std::array<u16, 3> sprites) : types(types), sprites(sprites) { }
  
  static const ItemSlots* slotsFor(ItemSlots::Type type);
};



#endif
