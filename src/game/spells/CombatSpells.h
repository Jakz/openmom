#ifndef _COMBAT_SPELLS_H_
#define _COMBAT_SPELLS_H_

#include "Common.h"
#include "Effects.h"
#include "UnitSpec.h"

#include <string>
#include <functional>
#include <vector>

class CombatUnit;
class Combat;
class SpellCast;
class Unit;

class CombatUnit
{
  
public:
  Unit* unit;
};

struct CastResult
{
  const bool success;
  const std::string message;
  
  CastResult() : success(true) { }
  CastResult(std::string message) : success(false), message(message) { }
};

class CombatSpellEffect
{
public:
  const enum Type
  {
    SINGLE_UNIT,
    GLOBAL
  } type;
protected:
  CombatSpellEffect(Type type) : type(type) { }
};


class CombatSingleUnitEffect : public CombatSpellEffect
{
public:
  typedef std::function<CastResult(Combat*, CombatUnit*, const SpellCast&)> lambda_type;
  CombatSingleUnitEffect(lambda_type effect) : CombatSpellEffect(SINGLE_UNIT), effect(effect) { }
  CastResult apply(Combat* combat, CombatUnit* unit, const SpellCast& cast) { return effect(combat,unit,cast); }
  
private:
  const lambda_type effect;
};

class CombatGlobalEffect : public CombatSpellEffect
{
public:
  typedef std::function<CastResult(Combat*, const SpellCast&)> lambda_type;
  CombatGlobalEffect(lambda_type effect) : CombatSpellEffect(SINGLE_UNIT), effect(effect) { }
  CastResult apply(Combat* combat, const SpellCast& cast) { return effect(combat,cast); }

private:
  const lambda_type effect;
};

class CombatEnchEffect
{
public:
  const enum Type
  {
    PERIODIC,
    UNIT_MODIFIER
  } type;
protected:
  CombatEnchEffect(Type type) : type(type) { }
};

class CombatPeriodicEnch : public CombatEnchEffect
{
public:
  typedef std::function<CastResult(Combat*, const SpellCast&)> lambda_type;
  CombatPeriodicEnch(lambda_type effect) : CombatEnchEffect(PERIODIC), effect(effect) { }
  CastResult applyEachTurn(Combat* combat, const SpellCast& cast) { return effect(combat, cast); }
  
private:
  lambda_type effect;
};

class CombatEnchModifier : public CombatEnchEffect
{
public:
  typedef std::function<CastResult(Combat*, CombatUnit*, const SpellCast&, Property property)> lambda_type;
  CombatEnchModifier(std::initializer_list<const UnitBonus*> effects) : CombatEnchEffect(UNIT_MODIFIER), effects(effects) { }
  
  s16 apply(Combat* combat, const SpellCast& cast, CombatUnit* unit, Property property);
  
private:
  s16 doApply(CombatUnit* unit, Property property)
  {
    s16 bonus = 0;
    for (auto e : effects)
      if (e->sameProperty(property))
        bonus += e->getValue(unit->unit);

    return bonus;
  }

  std::vector<const UnitBonus*> effects;
};

class CombatEffects
{
  const CombatSingleUnitEffect HEALING = CombatSingleUnitEffect([](Combat* combat, CombatUnit* unit, const SpellCast& cast){
    return CastResult();
  });
  
  const CombatSingleUnitEffect STAR_FIRES = CombatSingleUnitEffect([](Combat* combat, CombatUnit* unit, const SpellCast& cast){
    return CastResult();
  });
  
  const CombatSingleUnitEffect DISPEL_EVIL = CombatSingleUnitEffect([](Combat* combat, CombatUnit* unit, const SpellCast& cast){
    return CastResult();
  });
  
  const CombatSingleUnitEffect DOOM_BOLT = CombatSingleUnitEffect([](Combat* combat, CombatUnit* unit, const SpellCast& cast){
    // TODO
    /*
     if (unit.unit.hasSkill(SkillID.Immunity.MAGIC))
     {
     // TODO: vero messaggio
     return new CastResult("BLA BLA");
     }
     else
     {
     // TODO: modifiers al to damage?
     unit.unit.applyDamage(10);
     return new CastResult();
     }
     */
    return CastResult();
  });
  
  const CombatGlobalEffect FLAME_STRIKE = CombatGlobalEffect([](Combat* combat, const SpellCast& cast){
    //List<CombatUnit> affected = combat.enemyUnits(cast.owner);
    
    // TODO: damage effect
    
    // TODO: push animation
    
    
    // TODO: manage fizzles
    
    
    return CastResult();
  });
  
  
  
  
  
  const CombatEnchModifier TRUE_LIGHT = CombatEnchModifier({
    new FilterUnitBonus(Property::MELEE, 1, LIFE),
    new FilterUnitBonus(Property::RANGED, 1, LIFE),
    new FilterUnitBonus(Property::SHIELDS, 1, LIFE),
    new FilterUnitBonus(Property::RESIST, 1, LIFE),
    new FilterUnitBonus(Property::MELEE, -1, DEATH),
    new FilterUnitBonus(Property::RANGED, -1, DEATH),
    new FilterUnitBonus(Property::SHIELDS, -1, DEATH),
    new FilterUnitBonus(Property::RESIST, -1, DEATH)
  });
  
  const CombatEnchModifier PRAYER = CombatEnchModifier({
    new UnitBonus(Property::TO_HIT, 1),
    new UnitBonus(Property::TO_DEFEND, 1),
    new UnitBonus(Property::RESIST, 1)
  });
};


#endif
