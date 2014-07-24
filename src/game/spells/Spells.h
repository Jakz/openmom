#ifndef _SPELLS_H_
#define _SPELLS_H_

#include "Common.h"

#include "CombatSpells.h"

#include <list>


class Player;
class Skill;



enum SpellRarity : u8
{
  RARITY_COMMON = 0,
  RARITY_UNCOMMON,
  RARITY_RARE,
  RARITY_VERY_RARE,
  
  RARITY_COUNT
};

enum class SpellType : u8
{
  CITY,
  GLOBAL,
  GLOBAL_SKILL,
  UNIT_SKILL,
  SUMMON,
  UNKNOWN,
  
  SPECIAL,
  
  COMBAT_ENCHANT,
  COMBAT_INSTANT
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

enum class Target : u8
{
  FRIENDLY_UNIT,
  ENEMY_UNIT,
  FRIENDLY_CITY,
  ENEMY_CITY,
  ENEMY_UNIT_SPELL,
  FRIENDLY_ARMY,
  ENEMY_ARMY,
  BOTH_ARMIES,
  MAP_TILE,
  VIEWPORT,
  GLOBAL,
  
  NONE
};

struct ManaInfo
{
  const s16 researchCost;
  const s16 manaCost;
  const s16 manaCostDelta;
  const s16 combatManaCost;
  const s16 combatManaCostDelta;
  const s16 upkeep;
  
  ManaInfo(s16 rc, s16 mc, s16 mcd, s16 cmc, s16 cmcd, s16 upk) :
  researchCost(rc), manaCost(mc), manaCostDelta(mcd), combatManaCost(cmc), combatManaCostDelta(cmcd), upkeep(upk) { }
};

class Spell
{
  public:
    const I18 name;
    const SpellType type;
    const SpellRarity rarity;
    const SpellKind kind;
    const SpellDuration duration;
    const School school;
    const Target target;
    const ManaInfo mana;
  
  Spell(I18 name, SpellType type, SpellRarity rarity, SpellKind kind, SpellDuration duration, School school, Target target, const ManaInfo mana) :
    name(name), type(type), rarity(rarity), kind(kind), duration(duration), school(school), target(target), mana(mana)
  {
    
  }
  
  bool canBeCastInCombat() const { return mana.combatManaCost > 0; }
  bool canBeCastInOverland() const { return mana.manaCost > 0; }
};



class UnitSpell : public Spell
{
public:
	UnitSpell(I18 name, SpellRarity rarity, School school, SpellDuration duration, s16 researchCost , s16 manaCost, s16 combatManaCost, s16 upkeep, const Skill* skill) :
    Spell(name,SpellType::UNIT_SKILL,rarity,KIND_UNIT_SPELL,duration,school,Target::FRIENDLY_UNIT, ManaInfo{researchCost,manaCost,-1,combatManaCost,-1,upkeep}), skill(skill) { }

  const Skill* skill;
};

class CitySpell : public Spell
{
public:
  CitySpell(I18 name, SpellRarity rarity, School school, SpellDuration duration, Target target, s16 researchCost, s16 manaCost, s16 combatManaCost, s16 upkeep) :
  Spell(name, SpellType::CITY, rarity, KIND_CITY, duration, school, target, ManaInfo{researchCost, manaCost, -1, combatManaCost, -1, upkeep}) { }
};

class GlobalSpell : public Spell
{
protected:
  GlobalSpell(I18 name, SpellType type, SpellRarity rarity, SpellKind kind, School school, SpellDuration duration, Target target, s16 researchCost, s16 manaCost, s16 upkeep) :
  Spell(name, type, rarity, kind, duration, school, target, ManaInfo{researchCost, manaCost, -1, -1, -1, upkeep}) { }
};

class SkillGlobalSpell : public GlobalSpell
{
public:
  SkillGlobalSpell(I18 name, SpellRarity rarity, School school, SpellDuration duration, s16 researchCost, s16 manaCost, s16 upkeep, const Skill* skill) :
    GlobalSpell(name, SpellType::GLOBAL_SKILL, rarity, KIND_ENCHANTMENT, school, duration, Target::GLOBAL, researchCost, manaCost, upkeep), skill(skill) { }
  
  const Skill* skill;
};

class StaticGlobalSpell : public GlobalSpell
{
public:
  StaticGlobalSpell(I18 name, SpellRarity rarity, School school, SpellDuration duration, s16 researchCost, s16 manaCost, s16 upkeep) :
    GlobalSpell(name, SpellType::GLOBAL, rarity, KIND_ENCHANTMENT, school, duration, Target::GLOBAL, researchCost, manaCost, upkeep) { }
};

class CombatEnchSpell : public Spell
{
public:
  CombatEnchSpell(I18 name, SpellRarity rarity, School school, s16 researchCost, s16 combatManaCost, s16 combatManaCostDelta, Target target, const CombatEnchEffect& effect) :
  Spell(name, SpellType::COMBAT_INSTANT, rarity, KIND_ENCHANTMENT, COMBAT_ENCHANTMENT, school, target, {researchCost, -1, -1, combatManaCost, combatManaCostDelta, 0}), effect(effect) { }
  
  const CombatEnchEffect& effect;
};

class CombatSpell : public Spell
{
public:
  CombatSpell(I18 name, SpellRarity rarity, School school, SpellDuration duration, s16 researchCost, s16 combatManaCost, s16 combatManaCostDelta, Target target, const CombatSpellEffect& effect) :
  Spell(name, SpellType::COMBAT_INSTANT, rarity, KIND_COMBAT_SPELL, duration, school, target, {researchCost, -1, -1, combatManaCost, combatManaCostDelta, 0}), effect(effect) { }
  
  const CombatSpellEffect& effect;
};

class SpecialSpell : public Spell
{
public:
  SpecialSpell(I18 name, SpellRarity rarity, School school, SpellDuration duration, s16 researchCost, s16 manaCost, s16 manaCostDelta, Target Target) :
    Spell(name, SpellType::SPECIAL, rarity, KIND_SPECIAL, duration, school, target, {researchCost, manaCost, manaCostDelta, -1, -1, 0}) { }
  
  SpecialSpell(I18 name, SpellRarity rarity, School school, SpellDuration duration, s16 researchCost, s16 manaCost, s16 manaCostDelta, s16 combatManaCost, s16 combatManaCostDelta, Target Target) :
    Spell(name, SpellType::SPECIAL, rarity, KIND_SPECIAL, duration, school, target, {researchCost, manaCost, manaCostDelta, combatManaCost, combatManaCostDelta, 0}) { } // TODO: will upkeep be always 0=
};

class SummonSpell : public Spell
{
public:
  SummonSpell(I18 name, SpellRarity rarity, School school, s16 researchCost, s16 manaCost, s16 combatManaCost, const SummonSpec* spec) :
    Spell(name, SpellType::SUMMON, rarity, KIND_SUMMONING, UNDEFINED, school, Target::NONE, {researchCost, manaCost, -1, combatManaCost, -1, 0}) { }

  const SummonSpec* spec;
};


class SpellCast
{
  public:
    const Player* player;
    const Spell* spell;
    const u16 extraMana;
    const bool isVariable;
  
    SpellCast(const Player* player, const Spell* spell, u16 extraMana = 0) : player(player), spell(spell), extraMana(extraMana), isVariable(extraMana > 0) { }
  
    const UnitSpell* asUnitSpell() const { return static_cast<const UnitSpell*>(spell); }
    const CitySpell* asCitySpell() const { return static_cast<const CitySpell*>(spell); }
    const CombatEnchSpell* asCombatEnchSpell() { return static_cast<const CombatEnchSpell*>(spell); }
};


struct ResearchStatus
{
  const Spell* spell;
  bool discovered;
  
  ResearchStatus(const Spell* spell, bool discovered = false) : spell(spell), discovered(discovered) { }
};

typedef std::list<const SpellCast> cast_list;
typedef std::vector<const Spell*> spell_list;

class Spells
{
public:
  static spell_list spellsByRarityAndSchool(SpellRarity rarity, School school) { return spell_list(); } // TODO
  
  
  
  static const Spell* CORRUPTION;
  static const Spell* CHANGE_TERRAIN;
  static const Spell* RAISE_VOLCANO;
  
};



#endif
