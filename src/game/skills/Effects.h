//
//  Effects.h
//  OpenMoM
//
//  Created by Jack on 7/21/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#pragma once

#include "common/Common.h"

#include <vector>
#include <stack>

enum class Property : u8;
class Unit;
class SkillEffect;

using SkillEffectGroupParam = value_t;
struct SkillEffectGroup
{
public:
  enum class Mode : u16 { KEEP_ALL, UNIQUE, KEEP_GREATER, KEEP_LESSER, PRIORITY };
 
private:
  Mode _mode;
  const SkillEffect* _specific;
  
public:
  SkillEffectGroup(Mode mode) : _mode(mode), _specific(nullptr) { }
  
  const SkillEffect* master() { return _specific; }
  Mode mode() const { return _mode; }
};

struct ModifierValue
{
  enum Priority { FIRST, ANY, LAST } priority;
  enum Type { FLAT, MULTIPLIER, LEVEL_BASED } type;
  union {
    float multiplier;
    value_t value;
  };
  ModifierValue(value_t value) : priority(Priority::ANY), type(Type::FLAT), value(value) { }
  ModifierValue(float multiplier) : priority(Priority::ANY), type(Type::LEVEL_BASED), multiplier(multiplier) { }
};

class SkillEffect
{
public:
  enum class Order { GREATER, LESSER, EQUAL, UNCOMPARABLE, DIFFERENT };
  
protected:
  const SkillEffectGroup* _group;
  SkillEffectGroupParam _groupParam;
  
public:
  const enum class Type : u8
  {
    MOVEMENT,
    DISALLOW_MOVEMENT,
    
    ABILITY,
    PARAMETRIC_ABILITY,
    IMMUNITY,
    MAGIC_WEAPONS,
    
    COMBAT_BONUS,
    UNIT_BONUS,
    ARMY_BONUS,
    
    GRANT_SPELL,
    
    SPECIAL_ATTACK,
    
    PLAYER_BONUS,

    COMPOUND
  } type;
  
  
  SkillEffect(Type type, const SkillEffectGroup* group) : type(type), _group(group) { }
  SkillEffect(Type type) : SkillEffect(type, nullptr) { }
  
  virtual bool isCompound() const { return false; }
  virtual size_t size() const { return 1; }

  void setGroup(const SkillEffectGroup* group, SkillEffectGroupParam param = 0) { this->_group = group; this->_groupParam = param; }
  const SkillEffectGroup* group() const { return _group; }
  SkillEffectGroupParam groupParam() const { return _groupParam;  }

  virtual Order compare(const Unit* unit, const SkillEffect* other) const { return Order::UNCOMPARABLE; }
  
  template<typename T> const T* as() const { return static_cast<const T*>(this); }
};

template<typename T, SkillEffect::Type TYPE>
class SkillEnumEffect : public SkillEffect
{
private:
  const T _subType;
public:
  SkillEnumEffect(T type) : SkillEffect(TYPE), _subType(type) { }
  
  bool operator==(T type) const { return _subType == type; }
  T subType() const { return _subType; }
  
  Order compare(const Unit* unit, const SkillEffect* other) const override
  {
    if (other->type == type)
      return other->as<SkillEnumEffect<T, TYPE>>()->subType() == subType() ? Order::EQUAL : Order::DIFFERENT;
    else
      return Order::UNCOMPARABLE;
  }
};

class SimpleEffect : public SkillEffect
{
public:
  const enum class Type : u16
  {
    INVISIBILITY,
    
    
    IMMUNITY_MAGIC,
    IMMUNITY_ILLUSIONS,
    IMMUNITY_MISSILE,
    
    
    POISON,
    LIFE_STEALING,
    STONE_TOUCH,
    
    IMMOLATION,

    HEALER, 
    
    CREATE_OUTPOST,
    CREATE_ROAD,
    MELD_NODE,
    WALL_CRUSHING,
    PURIFY,
    
    FIRST_STRIKE,
    NEGATE_FIRST_STRIKE,
    ARMOR_PIERCING,
    LONG_RANGE,
    ILLUSIONARY_ATTACK,
    
    ALLOW_MELEE_ATTACKS_AGAINST_FLYING
  } effect;
  
  SimpleEffect(SkillEffect::Type type, Type effect) : SkillEffect(type), effect(effect) { }
};

class SimpleParametricEffect : public SimpleEffect
{
public:
  SimpleParametricEffect(SkillEffect::Type type, Type effect, value_t param) : SimpleEffect(type, effect), param(param) { }
  const value_t param;
};

class PropertyBonus : public SkillEffect
{
protected:
  PropertyBonus(SkillEffect::Type type, Property property, value_t value) : SkillEffect(type), property(property), value(value) { }

public:
  const Property property;
  const value_t value;
  
  virtual value_t getValue(const Unit* unit) const { return value; }
  bool sameProperty(Property property) const { return this->property == property; }
  
  Order compare(const Unit* unit, const SkillEffect* other) const override
  {
    if (other->type != SkillEffect::Type::UNIT_BONUS && other->type != SkillEffect::Type::ARMY_BONUS && other->type != SkillEffect::Type::COMBAT_BONUS)
      return Order::UNCOMPARABLE;
    else
    {
      const PropertyBonus* bonus = other->as<PropertyBonus>();
      
      if (bonus->property != property)
        return Order::UNCOMPARABLE;
      else
      {
        if (property < bonus->property)
          return Order::LESSER;
        else if (property > bonus->property)
          return Order::GREATER;
        else
          return Order::EQUAL;
      }
    }
  }
};

class UnitBonus : public PropertyBonus
{
public:
  UnitBonus(Property property, value_t value) : PropertyBonus(SkillEffect::Type::UNIT_BONUS, property, value) { }
};

class UnitLevelBonus : public UnitBonus
{
private:
  const float multiplier;
  
public:
  UnitLevelBonus(Property property, float multiplier) : UnitBonus(property, 0), multiplier(multiplier) { }

  value_t getValue(const Unit* unit) const override;
};

class FilterUnitBonus : public UnitBonus
{
private:
  predicate<const Unit*> filter;
  
public:
  FilterUnitBonus(Property property, value_t value, School school);
  value_t getValue(const Unit* unit) const override;
};

class ArmyBonus : public PropertyBonus
{
public:
  const enum class Type { WHOLE_ARMY, NORMAL_UNITS } target;

protected:
  bool applicableOn(const Unit* unit) const;
public:
  ArmyBonus(Property property, value_t value, Type target) : PropertyBonus(SkillEffect::Type::ARMY_BONUS, property, value), target(target) { }
  value_t getValue(const Unit* unit) const override;
};

class ArmyLevelBonus : public ArmyBonus
{
private:
  const float multiplier;
  
public:
  ArmyLevelBonus(Property property, float multiplier, Type target) : ArmyBonus(property, 0, target), multiplier(multiplier) { }
  
  value_t getValue(const Unit* unit) const override;
};

template<typename EnumType, SkillEffect::Type SkillType>
class ModifierEffect : public SkillEffect
{
private:
  EnumType _property;
  ModifierValue _value;
  
public:
  ModifierEffect(PlayerAttribute attribute, value_t value) : SkillEffect(SkillType), _property(attribute), _value(value) { }
  ModifierEffect(PlayerAttribute attribute, float value) : SkillEffect(SkillType), _property(attribute), _value(value) { }
  
  value_t getValue(const Unit* unit, EnumType attribute) const;
};

using PlayerAttributeModifier = ModifierEffect<PlayerAttribute, SkillEffect::Type::PLAYER_BONUS>;

class CombatBonus : public SkillEffect
{
public:
  enum class Phase : u8
  {
    BOTH,
    ATTACKING,
    DEFENDING
  };
  
  enum class Target : u8
  {
    ATTACKER,
    DEFENDER,
  };
  
  CombatBonus(Property property, value_t value, Phase trigger, Target target, bool boundToSkill) : SkillEffect(SkillEffect::Type::COMBAT_BONUS), property(property), value(value), trigger(trigger), target(target), boundToSkill(boundToSkill) { }
  
  const Phase trigger;
  const Target target;
  const Property property;
  const value_t value;
  const bool boundToSkill; // TODO wtf?
};


enum class MovementType
{
  NORMAL = 0,
  FORESTWALK,
  FLYING,
  UNDERGROUND,
  MOUNTAINWALK,
  NON_CORPOREAL,
  PATH_FINDER,
  PLANAR_TRAVEL,
  TELEPORT,
  SWIMMING,
  WINDWALK,
  SAILING,
};

using MovementEffect = SkillEnumEffect<MovementType, SkillEffect::Type::MOVEMENT>;
using MovementDisallowEffect = SkillEnumEffect<MovementType, SkillEffect::Type::DISALLOW_MOVEMENT>;

enum SpecialAttackType
{
  THROWN_ATTACK,
  
  FIRE_BREATH,
  
  POISON_TOUCH
};

class SpecialAttackEffect : public SkillEnumEffect<SpecialAttackType, SkillEffect::Type::SPECIAL_ATTACK>
{
private:
  value_t _strength;
  
public:
  SpecialAttackEffect(SpecialAttackType type, value_t strength) : SkillEnumEffect(type), _strength(strength) { }
  
  value_t strength() const { return _strength; }
};

//TODO: technically is a combat instant spell
//TODO: implement mechanics
class Spell;
class SpellGrantEffect : public SkillEffect
{
private:
  const Spell* _spell;
  const value_t _times;
  const value_t _strength;
  
public:
  SpellGrantEffect(const Spell* spell, const value_t times, const value_t strength = 0) : SkillEffect(SkillEffect::Type::GRANT_SPELL), _spell(spell), _times(times), _strength(strength) { }
  
  const Spell* spell() const { return _spell; }
  value_t times() const { return _times; }
  value_t strength() const { return _strength; }
};

using effect_init_list = const std::initializer_list<const SkillEffect*>;
struct effect_list;
struct effect_list_deep_iterator;

struct effect_list
{
private:
  using inner_type_t = std::vector<const SkillEffect*>;
  inner_type_t data;
  
public:
  using iterator = inner_type_t::const_iterator;
  using value_type = inner_type_t::value_type;
  
  effect_list() { }
  effect_list(const inner_type_t& effects) : data(effects) { }
  effect_list(const effect_init_list& list) : data(list) { }
  
  void push_back(const SkillEffect* effect) { data.push_back(effect); }
  void resize(size_t size) { data.resize(size); }
  
  iterator begin() const { return data.begin(); }
  iterator end() const { return data.end(); }
  
  effect_list_deep_iterator dbegin() const;
  effect_list_deep_iterator dend() const;
  
  size_t size() const { return data.size(); }
  size_t flatSize() const { return std::accumulate(data.begin(), data.end(), 0UL, [](size_t v, const SkillEffect* effect) { return v + effect->size(); }); }

  void filter(std::function<bool(const SkillEffect*)> predicate)
  {
    auto nend = std::remove_if(data.begin(), data.end(), [&predicate](const SkillEffect* effect) { return !predicate(effect); });
    data.erase(nend, data.end());
  }
  
  const SkillEffect* operator[](size_t index) const { return data[index]; }
  
  effect_list operator+(const effect_list& other) const {
    effect_list result;
    auto& rdata = result.data;
    rdata.reserve(data.size() + other.data.size());
    rdata.insert(rdata.end(), data.begin(), data.end());
    rdata.insert(rdata.end(), other.data.begin(), other.data.end());
    return result;
  }
  
  effect_list& operator+=(const effect_list& other) {
    data.reserve(data.size()+other.data.size());
    data.insert(data.end(), other.data.begin(), other.data.end());
    return *this;
  }

  /* flatten nested effects */
  effect_list flatten();
  
  /* this method builds the actual effects taking into consideration
   that some effects override or replace others */
  effect_list actuals(const Unit* unit) const;
};

//TODO: to remove after hardcoded effects has been removed
static const effect_list unit_bonus_build(std::initializer_list<Property> properties, value_t value)
{
  effect_list effects;
  effects.resize(properties.size());
  std::transform(properties.begin(), properties.end(), std::back_inserter(effects), [&] (const Property& property) { return new UnitBonus(property, value); });
  return effects;
}

class CompoundEffect : public SkillEffect
{
private:
  effect_list effects;

public:
  CompoundEffect() : SkillEffect(SkillEffect::Type::COMPOUND) { }
  CompoundEffect(SkillEffectGroup* group) : SkillEffect(SkillEffect::Type::COMPOUND, group) { }
  CompoundEffect(const effect_list& effects) : SkillEffect(SkillEffect::Type::COMPOUND), effects(effects) { }

  bool isCompound() const override { return true; }
  size_t size() const override { return effects.size(); }

  effect_list::iterator begin() const { return effects.begin(); }
  effect_list::iterator end() const { return effects.end(); }

  friend class effect_list_deep_iterator;
};

struct effect_list_deep_iterator
{
private:
  struct stack_v {
    const effect_list* list;
    effect_list::iterator it;
    bool end() const { return it == list->end(); }
    const SkillEffect* operator*() const { return *it; }
    stack_v(const effect_list* list, effect_list::iterator it) : list(list), it(it) { }
    bool operator==(const stack_v& other) const { return list == other.list && it == other.it; }
  };
  
  std::stack<stack_v> stack;
  
  void adjust()
  {
    if (stack.top().end())
      return;
    
    const SkillEffect* c = operator*();
    
    while (c && c->isCompound())
    {
      const CompoundEffect* ce = c->as<CompoundEffect>();
      stack.emplace(&ce->effects, ce->effects.begin());
      adjust();
      
      if (!stack.top().end())
        c = operator*();
      else
      {
        stack.pop();
        break;
      }
    }
  }
  
public:
  using difference_type = ptrdiff_t;
  using value_type = SkillEffect * ;
  using reference = const SkillEffect&;
  using pointer = const SkillEffect*;
  using iterator_category = std::forward_iterator_tag;
  
public:
  effect_list_deep_iterator(const effect_list* list, effect_list::iterator it) { stack.emplace(list, it); adjust(); }
  
  effect_list_deep_iterator& operator++()
  {
    auto& it = stack.top().it;
    
    /* iterator is not at end */
    if (!stack.top().end())
    {
      ++it;
      adjust();
    }
    
    {
      /* iterator is end of current child: pop and advance parent */
      while (stack.top().end() && stack.size() > 1)
      {
        /* pop scope: we're at end */
        stack.pop();
        
        /* now we're on parent, we should advance it */
        ++stack.top().it;
      }
    }
    
    return *this;
  }
  
  bool operator==(const effect_list_deep_iterator& other) const { return stack == other.stack; }
  bool operator!=(const effect_list_deep_iterator& other) const { return stack != other.stack; }
  
  const SkillEffect* operator*() const { return *stack.top(); }
};

inline effect_list_deep_iterator effect_list::dbegin() const { return effect_list_deep_iterator(this, begin()); }
inline effect_list_deep_iterator effect_list::dend() const { return effect_list_deep_iterator(this, end()); }



