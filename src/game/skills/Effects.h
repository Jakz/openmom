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

using SKillEffectGroupParam = value_t;
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

class SkillEffect
{
public:
  enum class Order { GREATER, LESSER, EQUAL, UNCOMPARABLE, DIFFERENT };
  
protected:
  const SkillEffectGroup* _group;
  SKillEffectGroupParam _groupParam;
  
public:
  const enum class Type : u8
  {
    MOVEMENT,
    ABILITY,
    PARAMETRIC_ABILITY,
    IMMUNITY,
    MAGIC_WEAPONS,
    
    COMBAT_BONUS,
    UNIT_BONUS,
    ARMY_BONUS,
    
    GRANT_SPELL,
    
    SPECIAL_ATTACK,

    COMPOUND
  } type;
  
  
  SkillEffect(Type type, const SkillEffectGroup* group) : type(type), _group(group) { }
  SkillEffect(Type type) : SkillEffect(type, nullptr) { }
  
  virtual bool isCompound() const { return false; }
  virtual size_t size() const { return 1; }

  void setGroup(const SkillEffectGroup* group, SKillEffectGroupParam param = 0) { this->_group = group; this->_groupParam = param; }
  const SkillEffectGroup* group() const { return _group; }
  SKillEffectGroupParam groupParam() const { return _groupParam;  }

  virtual Order compare(const Unit* unit, const SkillEffect* other) const { return Order::UNCOMPARABLE; }
  
  template<typename T> Type typeForClass() const { return SkillEffect::Type::UNIT_BONUS;  }
  template<typename T> const T* as() const { return true || type == typeForClass<T>() ? static_cast<const T*>(this) : nullptr; }
};

class ArmyBonus;
class UnitBonus;

template<> inline SkillEffect::Type SkillEffect::typeForClass<ArmyBonus>() const { return SkillEffect::Type::ARMY_BONUS; }
template<> inline SkillEffect::Type SkillEffect::typeForClass<UnitBonus>() const { return SkillEffect::Type::UNIT_BONUS; }


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
    
    FIRE_BREATH,
    LIGHTING_BREATH,
    
    THROWN_ATTACK,
    
    DOOM_GAZE,
    DEATH_GAZE,
    STONE_GAZE,
    
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
    ILLUSIONARY_ATTACK
  } effect;
  
  SimpleEffect(SkillEffect::Type type, Type effect) : SkillEffect(type), effect(effect) { }
};

class SimpleParametricEffect : public SimpleEffect
{
public:
  SimpleParametricEffect(SkillEffect::Type type, Type effect, s16 param) : SimpleEffect(type, effect), param(param) { }
  const s16 param;
};

class PropertyBonus : public SkillEffect
{
public:
  enum class Mode
  {
    ADDITIVE,
    OVERRIDE, 
    OVERRIDE_IF_GREATER,
    OVERRIDE_IF_LESSER
  };
  
protected:
  PropertyBonus(SkillEffect::Type type, Property property, Mode mode, s16 value) : SkillEffect(type), property(property), mode(mode), value(value) { }
  
public:

  const Mode mode;
  const Property property;
  const s16 value;
  
  bool sameProperty(Property property) const { return this->property == property; }
  
  virtual s16 getValue(const Unit* unit) const { return value; }
  
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
  UnitBonus(Property property, Mode mode, s16 value) : PropertyBonus(SkillEffect::Type::UNIT_BONUS, property, mode, value) { }
  UnitBonus(Property property, s16 value) : UnitBonus(property, PropertyBonus::Mode::ADDITIVE, value) { }
};

class UnitLevelBonus : public UnitBonus
{
private:
  const float multiplier;
  
public:
  UnitLevelBonus(Property property, float multiplier) : UnitBonus(property, 0), multiplier(multiplier) { }

  s16 getValue(const Unit* unit) const override;
};

class FilterUnitBonus : public UnitBonus
{
private:
  predicate<const Unit*> filter;
  
public:
  FilterUnitBonus(Property property, s16 value, School school);
  s16 getValue(const Unit* unit) const override;
};

class ArmyBonus : public PropertyBonus
{
protected:
  bool applicableOn(const Unit* unit) const;
  
public:
  const enum class Type { WHOLE_ARMY, NORMAL_UNITS } target;
  
  ArmyBonus(Property property, Mode mode, s16 value, Type target) : PropertyBonus(SkillEffect::Type::ARMY_BONUS, property, mode, value), target(target) { }
  ArmyBonus(Property property, s16 value, Type target) : PropertyBonus(SkillEffect::Type::ARMY_BONUS, property, PropertyBonus::Mode::ADDITIVE, value), target(target) { }
  s16 getValue(const Unit* unit) const override;
};

class ArmyLevelBonus : public ArmyBonus
{
private:
  const float multiplier;
  
public:
  ArmyLevelBonus(Property property, float multiplier, Type target) : ArmyBonus(property, 0, target), multiplier(multiplier) { }
  
  s16 getValue(const Unit* unit) const override;
};

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
  
  CombatBonus(Property property, s16 value, Phase trigger, Target target, bool boundToSkill) : SkillEffect(SkillEffect::Type::COMBAT_BONUS), property(property), value(value), trigger(trigger), target(target), boundToSkill(boundToSkill) { }
  
  const Phase trigger;
  const Target target;
  const Property property;
  const s16 value;
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

enum SpecialAttackType
{
  THROWN_ATTACK,
  
  FIRE_BREATH,
  
  POISON_TOUCH
};

class SpecialAttackEffect : public SkillEnumEffect<SpecialAttackType, SkillEffect::Type::SPECIAL_ATTACK>
{
private:
  s16 _strength;
  
public:
  SpecialAttackEffect(SpecialAttackType type, s16 strength) : SkillEnumEffect(type), _strength(strength) { }
  
  s16 strength() const { return _strength; }
};

//TODO: technically is a combat instant spell
//TODO: implement mechanics
class Spell;
class SpellGrantEffect : public SkillEffect
{
private:
  const Spell* _spell;
  const u16 _times;
  const s16 _strength;
  
public:
  SpellGrantEffect(const Spell* spell, const u16 times, const s16 strength = 0) : SkillEffect(SkillEffect::Type::GRANT_SPELL), _spell(spell), _times(times), _strength(strength) { }
  
  const Spell* spell() const { return _spell; }
  u16 times() const { return _times; }
  s16 strength() const { return _strength; }
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
static const effect_list unit_bonus_build(std::initializer_list<Property> properties, s16 value)
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



