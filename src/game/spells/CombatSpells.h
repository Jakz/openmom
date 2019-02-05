#ifndef _COMBAT_SPELLS_H_
#define _COMBAT_SPELLS_H_

#include "common/Common.h"
#include "Effects.h"
#include "UnitSpec.h"
//#include "Spells.h"

#include <string>
#include <functional>
#include <vector>

struct CastResult
{
  const bool success;
  const std::string message;
  
  CastResult() : success(true) { }
  CastResult(std::string message) : success(false), message(message) { }
};

class CombatEnchSpell;
template<typename T> class SpellCast;
using CombatEnchCast = SpellCast<CombatEnchSpell>;

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
  typedef std::function<CastResult(const combat::Combat*, const combat::CombatUnit*, const Cast&)> lambda_type;
  CombatSingleUnitEffect(lambda_type effect) : CombatSpellEffect(SINGLE_UNIT), effect(effect) { }
  CastResult apply(const combat::Combat* combat, combat::CombatUnit* unit, const Cast& cast) const { return effect(combat,unit,cast); }
  
private:
  const lambda_type effect;
};

class CombatGlobalEffect : public CombatSpellEffect
{
public:
  using lambda_type = std::function<CastResult(const combat::Combat*, const CombatEnchCast&)>;
  CombatGlobalEffect(lambda_type effect) : CombatSpellEffect(SINGLE_UNIT), effect(effect) { }
  CastResult apply(const combat::Combat* combat, const CombatEnchCast& cast) const { return effect(combat,cast); }

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
  typedef std::function<CastResult(const combat::Combat*, const CombatEnchCast&)> lambda_type;
  CombatPeriodicEnch(lambda_type effect) : CombatEnchEffect(PERIODIC), effect(effect) { }
  CastResult applyEachTurn(const combat::Combat* combat, const CombatEnchCast& cast) const { return effect(combat, cast); }
  
private:
  lambda_type effect;
};

class CombatEnchModifier : public CombatEnchEffect
{
public:
  typedef std::function<CastResult(const combat::Combat*, const Unit*, const CombatEnchCast&, Property property)> lambda_type;
  CombatEnchModifier(std::initializer_list<const UnitPropertyBonus*> effects) : CombatEnchEffect(UNIT_MODIFIER), effects(effects) { }
  
  value_t apply(const combat::Combat* combat, const CombatEnchCast& cast, const Unit* unit, Property property) const;
  
private:
  value_t doApply(const Unit* unit, Property property) const
  {
    value_t value = 0;
    for (const auto effect : effects)
      value = effect->transformValue(property, value, unit);

    return value;
  }

  std::vector<const UnitPropertyBonus*> effects;
};

class CombatEffects
{
  const CombatSingleUnitEffect HEALING = CombatSingleUnitEffect([](const combat::Combat* combat, const combat::CombatUnit* unit, const Cast& cast){
    return CastResult();
  });
  
  const CombatSingleUnitEffect STAR_FIRES = CombatSingleUnitEffect([](const combat::Combat* combat, const combat::CombatUnit* unit, const Cast& cast){
    return CastResult();
  });
  
  const CombatSingleUnitEffect DISPEL_EVIL = CombatSingleUnitEffect([](const combat::Combat* combat, const combat::CombatUnit* unit, const Cast& cast){
    return CastResult();
  });
  
  const CombatSingleUnitEffect DOOM_BOLT = CombatSingleUnitEffect([](const combat::Combat* combat, const combat::CombatUnit* unit, const Cast& cast){
    
    
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
  
  const CombatGlobalEffect FLAME_STRIKE = CombatGlobalEffect([](const combat::Combat* combat, const auto& cast){
    //List<CombatUnit> affected = combat.enemyUnits(cast.owner);
    
    // TODO: damage effect
    
    // TODO: push animation
    
    
    // TODO: manage fizzles
    
    
    return CastResult();
  });
  
  
  /*
  predicate<const Unit*> unitSchoolPredicate(School school) { return [school](const Unit* unit) { return unit->school() == school; }; }
  
  
  const CombatEnchModifier TRUE_LIGHT = CombatEnchModifier({
    new UnitPropertyBonus(Property::MELEE, 1, unitSchoolPredicate(School::LIFE)),
    new UnitPropertyBonus(Property::RANGED, 1, unitSchoolPredicate(LIFE)),
    new UnitPropertyBonus(Property::SHIELDS, 1, unitSchoolPredicate(LIFE)),
    new UnitPropertyBonus(Property::RESIST, 1, unitSchoolPredicate(LIFE)),
    new UnitPropertyBonus(Property::MELEE, -1, unitSchoolPredicate(DEATH)),
    new UnitPropertyBonus(Property::RANGED, -1, unitSchoolPredicate(DEATH)),
    new UnitPropertyBonus(Property::SHIELDS, -1, unitSchoolPredicate(DEATH)),
    new UnitPropertyBonus(Property::RESIST, -1, unitSchoolPredicate(DEATH))
  });
  
  const CombatEnchModifier PRAYER = CombatEnchModifier({
    new UnitPropertyBonus(Property::TO_HIT, 1),
    new UnitPropertyBonus(Property::TO_DEFEND, 1),
    new UnitPropertyBonus(Property::RESIST, 1)
  });*/
};

#endif
