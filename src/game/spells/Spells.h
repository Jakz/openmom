#ifndef _SPELLS_H_
#define _SPELLS_H_

#include "Common.h"

class Player;

enum SpellRarity : u8
{
  RARITY_COMMON = 0,
  RARITY_UNCOMMON,
  RARITY_RARE,
  RARITY_VERY_RARE,
  
  RARITY_COUNT
};

enum SpellType : u8
{
  
};

enum SpellKind : u8
{
  KIND_SUMMONING = 0,
  KIND_SPECIAL,
  KIND_CITY,
  KIND_ENCHANTMENT,
  KIND_UNIT_SPELL,
  KIND_COMBAT_SPELL,
  
  KIND_COUNT
};

enum SpellDuration : u8
{
  COMBAT_INSTANT,
  CONTINUOUS,
  COMBAT_CONTINUOUS,
  COMBAT_ENCHANTMENT,
  PERMANENT,
  UNDEFINED
};

enum SpellTarget : u8
{
  TARGET_FRIENDLY_UNIT,
  TARGET_ENEMY_UNIT,
  TARGET_FRIENDLY_CITY,
  TARGET_ENEMY_CITY,
  TARGET_ENEMY_UNIT_SPELL,
  TARGET_FRIENDLY_ARMY,
  TARGET_ENEMY_ARMY,
  TARGET_BOTH_ARMIES,
  TARGET_MAP_TILE,
  TARGET_VIEWPORT,
  TARGET_GLOBAL,
  
  TARGET_NONE
};

class Spell
{
  public:
    const I18 name;
    //const SpellType type;
    const SpellRarity rarity;
    const SpellKind kind;
    const SpellDuration duration;
    const School school;
    const SpellTarget target;
    const s16 researchCost;
    const s16 manaCost;
    const s16 manaCostDelta;
    const s16 combatManaCost;
    const s16 combatManaCostDelta;
    const s16 upkeep;
  
  Spell(I18 name, /*SpellType type,*/ SpellRarity rarity, SpellKind kind, SpellDuration duration, School school, SpellTarget target,
            s16 researchCost, s16 manaCost, s16 manaCostDelta, s16 combatManaCost, s16 combatManaCostDelta, s16 upkeep) :
    name(name), /*type(type),*/ rarity(rarity), kind(kind), duration(duration), school(school), target(target),
    researchCost(researchCost), manaCost(manaCost), manaCostDelta(manaCostDelta), combatManaCost(combatManaCost), combatManaCostDelta(combatManaCostDelta), upkeep(upkeep)
  {
    
  }
  
  bool canBeCastInCombat() { return combatManaCost > 0; }
  bool canBeCastInOverland() { return manaCost > 0; }
};

class UnitSpell : public Spell
{
  // TODO
};

class CitySpell : public Spell
{
  public:
    CitySpell(I18 name, SpellRarity rarity, School school, SpellDuration duration, s16 researchCost, s16 manaCost, s16 combatManaCost, s16 upkeep, SpellTarget target) :
    Spell(name, rarity, KIND_CITY, duration, school, target, researchCost, manaCost, -1, combatManaCost, -1, upkeep) { }
};

class CombatEnchSpell : public Spell
{
  // TODO
};



class SpellCast
{
  public:
    const Player* player;
    const Spell& spell;
  
    SpellCast(const Player* player, const Spell& spell) : player(player), spell(spell) { }
  
    const UnitSpell& asUnitSpell() { return static_cast<const UnitSpell&>(spell); }
    const CitySpell& asCitySpell() { return static_cast<const CitySpell&>(spell); }
    const CombatEnchSpell& asCombatEnchSpell() { return static_cast<const CombatEnchSpell&>(spell); }
};

class VariableSpellCast : public SpellCast
{
  public:
    const u16 extraMana;
  
    VariableSpellCast(const Player* player, const Spell& spell, u16 extraMana) : SpellCast(player,spell), extraMana(extraMana) { }
};


#endif
