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

#include <vector>
#include <stack>

enum class Property : u8;
class Unit;

/*
template<typename T>
class Value
{
private:
  T _base;
  float _multiplier;

public:
  Value(T base, float multiplier) : _base(base), _multiplier(multiplier) { assert(_multiplier >= 0.0f); }
  Value(T base) : Value(base, 1.0f) { }
  Value() : Value(0) { }

  inline Value operator+(T value) const { return Value(_base + value, _multiplier); }
  inline Value operator-(T value) const { return Value(_base - value, _multiplier); }
  inline Value operator*(float multiplier) const { return Value(_base, _multiplier + multiplier); }
  inline Value operator/(float multiplier) const { return Value(_base, _multiplier - multiplier); }

  inline operator T() const { return _base * _multiplier; }
};
*/

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

class SkillModifierEffect : public UnitEffect
{
protected:
  UnitModifierValue _value;
  predicate<const Unit*> _predicate;
  value_t transformValue(value_t value, const Unit* unit) const { return _value.transformValue(value, unit); }

public:
  SkillModifierEffect(UnitEffectType type, UnitModifierValue value) : UnitEffect(type), _value(value), _predicate([](auto unit) { return true; }) { }
  SkillModifierEffect(UnitEffectType type, UnitModifierValue value, predicate<const Unit*> predicate) : UnitEffect(type), _value(value), _predicate(predicate) { }

  const UnitModifierValue& modifier() const { return _value; }
  bool isModifier() const override { return true; }

  Order compare(const Unit* unit, const UnitEffect* other) const override
  {
    //TODO: this doesn't check if kind of modifier is the same so it should be used only when this is sure (eg in an yaml defined SkillGroup) 
    if (other->isModifier())
    {
      return modifier().compareMagnitude(unit, other->as<SkillModifierEffect>()->modifier());
    }
    else
      return Order::UNCOMPARABLE;
  }

  using modifier_type = UnitModifierValue;
};

template<typename EnumType, UnitEffectType SkillType>
class PropertyModifierEffect : public SkillModifierEffect
{
private:
  EnumType _property;

public:
  PropertyModifierEffect(EnumType property, UnitModifierValue value) : SkillModifierEffect(SkillType, value), _property(property) { }
  PropertyModifierEffect(EnumType property, UnitModifierValue value, predicate<const Unit*> predicate) : SkillModifierEffect(SkillType, value, predicate), _property(property) { }

  PropertyModifierEffect(EnumType property, UnitModifierValue value, School school) : PropertyModifierEffect(property, value, [school](const Unit* unit) { return unit->school() == school; }) { }

  bool isAffecting(EnumType property) const { return _property == property; }

  value_t transformValue(EnumType property, value_t value, const Unit* unit) const
  {
    return property == _property ? _value.transformValue(value, unit) : value;
  }

  using property_type = EnumType;
  using skill_type = std::integral_constant<UnitEffectType, SkillType>;
};

using WizardAttributeModifier = PropertyModifierEffect<WizardAttribute, UnitEffectType::WIZARD_BONUS>;
using UnitPropertyBonus = PropertyModifierEffect<Property, UnitEffectType::UNIT_BONUS>;
using ArmyPropertyBonus = PropertyModifierEffect<Property, UnitEffectType::ARMY_BONUS>;

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
  
  POISON_TOUCH
};

class SpecialAttackEffect : public EnumEffect<UnitEffect, UnitEffectType::SPECIAL_ATTACK, SpecialAttackType>
{
private:
  value_t _strength;
  
public:
  SpecialAttackEffect(SpecialAttackType type, value_t strength) : EnumEffect(type), _strength(strength) { }
  value_t strength() const { return _strength; }
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