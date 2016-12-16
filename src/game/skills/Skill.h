#ifndef _SKILL_H_
#define _SKILL_H_

#include "Common.h"

#include "Effects.h"

#include <vector>

// HERO_SKILL -> FIGHTER, MAGE, BOTH
// HERO_SKILL -> manage super skill

// MOVEMENT_SKILL -> shared true/false

enum class SkillBase : u16
{
  HERO_AGILITY = 128,
  HERO_ARCANE_POWER = 129,
  HERO_ARMS_MASTER = 130,
  HERO_BLADE_MASTER = 131,
  HERO_CHARMED = 132,
  HERO_CONSTITUTION = 133,
  HERO_LEADERSHIP = 134,
  HERO_LEGENDARY = 135,
  HERO_LUCK = 136,
  HERO_MIGHT = 137,
  HERO_NOBLE = 138,
  HERO_PRAYER_MASTER = 139,
  HERO_SAGE = 140,
  HERO_SPELL_CASTER = 141,
  
  // NATIVE
  
  FIRST_STRIKE = 10,
	LARGE_SHIELD = 9,
	
	ARMOR_PIERCING = 25,
	NEGATE_FIRST_STRIKE = 26,
	LONG_RANGE = 28,
	
	ILLUSIONARY_ATTACK = 43,
	POISON_ATTACK = 44,
	LIFE_STEALING = 45,
	FIERY_BREATH = 46,
	LIGHTING_BREATH = 47,
	
	DOOM_GAZE = 59,
	DEATH_GAZE = 60,
	STONE_GAZE = 61,
	STONE_TOUCH = 62,
	IMMOLATION = 63,
	THROW_WEAPON = 77,
	
	DISPEL_EVIL = 29,
	INVISIBILITY = 34,
	CAUSE_FEAR = 35,
	WEB_SPELL = 36,
	CREATE_UNDEAD = 39,
	SUMMON_DEMONS = 40,
	CASTER = 141,
	HEALING_SPELL = 41,
	FIRE_BALL_SPELL = 42,
	DOOM_BOLT_SPELL = 42+16,
  
  IMMUNITY_POISON = 16,
  IMMUNITY_FIRE = 17,
  IMMUNITY_STONING = 18,
  IMMUNITY_WEAPON = 19,
  IMMUNITY_MISSILE = 20,
  IMMUNITY_ILLUSIONS = 21,
  IMMUNITY_COLD = 22,
  IMMUNITY_MAGIC = 23,
  IMMUNITY_DEATH = 24,
  
  MELD = 11,
  PURIFY = 27,
  CREATE_OUTPOST = 15,
  CREATE_ROAD = 31,
  WALL_CRUSHING = 30,
  
  SCOUTING = 12, // PARAMETRIC
  TO_HIT = 13, // PARAMETRIC
  LUCKY = 14, // PARAMETRIC
  
  HOLY_BONUS = 32, // PARAMETRIC, NOT CUMULATIVE
  RESISTANCE_TO_ALL = 37, // PARAMETRIC, NOT CUMULATIVE
  HEALER = 38,
  REGENERATION = 33,
  
  MITHRIL_WEAPONS = 176,
  ADAMANTIUM_WEAPONS = 177,
  
  // MOVEMENT
  
  FORESTWALK = 64,
  FLYING = 65,
  UNDERGROUND = 66,
  MOUNTAINWALK = 67,
  NON_CORPOREAL = 68,
  PATH_FINDER = 69,
  PLANAR_TRAVEL = 70,
  TELEPORT = 71,
  SWIMMING = 72,
  WINDWALK = 73,
  SAILING = 74,
  DESERTWALK = 75,
  SWAMPWALK = 76,
  
  
  // SPELLS
  
  // chaos
  SPELL_ELDRITCH_WEAPON = 48-32+192,
  SPELL_FLAME_BLADE = 49-32+192,
  SPELL_CHAOS_CHANNELS_SHIELD = 212,
  SPELL_CHAOS_CHANNELS_WINGS = 213,
  SPELL_CHAOS_CHANNELS_BREATH = 214,
  SPELL_IMMOLATION = 219,
  
  
  // death
  SPELL_CLOAK_OF_FEAR = 32-32+192,
  SPELL_UNDEAD = 50-32+192,
  SPELL_BLACK_CHANNELS = 211,
  SPELL_WRAITH_FORM = 68+16,
  SPELL_BERSERK = 221,
  
  // nature
  SPELL_RESIST_ELEMENTS = 35-32+192,
  SPELL_GIANT_STRENGTH = 33-32+192,
  SPELL_STONE_SKIN = 34-32+192,
  SPELL_SWIMMING = 72+16,
  
  SPELL_PATH_FINDER = 69+16,
  SPELL_REGENERATION = 218,
  SPELL_ELEMENTAL_ARMOR = 36-32+192,
  SPELL_IRON_SKIN = 37-32+192,
  
  // sorcery
  SPELL_RESIST_MAGIC = 38-32+192,
  SPELL_GUARDIAN_WIND = 39-32+192,
  SPELL_IMMUNITY_MAGIC = 215,
  SPELL_SPELL_LOCK = 216,
  SPELL_INVISIBILITY = 217,
  SPELL_WINDWALK = 89,
  SPELL_HASTE = 220,
  
  SPELL_FLYING = 65+16,
  
  // life
  SPELL_HOLY_WEAPON = 200,
  SPELL_BLESS = 201,
  SPELL_ENDURANCE = 202,  //*
  SPELL_TRUE_SIGHT = 203, //*
  SPELL_HEROISM = 204, //*
  SPELL_HOLY_ARMOR = 205, //*
  SPELL_LION_HEART = 206, //*
  SPELL_PLANAR_TRAVEL = 86, //*
  SPELL_INVULNERABILITY = 207, //*
  SPELL_RIGHTEOUSNESS = 223, //*
  

  //ITEMS
  // chaos
  
  // death
  ITEM_CLOAK_OF_FEAR = 122,
  //DEATH
  //POWER_DRAIN
  //VAMPIRIC
  //WRAITH_FORM
  
  // nature
  ITEM_RESIST_ELEMENTS = 112,
  ITEM_ELEMENTAL_ARMOR = 114,
  ITEM_GIANT_STRENGTH = 123,
  // PETRIFYING
  ITEM_PATH_FINDER = 101,
  ITEM_REGENERATION = 124,
  ITEM_SWIMMING = 104,
  
  // sorcery
  ITEM_FLYING = 97,
  ITEM_RESIST_MAGIC = 113,
  ITEM_IMMUNITY_MAGIC = 115,
  ITEM_GUARDIAN_WIND = 125,
  ITEM_HASTE = 127,
  
  
  // life
  ITEM_BLESS = 116,
  ITEM_ENDURANCE = 117,
  ITEM_HOLY_AVENGER = 126,
  ITEM_INVULNERABILITY = 118,
  ITEM_LION_HEART = 119,
  ITEM_PLANAR_TRAVEL = 102,
  ITEM_RIGHTEOUSNESS = 120,
  ITEM_TRUE_SIGHT = 121,
  
  
  // STATUS
  STATUS_WEB = 224,
  STATUS_CONFUSION = 225,
  STATUS_VERTIGO = 226,
  STATUS_HIT_PENALTY = 227,
  STATUS_SHATTER = 228,
  STATUS_WARP_CREATURE_DEFENSE = 229,
  STATUS_WARP_CREATURE_ATTACK = 230,
  STATUS_WARP_CREATURE_MAGIC_DEFENSE = 231,
  STATUS_BLACK_SLEEP = 232,
  STATUS_WEAKNESS = 233,
  STATUS_POSESSION = 234
};

class Skill
{
public:
  Skill(SkillBase base) : base(base) { }
  
  
  const SkillBase base;

  virtual const std::string name() const;
  virtual effect_list& getEffects() const = 0;
  
  virtual bool is(SkillBase base) const { return this->base == base; }
  
  
  struct Comparator : public std::binary_function<const Skill*, const Skill*, bool>
  {
    bool operator() (const Skill *b1, const Skill* b2);
  } comparator;
  
  static bool isHeroBase(SkillBase base);
  static bool isMovementBase(SkillBase base);
};

class ConcreteSkill : public Skill
{
private:
  effect_list effects;
  
public:
  bool hideValue;
  
  ConcreteSkill(SkillBase base, effect_init_list effects, bool hideValue = true) : Skill(base), effects(effects), hideValue(hideValue) { }
  ConcreteSkill(SkillBase base, effect_list effects, bool hideValue = true) : Skill(base), effects(effects), hideValue(hideValue) { }

  
  effect_list& getEffects() const override { return effects; }
  
  bool hasSimpleEffect(SimpleEffect::Type type);
  const SpecialAttackEffect* hasEffect(SimpleEffect::Type ident);
  
  const std::string name() const override;
};

class WrapSkill : public Skill
{
private:
  const Skill& existingSkill;

public:
  WrapSkill(SkillBase base, const Skill& existingSkill) : Skill(base), existingSkill(existingSkill) { }
  
  effect_list& getEffects() const override { return existingSkill.getEffects(); }
  bool is(SkillBase) const override { return this->base == base || existingSkill.base == base; }
};

using skill_init_list = std::initializer_list<const Skill*>;

struct skill_list
{
private:
  std::vector<const Skill*> skills;
  
public:
  using iterator = decltype(skills)::iterator;
  using const_iterator = decltype(skills)::const_iterator;
  
  skill_list() : skills() { }
  skill_list(const skill_init_list& skills) : skills(skills) { }
  
  void push_back(const Skill* skill) { skills.push_back(skill); }
  
  size_t size() const { return skills.size(); }
  const Skill* operator[](size_t index) const { return skills[index]; }
  
  iterator begin() { return skills.begin(); }
  iterator end() { return skills.end(); }
  
  const_iterator begin() const { return skills.begin(); }
  const_iterator end() const { return skills.end(); }

  bool hasSkill(SkillBase base) const
  {
    return std::find_if(skills.begin(), skills.end(), [&](const Skill* c) { return c->base == base; }) != skills.end();
  }
};



// ([A-Z_]+)\(([0-9+\-]+)\),   $1 = $2

class Skills
{
public:
  static const Skill *MELD, *CREATE_OUTPOST, *CREATE_ROAD, *WALL_CRUSHING, *PURIFY;
  
  static const Skill *LUCKY, *RESISTANCE_TO_ALL;
  
  static const Skill *LARGE_SHIELD, *FIRST_STRIKE, *NEGATE_FIRST_STRIKE, *ARMOR_PIERCING, *LONG_RANGE, *ILLUSIONARY_ATTACK;
  
  static const Skill *POISON_ATTACK1, *POISON_ATTACK4, *POISON_ATTACK6, *POISON_ATTACK15, *IMMOLATION, *FIERY_BREATH5, *FIERY_BREATH20;
  static const Skill *LIFE_STEALING0, *LIFE_STEALING3, *LIFE_STEALING4, *LIFE_STEALING5, *LIGHTING_BREATH20;
  static const Skill *DOOM_GAZE4, *DEATH_GAZE2, *DEATH_GAZE4, *STONE_GAZE1, *STONE_GAZE3, *STONE_GAZE4, *STONE_TOUCH3, *THROW_WEAPON1, *THROW_WEAPON2;
  
  static const Skill *SCOUTING1, *SCOUTING2, *SCOUTING3, *TO_HIT1, *TO_HIT2, *TO_HIT3, *MITHRIL_WEAPONS, *ADAMANTIUM_WEAPONS;

  static const Skill *FORESTWALK, *FLYING, *UNDERGROUND, *MOUNTAINWALK, *NON_CORPOREAL, *PATH_FINDER, *PLANAR_TRAVEL, *TELEPORT, *SWIMMING, *WINDWALK, *SAILING, *DESERTWALK, *SWAMPWALK;
  
  static const Skill *IMMUNITY_MISSILE, *IMMUNITY_MAGIC, *IMMUNITY_ILLUSIONS;
  
  
  static const Skill *HERO_AGILITY, *HERO_ARMS_MASTER, *HERO_BLADE_MASTER, *HERO_CHARMED, *HERO_CONSTITUTION, *HERO_LUCK, *HERO_MIGHT, *HERO_PRAYER_MASTER;
  
  static const Skill *SPELL_ELDRITCH_WEAPON, *SPELL_FLAME_BLADE;
  static const Skill *SPELL_RESIST_ELEMENTS, *SPELL_GIANT_STRENGTH, *SPELL_STONE_SKIN, *SPELL_SWIMMING, *SPELL_PATH_FINDER, *SPELL_ELEMENTAL_ARMOR, *SPELL_IRON_SKIN;
  static const Skill *SPELL_RESIST_MAGIC, *SPELL_GUARDIAN_WIND, *SPELL_FLYING, *SPELL_BLESS, *SPELL_ENDURANCE, *SPELL_HEROISM, *SPELL_HOLY_ARMOR, *SPELL_HOLY_WEAPON, *SPELL_PLANAR_TRAVEL, *SPELL_TRUE_SIGHT;
    
};

#endif
