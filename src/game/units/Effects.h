//
//  Effects.h
//  OpenMoM
//
//  Created by Jack on 7/21/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _SKILL_EFFECTS_H_
#define _SKILL_EFFECTS_H_

#include "Common.h"

#include <vector>

enum class Property : u8;
class Unit;

enum class SkillEffectType : u8
{
  MOVEMENT,
  IMMUNITY,
  MAGIC_WEAPONS,
  
  PROPERTY_BONUS,
  ARMY_BONUS,
  
  SPECIAL_ATTACK
};

enum class ImmunityEffectID : u8
{
  MAGIC,
  ILLUSIONS,
  MISSILE
};

enum class MovementEffectID : u8 // TODO: maybe they can be moved inside relative effect
{
  FORESTWALK = 0,
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
  DESERTWALK,
  SWAMPWALK
};

enum class SpecialAttackID : u8
{
  POISON,
  LIFE_STEALING,
  STONE_TOUCH,
  
  IMMOLATION,
  
  FIRE_BREATH,
  LIGHTING_BREATH,
  
  THROWN_ATTACK,
  
  DOOM_GAZE,
  DEATH_GAZE,
  STONE_GAZE
};

class SkillEffect
{
public:
  SkillEffect(SkillEffectType type) : type(type) { }
  
  const SkillEffectType type;
};

class MovementEffect : public SkillEffect
{
public:
  MovementEffect(MovementEffectID movement, bool shared) : SkillEffect(SkillEffectType::MOVEMENT), movement(movement), shared(shared) { }
  
  const MovementEffectID movement;
  const bool shared;
};

class ImmunityEffect : public SkillEffect
{
public:
  ImmunityEffect(ImmunityEffectID immunity) : SkillEffect(SkillEffectType::IMMUNITY), immunity(immunity) { }
  
  const ImmunityEffectID immunity;
};

class SpecialAttackEffect : public SkillEffect
{
public:
  SpecialAttackEffect(SpecialAttackID attack, s16 strength = 0) : SkillEffect(SkillEffectType::SPECIAL_ATTACK), attack(attack), strength(strength) { }
  
  const SpecialAttackID attack;
  const s16 strength;
};

class PropertyBonus : public SkillEffect
{
protected:
  PropertyBonus(Property property, s16 value) : SkillEffect(SkillEffectType::PROPERTY_BONUS), property(property), value(value) { }
  
public:
 
  const Property property;
  const s16 value;
  
  virtual const s16 getValue(Unit* unit) const { return value; }
  
  // TODO: finish
};

class UnitBonus : public PropertyBonus
{
public:
  UnitBonus(Property property, s16 value) : PropertyBonus(property, value) { }
};

class ArmyBonus : public PropertyBonus
{
public:
  const enum class Type { WHOLE_ARMY, NORMAL_UNITS } target;
  
  ArmyBonus(Property property, s16 value, Type target) : PropertyBonus(property, value), target(target) { }
  
  const s16 getValue(Unit* unit) const;
};





class Effects
{
public:
  static const MovementEffect FORESTWALK, FLYING, UNDERGROUND, MOUNTAINWALK, NON_CORPOREAL, PATH_FINDER, PLANAR_TRAVEL, TELEPORT, SWIMMING, WINDWALK, SAILING, DESERTWALK, SWAMPWALK;
};

typedef const std::vector<const SkillEffect*> effect_list;
typedef const std::initializer_list<const SkillEffect*> effect_init_list;

#endif
