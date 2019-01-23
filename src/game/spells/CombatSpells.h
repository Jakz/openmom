#ifndef _COMBAT_SPELLS_H_
#define _COMBAT_SPELLS_H_

#include "common/Common.h"
#include "Effects.h"
#include "UnitSpec.h"

#include <string>
#include <functional>
#include <vector>

class SpellCast;

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
  typedef std::function<CastResult(const combat::Combat*, const combat::CombatUnit*, const SpellCast&)> lambda_type;
  CombatSingleUnitEffect(lambda_type effect) : CombatSpellEffect(SINGLE_UNIT), effect(effect) { }
  CastResult apply(const combat::Combat* combat, combat::CombatUnit* unit, const SpellCast& cast) const { return effect(combat,unit,cast); }
  
private:
  const lambda_type effect;
};

class CombatGlobalEffect : public CombatSpellEffect
{
public:
  typedef std::function<CastResult(const combat::Combat*, const SpellCast&)> lambda_type;
  CombatGlobalEffect(lambda_type effect) : CombatSpellEffect(SINGLE_UNIT), effect(effect) { }
  CastResult apply(const combat::Combat* combat, const SpellCast& cast) const { return effect(combat,cast); }

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
  typedef std::function<CastResult(const combat::Combat*, const SpellCast&)> lambda_type;
  CombatPeriodicEnch(lambda_type effect) : CombatEnchEffect(PERIODIC), effect(effect) { }
  CastResult applyEachTurn(const combat::Combat* combat, const SpellCast& cast) const { return effect(combat, cast); }
  
private:
  lambda_type effect;
};

class CombatEnchModifier : public CombatEnchEffect
{
public:
  typedef std::function<CastResult(const combat::Combat*, const Unit*, const SpellCast&, Property property)> lambda_type;
  CombatEnchModifier(std::initializer_list<const UnitBonus*> effects) : CombatEnchEffect(UNIT_MODIFIER), effects(effects) { }
  
  value_t apply(const combat::Combat* combat, const SpellCast& cast, const Unit* unit, Property property) const;
  
private:
  value_t doApply(const Unit* unit, Property property) const
  {
    s16 bonus = 0;
    for (auto e : effects)
      if (e->sameProperty(property))
        bonus += e->getValue(unit);

    return bonus;
  }

  std::vector<const UnitBonus*> effects;
};

class CombatEffects
{
  const CombatSingleUnitEffect HEALING = CombatSingleUnitEffect([](const combat::Combat* combat, const combat::CombatUnit* unit, const SpellCast& cast){
    return CastResult();
  });
  
  const CombatSingleUnitEffect STAR_FIRES = CombatSingleUnitEffect([](const combat::Combat* combat, const combat::CombatUnit* unit, const SpellCast& cast){
    return CastResult();
  });
  
  const CombatSingleUnitEffect DISPEL_EVIL = CombatSingleUnitEffect([](const combat::Combat* combat, const combat::CombatUnit* unit, const SpellCast& cast){
    return CastResult();
  });
  
  const CombatSingleUnitEffect DOOM_BOLT = CombatSingleUnitEffect([](const combat::Combat* combat, const combat::CombatUnit* unit, const SpellCast& cast){
    
    
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
  
  const CombatGlobalEffect FLAME_STRIKE = CombatGlobalEffect([](const combat::Combat* combat, const SpellCast& cast){
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
