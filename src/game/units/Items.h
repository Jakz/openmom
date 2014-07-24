#ifndef _ITEMS_H_
#define _ITEMS_H_

#include "Common.h"

#include <vector>

class Skill;
class SkillEffect;
enum class Property : u8;

enum class ItemSlots : u8
{
  RANGED,
  BATTLE_MAGE,
  MAGE,
  WARRIOR
};

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
    const static u32 MAX_SLOTS = 3;
};

#endif
