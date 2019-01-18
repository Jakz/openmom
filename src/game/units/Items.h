#ifndef _ITEMS_H_
#define _ITEMS_H_

#include "common/Common.h"
#include "game/skills/Effects.h"
#include "game/skills/Skill.h"

#include <vector>
#include <numeric>
#include <array>
#include <string>

class Spell;
class Skill;
class SkillEffect;
enum class Property : u8;

namespace items
{
  constexpr size_t TYPES_COUNT = 10;
  
  enum class Class : u8 { MELEE = 0, RANGED, MELEE_STAFF, STAFF_WAND, ARMOR, MISC };
  enum TypeID : u8 { SWORD = 0, MACE, AXE, BOW, STAFF, WAND, MISC, SHIELD, CHAIN, PLATE };
  

  struct PropertyAffix
  {
    Property property;
    value_t value;
  };

  class Item
  {
  private:
    TypeID _type;
    int _gfx;
    
    std::vector<PropertyAffix> _affixes;
    skill_list _skills;


  public:
    Item(TypeID type, int gfx) : _type(type), _gfx(gfx) { }

    void addAffix(PropertyAffix affix) { _affixes.push_back(affix); }
    void add(const Skill* skill) { _skills.push_back(skill); }

    const skill_list& skills() const { return _skills; };
    value_t getBonusProperty(Property property) const
    {
      return std::accumulate(_affixes.begin(), _affixes.end(), 0,
                      [property](value_t v, const PropertyAffix& affix) { return affix.property == property ? v + affix.value : v; }
      );
    }
    
    std::string name() const { return "Stick of the Mage"; } //TODO
    TypeID type() const { return _type; }
    int gfx() const { return _gfx; }
    School school() const;

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
    constexpr static count_t MAX_SLOTS = 3;
  };
  
  template<size_t SIZE>
  class AllowedSlots
  {
  public:
    const std::array<Class, SIZE> types;
    AllowedSlots(Class c1, Class c2, Class c3) : types({c1,c2,c3}) { }
  };
  
  
  template<size_t SIZE>
  class ItemSlots
  {
  private:
    std::array<const Item*, SIZE> _items; //TODO: unique_ptr?
    
    /* we keep this cached because it's useful to have it cached */
    skill_list _skills;

    void cacheSkills()
    {
      std::for_each(_items.begin(), _items.end(), [this](const Item* item) {
        if (item != nullptr)
          _skills.insert(_skills.end(), item->skills().begin(), item->skills().end());
      });
    }
    
  public:
    ItemSlots() : _items({nullptr}) { }
    
    using iterator = typename decltype(_items)::const_iterator;
    
    const skill_list& powers() const { return _skills; }
    
    iterator begin() const { return _items.begin(); }
    iterator end() const { return _items.end(); }
    
    void set(size_t index, const Item* item) { _items[index] = item; cacheSkills(); }
    const Item* operator[](size_t index) const { return _items[index]; }
  };
  
  class PropertyAffixSpec
  {
    const Property property;
    std::vector<value_t> values;
    std::vector<value_t> costs;
    std::string _name; //TODO: localize
    
  public:
    PropertyAffixSpec(const PropertyAffixSpec& other, size_t limit) :
    property(other.property), _name(other._name), values(other.values.begin(), other.values.begin()+limit), costs(other.costs.begin(), other.costs.begin()+limit)
    {
      
    }
    
    PropertyAffixSpec(const PropertyAffixSpec& other) : PropertyAffixSpec(other, other.size()) { }
    
    PropertyAffixSpec(Property property, const std::string& name, const std::initializer_list<value_t>& values, const std::initializer_list<value_t>& costs) :
    property(property), _name(name), values(values), costs(costs)
    {
      assert(values.size() == costs.size());
    }
    
    size_t sizeForCost(value_t max) const
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
  
  class ItemSpellCharge
  {
  private:
    const Spell* spell;
    value_t amount;
  public:
    ItemSpellCharge(const Spell* spell, value_t amount) : spell(spell), amount(amount) { }
  };
}


#endif
