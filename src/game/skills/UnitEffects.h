//
//  Effects.h
//  OpenMoM
//
//  Created by Jack on 7/21/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#pragma once

#include "common/Common.h"

#include "Effects.h"
#include "Modifiers.h"

#include <vector>
#include <stack>

enum class Property : u8;
class Unit;

class SimpleEffect : public UnitEffect
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
    
    ALLOW_MELEE_ATTACKS_AGAINST_FLYING,
    USE_MANA_POOL_FOR_RANGED_ATTACKS,
    MAGICAL_WEAPONS,
  } effect;
  
  SimpleEffect(UnitEffectType type, Type effect) : UnitEffect(type), effect(effect) { }
};

class SimpleParametricEffect : public SimpleEffect
{
public:
  SimpleParametricEffect(UnitEffectType type, Type effect, value_t param) : SimpleEffect(type, effect), param(param) { }
  const value_t param;
};

struct UnitModifierLevelGetter
{
  value_t operator()(const Unit* unit) const;
};

using UnitModifierValue = Modifier<value_t, Unit, UnitModifierLevelGetter>;

using WizardAttributeModifier = SpecificModifierEffect<UnitEffect, UnitEffectType::WIZARD_BONUS, UnitModifierValue, WizardAttribute>;
using UnitPropertyBonus = SpecificModifierEffect<UnitEffect, UnitEffectType::UNIT_BONUS, UnitModifierValue, Property>;
using ArmyPropertyBonus = SpecificModifierEffect<UnitEffect, UnitEffectType::ARMY_BONUS, UnitModifierValue, Property>;

class CombatBonus : public UnitEffect
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
  
  CombatBonus(Property property, value_t value, Phase trigger, Target target, bool boundToSkill) : UnitEffect(UnitEffectType::COMBAT_BONUS), property(property), value(value), trigger(trigger), target(target), boundToSkill(boundToSkill) { }
  
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

using MovementEffect = EnumEffect<UnitEffect, UnitEffectType::MOVEMENT, MovementType>;
using MovementDisallowEffect = EnumEffect<UnitEffect, UnitEffectType::DISALLOW_MOVEMENT, MovementType>;

enum SpecialAttackType
{
  THROWN_ATTACK,
  
  FIRE_BREATH,
  
  POISON_TOUCH,

  GENERIC
};

class SpecialAttackEffect : public EnumEffect<UnitEffect, UnitEffectType::SPECIAL_ATTACK, SpecialAttackType>
{
public:
  enum class Trigger { ON_MELEE_ATTACK };
  enum class Figure { ON_EACH_FIGURE };

private:

  damage_amount _damage;
  value_t _bonus;
  Trigger _trigger;
  Figure _figure;
  School _school;

public:
  SpecialAttackEffect(SpecialAttackType type, value_t strength) : EnumEffect(type), _damage(strength) { }

  SpecialAttackEffect(damage_amount strength, value_t bonus, Trigger trigger, Figure figure, School school) : EnumEffect(SpecialAttackType::GENERIC),
    _damage(strength), _bonus(bonus), _trigger(trigger), _figure(figure), _school(school) { }
  
  value_t bonus() const { return _bonus; }
  damage_amount damage() const { return _damage; }

  Trigger trigger() const { return _trigger; }
  Figure figure() const { return _figure; }
};

//TODO: technically is a combat instant spell
//TODO: implement mechanics
class Spell;
class SpellGrantEffect : public UnitEffect
{
private:
  const Spell* _spell;
  const value_t _times;
  const value_t _strength;
  
public:
  SpellGrantEffect(const Spell* spell, const value_t times, const value_t strength = 0) : UnitEffect(UnitEffectType::GRANT_SPELL), _spell(spell), _times(times), _strength(strength) { }
  
  const Spell* spell() const { return _spell; }
  value_t times() const { return _times; }
  value_t strength() const { return _strength; }
};
