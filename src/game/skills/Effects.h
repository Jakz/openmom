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

class SkillEffect;
using effect_list = std::vector<const SkillEffect*>;
using effect_init_list = const std::initializer_list<const SkillEffect*>;

enum class Property : u8;
class Unit;

class SkillEffect
{
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
    
    SPECIAL_ATTACK
  } type;
  
  SkillEffect(Type type) : type(type) { }
  template<typename T> const T* as() const { return static_cast<const T*>(this); }
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
  const s16 param;
  
  SimpleParametricEffect(SkillEffect::Type type, Type effect, s16 param) : SimpleEffect(type, effect), param(param) { }
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
  
  // TODO: finish
};

class UnitBonus : public PropertyBonus
{
public:
  UnitBonus(Property property, s16 value) : PropertyBonus(SkillEffect::Type::UNIT_BONUS, property, value) { }

  
  static const effect_list build(std::initializer_list<Property> properties, s16 value)
  {
    effect_list effects;
    effects.resize(properties.size());
    std::transform(properties.begin(), properties.end(), effects.begin(), [&] (const Property& property) { return new UnitBonus(property, value); });
    return effects;
  }
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
public:
  enum class StackableGroup : u16 { NONE = 0, START = 1 };
  
protected:
  StackableGroup group;
  bool applicableOn(const Unit* unit) const;
  
public:
  const enum class Type { WHOLE_ARMY, NORMAL_UNITS } target;
  
  ArmyBonus(Property property, s16 value, Type target, StackableGroup group = StackableGroup::NONE) : PropertyBonus(SkillEffect::Type::ARMY_BONUS, property, value), target(target), group(group) { }
  
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
    DEFENDER
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

class MovementEffect : public SkillEffect
{
private:
  const MovementType _type;
  
public:
  MovementEffect(MovementType type) : SkillEffect(SkillEffect::Type::MOVEMENT), _type(type) { }
  bool operator==(MovementType type) const { return _type == type; }
  MovementType type() const { return _type; }
};



#endif
