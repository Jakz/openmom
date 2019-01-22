#ifndef _SPELLS_H_
#define _SPELLS_H_

#include "common/Common.h"

#include "CombatSpells.h"

#include <list>


class Player;
class Skill;



enum class SpellRarity : u32
{
  COMMON = 0,
  UNCOMMON,
  RARE,
  VERY_RARE,
  
  COUNT,
  
  NONE
};

enum class SpellType : u8
{
  CITY = 0,
  GLOBAL,
  GLOBAL_SKILL,
  UNIT_SKILL,
  SUMMON,
  UNKNOWN,
  
  SPECIAL,
  
  COMBAT_ENCHANT,
  COMBAT_INSTANT
};

enum class SpellKind : u8
{
  SUMMONING = 0,
  SPECIAL,
  CITY,
  ENCHANTMENT,
  UNIT_SPELL,
  COMBAT_SPELL,
  
  COUNT,
  
  NONE
};

enum class SpellDuration : u8
{
  COMBAT_INSTANT = 0,
  CONTINUOUS,
  COMBAT_ENCHANTMENT,
  PERMANENT,
  UNDEFINED
};

enum class Target : u8
{
  FRIENDLY_UNIT = 0,
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
  const value_t researchCost;
  const value_t manaCost;
  const value_t manaCostDelta;
  const value_t combatManaCost;
  const value_t combatManaCostDelta;
  const value_t upkeep;
  
  ManaInfo(value_t rc, value_t mc, value_t mcd, value_t cmc, value_t cmcd, value_t upk) :
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
  const help_ref help;
  
  Spell(I18 name, SpellType type, SpellRarity rarity, SpellKind kind, SpellDuration duration, School school, Target target, const ManaInfo mana);
  
  bool canBeCastInCombat() const { return mana.combatManaCost > 0; }
  bool canBeCastInOverland() const { return mana.manaCost > 0; }
  
  template <typename T> const T* as() const { return static_cast<const T*>(this); }
};



class UnitSpell : public Spell
{
public:
	UnitSpell(I18 name, SpellRarity rarity, School school, s16 researchCost , s16 manaCost, s16 combatManaCost, s16 upkeep, const Skill* skill) :
  Spell(name,SpellType::UNIT_SKILL,rarity, SpellKind::UNIT_SPELL, SpellDuration::CONTINUOUS, school,Target::FRIENDLY_UNIT, ManaInfo{researchCost,manaCost,-1,combatManaCost,-1,upkeep}), skill(skill) { }

  const Skill* skill;
};

class CitySpell : public Spell
{
public:
  CitySpell(I18 name, SpellRarity rarity, School school, SpellDuration duration, Target target, s16 researchCost, s16 manaCost, s16 combatManaCost, s16 upkeep) :
  Spell(name, SpellType::CITY, rarity, SpellKind::CITY, duration, school, target, ManaInfo{researchCost, manaCost, -1, combatManaCost, -1, upkeep}) { }
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
    GlobalSpell(name, SpellType::GLOBAL_SKILL, rarity, SpellKind::ENCHANTMENT, school, duration, Target::GLOBAL, researchCost, manaCost, upkeep), skill(skill) { }
  
  const Skill* skill;
};

class StaticGlobalSpell : public GlobalSpell
{
public:
  StaticGlobalSpell(I18 name, SpellRarity rarity, School school, SpellDuration duration, s16 researchCost, s16 manaCost, s16 upkeep) :
    GlobalSpell(name, SpellType::GLOBAL, rarity, SpellKind::ENCHANTMENT, school, duration, Target::GLOBAL, researchCost, manaCost, upkeep) { }
};

class CombatEnchSpell : public Spell
{
public:
  CombatEnchSpell(I18 name, SpellRarity rarity, School school, s16 researchCost, s16 combatManaCost, s16 combatManaCostDelta, Target target, const CombatEnchEffect& effect) :
  Spell(name, SpellType::COMBAT_INSTANT, rarity, SpellKind::ENCHANTMENT, SpellDuration::COMBAT_ENCHANTMENT, school, target, {researchCost, -1, -1, combatManaCost, combatManaCostDelta, 0}), effect(effect) { }
  
  const CombatEnchEffect& effect;
};

class CombatSpell : public Spell
{
public:
  CombatSpell(I18 name, SpellRarity rarity, School school, SpellDuration duration, s16 researchCost, s16 combatManaCost, s16 combatManaCostDelta, Target target, const CombatSpellEffect& effect) :
  Spell(name, SpellType::COMBAT_INSTANT, rarity, SpellKind::COMBAT_SPELL, duration, school, target, {researchCost, -1, -1, combatManaCost, combatManaCostDelta, 0}), effect(effect) { }
  
  const CombatSpellEffect& effect;
};

class SpecialSpell : public Spell
{
public:
  SpecialSpell(I18 name, SpellRarity rarity, School school, SpellDuration duration, s16 researchCost, s16 manaCost, s16 manaCostDelta, Target target) :
    Spell(name, SpellType::SPECIAL, rarity, SpellKind::SPECIAL, duration, school, target, {researchCost, manaCost, manaCostDelta, -1, -1, 0}) { }
  
  SpecialSpell(I18 name, SpellRarity rarity, School school, SpellDuration duration, s16 researchCost, s16 manaCost, s16 manaCostDelta, s16 combatManaCost, s16 combatManaCostDelta, Target target) :
    Spell(name, SpellType::SPECIAL, rarity, SpellKind::SPECIAL, duration, school, target, {researchCost, manaCost, manaCostDelta, combatManaCost, combatManaCostDelta, 0}) { } // TODO: will upkeep be always 0?
};

class SummonSpell : public Spell
{
public:
  SummonSpell(I18 name, SpellRarity rarity, School school, s16 researchCost, s16 manaCost, s16 combatManaCost, const SummonSpec* unit) :
  Spell(name, SpellType::SUMMON, rarity, SpellKind::SUMMONING, SpellDuration::UNDEFINED, school, Target::NONE, {researchCost, manaCost, -1, combatManaCost, -1, 0}), unit(unit) { }

  const SummonSpec* unit;
};

class SpellCaster
{
  enum class Type { WIZARD, UNIT };
  
  Type type;
  union { const Player* _player; const Unit* _unit; };
  
public:
  SpellCaster(const Player* player) : type(Type::WIZARD), _player(player) { }
  SpellCaster(const Unit* unit) : type(Type::UNIT), _unit(unit) { }
  bool isWizard() const { return type == Type::WIZARD; }
  
  const Player* player() const { return _player; }
  const Unit* unit() const { return _unit; }
};

class SpellCast
{
private:
  SpellCaster _caster;
  
public:
  const Spell* spell;
  value_t extraMana;
  bool isVariable;
  bool castInCombat;

  SpellCast(const Player* player, const Spell* spell, u16 extraMana = 0) : SpellCast(player, spell, false, extraMana) { }
  SpellCast(const Player* player, const Spell* spell, bool castInCombat, u16 extraMana = 0) : _caster(player), spell(spell), extraMana(extraMana), isVariable(extraMana > 0), castInCombat(castInCombat) { }
  
  bool operator==(const Spell* spell) const { return this->spell == spell; }

  template<typename T> const T* as() const { return static_cast<const T*>(spell); }

  value_t totalMana() const { return (castInCombat ? spell->mana.combatManaCost : spell->mana.manaCost) + extraMana; }
  
  const SpellCaster& caster() const { return _caster; }
  
  const UnitSpell* asUnitSpell() const { return as<UnitSpell>(); }
  const CitySpell* asCitySpell() const { return as<CitySpell>(); }
  const CombatEnchSpell* asCombatEnchSpell() { return spell->type == SpellType::COMBAT_ENCHANT ? as<CombatEnchSpell>() : nullptr; }
};


struct ResearchStatus
{
  const Spell* spell;
  bool discovered;
  
  ResearchStatus(const Spell* spell, bool discovered = false) : spell(spell), discovered(discovered) { }
};

typedef std::vector<SpellCast> cast_list;
typedef std::vector<const Spell*> spell_list;

class Spells
{
public:
  static const std::vector<SpellKind>& spellKinds(bool combat);
  static spell_list& spellsByRarityAndSchool(SpellRarity rarity, School school);
  
  static const Spell *ENDURANCE;
  
  
  
  static const Spell* CORRUPTION;
  static const Spell* CHANGE_TERRAIN;
  static const Spell* RAISE_VOLCANO;
  
};



#endif
