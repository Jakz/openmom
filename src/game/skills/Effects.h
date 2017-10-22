//
//  Effects.h
//  OpenMoM
//
//  Created by Jack on 7/21/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _SKILL_EFFECTS_H_
#define _SKILL_EFFECTS_H_

#include "common/Common.h"

#include <vector>

enum class Property : u8;
class Unit;

struct SkillEffectGroup
{
public:
  enum class Mode : u16 { KEEP_ALL, UNIQUE, KEEP_GREATER, KEEP_LESSER };
 
private:
  Mode _mode;
  
  
public:
  SkillEffectGroup(Mode mode) : _mode(mode) { }
  
  Mode mode() const { return _mode; }
};

class SkillEffect
{
public:
  enum class Order { GREATER, LESSER, EQUAL, UNCOMPARABLE, DIFFERENT };
  
protected:
  const SkillEffectGroup* _group;
  
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
    
    SPECIAL_ATTACK
  } type;
  
  
  SkillEffect(Type type, const SkillEffectGroup* group) : type(type), _group(group) { }
  SkillEffect(Type type) : SkillEffect(type, nullptr) { }
  
  void setGroup(const SkillEffectGroup* group) { this->_group = group; }
  const SkillEffectGroup* group() const { return _group; }

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

class SpecialAttackEffect : public SimpleEffect
{
public:
  SpecialAttackEffect(SimpleEffect::Type attack, s16 strength = 0) : SimpleEffect(SkillEffect::Type::SPECIAL_ATTACK, attack), strength(strength) { }
  const s16 strength;
};

class PropertyBonus : public SkillEffect
{
protected:
  PropertyBonus(SkillEffect::Type type, Property property, s16 value) : SkillEffect(type), property(property), value(value) { }
  
public:
 
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
  UnitBonus(Property property, s16 value) : PropertyBonus(SkillEffect::Type::UNIT_BONUS, property, value) { }
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
  
  ArmyBonus(Property property, s16 value, Type target) : PropertyBonus(SkillEffect::Type::ARMY_BONUS, property, value), target(target) { }
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
    ATTACKER,
    DEFENDER,
    ALWAYS
  };
  
  CombatBonus(Property property, s16 value, Phase owner, Phase target, bool boundToSkill) : SkillEffect(SkillEffect::Type::COMBAT_BONUS), property(property), value(value), owner(owner), target(target), boundToSkill(boundToSkill) { }
  
  const Phase owner, target;
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

/*class MovementEffect : public SkillEffect
{
private:
  const MovementType _type;
  
public:
  MovementEffect(MovementType type) : SkillEffect(SkillEffect::Type::MOVEMENT), _type(type) { }
  bool operator==(MovementType type) const { return _type == type; }
  MovementType type() const { return _type; }
  
  Order compare(const Unit* unit, const SkillEffect* other) const override
  {
    if (other->type == SkillEffect::Type::MOVEMENT)
      return other->as<MovementEffect>()->type() == type() ? Order::EQUAL : Order::DIFFERENT;
    else
      return Order::UNCOMPARABLE;
  }
};*/

using MovementEffect = SkillEnumEffect<MovementType, SkillEffect::Type::MOVEMENT>;

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
  size_t size() const { return data.size(); }
  
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

#endif
