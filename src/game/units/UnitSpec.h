//
//  Unitspec.h
//  OpenMoM
//
//  Created by Jack on 7/21/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _UNIT_SPEC_H_
#define _UNIT_SPEC_H_

#include "common/Common.h"
#include "Buildings.h"
#include "Skill.h"
#include "Race.h"
#include "Items.h"

#include <vector>


enum class UnitID : u16
{
  SPEARMEN,
	SWORDSMEN,
	BOWMEN,
	CAVALRY,
	SHAMANS,
	PRIESTS,
	MAGICIANS,
	PIKEMEN,
	HALBERDIERS,
	ENGINEERS,
  SETTLERS,
	
	GALLEY,
	TRIREME,
	WARSHIP,
	CATAPULT,
	
	BERSERKERS,
	ELVEN_LORDS,
	MINOTAURS,
	GOLEM,
	
	STAG_BEETLE,
	DRAGON_TURTLE,
	WAR_MAMMOTHS,
	
	PALADINS,
	
	SLINGERS,
	
	RANGERS,
	MANTICORES,
	
	LONGBOWMEN,
	
	DOOM_DRAKES,
	WOLF_RIDERS,
	CENTAURS,
	HORSEBOWMEN,
	
	PEGASAI,
	GRIFFINS,
	WYVERN_RIDERS,
	NIGHTMARES,
	
	JAVELINEERS,
	NIGHTBLADES,
	HAMMERHANDS,
	WAR_TROLLS,
	
	STEAM_CANNON,
	
	AIR_SHIP,
	
	WARLOCKS,
  
  
  
  
  
  MAGIC_SPIRIT,
  
  GUARDIAN_SPIRIT,
  
  HELL_HOUNDS,
  GREAT_DRAKE,

  
  HERO_DWARF,
  HERO_ORC_WARRIOR
};

enum class Ranged : u16
{
  NONE = 0,
  ROCK,
  ARROW,
  BULLET,
  CHAOS,
  DEATH,
  LIFE,
  NATURE,
  SORCERY
};

enum class MeleeType
{
  NORMAL,
  MAGIC,
  MITHRIL,
  ADAMANTIUM
};

enum class MovementBaseType
{
  WALKING,
  SWIMMING,
  FLYING
};

struct RangedInfo
{
  Ranged type;
  s16 strength;
  s16 ammo;
  
  RangedInfo(Ranged type, s16 strength, s16 ammo) : type(type), strength(strength), ammo(ammo) { }
  RangedInfo() : RangedInfo(Ranged::NONE, 0, 0) { }
  bool isPresent() const { return type != Ranged::NONE; }
};

struct MeleeInfo
{
  MeleeType type;
  s16 strength;
  
  MeleeInfo(MeleeType type, s16 strength) : type(type), strength(strength) { }
  MeleeInfo(s16 strength) : MeleeInfo(MeleeType::NORMAL, strength) { }
  MeleeInfo() : MeleeInfo(1) { }
};

struct MovementInfo
{
  MovementBaseType type;
  s16 moves;
  
  MovementInfo(MovementBaseType type, s16 moves) : type(type), moves(moves) { }
  MovementInfo(s16 moves) : MovementInfo(MovementBaseType::WALKING, moves) { }
  MovementInfo() : MovementInfo(2) { }
};

enum class Property : u8
{
  MELEE = 0,
  MELEE_TYPE,
  RANGED,
  RANGED_TYPE,
  SHIELDS,
  SHIELDS_RANGED,
  SHIELDS_CHAOS,
  SHIELDS_LIFE,
  SHIELDS_DEATH,
  SHIELDS_NATURE,
  SHIELDS_SORCERY,
  RESIST,
  RESIST_CHAOS,
  RESIST_LIFE,
  RESIST_DEATH,
  RESIST_NATURE,
  RESIST_SORCERY,
  HIT_POINTS,
  //UPKEEP,
  MOVEMENT,
  MOVEMENT_BASE_TYPE,
  SIGHT,
  SPELL_SKILL,
  TO_HIT, // +% to hit expressed in centesimal (eg. 10 = 10%)
  TO_DEFEND,
  XP, // bonus xp to each turn, default is 1 but can modified by certain skills
  FIGURES,
  AMMO,
  ALIVE_FIGURES,
  THROWN_ATTACK, //TODO: implement to get it from skills
  BREATH_ATTACK, // TODO: implement to get it from skills
  
  HEALTH_REGEN, // expressed in percent, (eg. 20 = 20% of total hits)
  
  AVAILABLE_MOVEMENT,
  AVAILABLE_AMMO
};

using prop_value = s32;
using value_t = s32;

class Propertable
{
public:
  virtual prop_value getBaseProperty(Property property) const { return 0; }
  virtual prop_value getBonusProperty(Property property) const { return 0; }
  
  prop_value getProperty(Property property) const { return getBaseProperty(property) + getBonusProperty(property); }
  template<typename T> T getEnumProperty(Property property) const { return static_cast<T>(getProperty(property)); }
  
  MovementInfo getActualMovementInfo() const { return MovementInfo(getEnumProperty<MovementBaseType>(Property::MOVEMENT_BASE_TYPE), getProperty(Property::AVAILABLE_MOVEMENT)); }
  MovementInfo getMovementInfo() const { return MovementInfo(getEnumProperty<MovementBaseType>(Property::MOVEMENT_BASE_TYPE), getProperty(Property::MOVEMENT)); }
  s16 getAvailableMoves() const { return getProperty(Property::AVAILABLE_MOVEMENT); }
  MovementBaseType getMovementBaseType() const { return getEnumProperty<MovementBaseType>(Property::MOVEMENT_BASE_TYPE); }
  bool isFlying() const { return getMovementBaseType() == MovementBaseType::FLYING; }
  
  MeleeInfo getMeleeInfo() const { return MeleeInfo(getEnumProperty<MeleeType>(Property::MELEE_TYPE), getProperty(Property::MELEE)); }
  
  RangedInfo getRangedInfo() const { return RangedInfo(getEnumProperty<Ranged>(Property::RANGED_TYPE), getProperty(Property::RANGED), getProperty(Property::AMMO)); }
  RangedInfo getActualRangedInfo() const { return RangedInfo(getEnumProperty<Ranged>(Property::RANGED_TYPE), getProperty(Property::RANGED), getProperty(Property::AVAILABLE_AMMO)); }
  
  prop_value getDefenseForSchool(School school)
  {
    switch (school)
    {
      case School::CHAOS: return getProperty(Property::SHIELDS_CHAOS);
      case School::NATURE: return getProperty(Property::SHIELDS_NATURE);
      case School::SORCERY: return getProperty(Property::SHIELDS_SORCERY);
      case School::LIFE: return getProperty(Property::SHIELDS_LIFE);
      case School::DEATH: return getProperty(Property::SHIELDS_DEATH);
      default: assert(false); return 0;
    }
  }
  
  prop_value getFigures() const { return getProperty(Property::FIGURES); }
  prop_value getAliveFigures() const { return getProperty(Property::FIGURES); }
};

enum class HeroType : u8
{
  HERO,
  CHAMPION,
  SPECIAL
};

enum class UnitType
{
  RACIAL,
  FANTASTIC,
  HERO
};

class RaceUnitSpec;
class HeroSpec;
class SummonSpec;

class UnitSpec : public Productable
{
protected:
  UnitSpec(UnitType type, Upkeep upkeep, s16 cost, s16 melee, RangedInfo ranged, s16 defense, s16 resistance, s16 hits, s16 figures, s16 movement, s16 sight, const skill_list& skills) :
  type(type), upkeep(upkeep), cost(cost), melee(melee), ranged(ranged), defense(defense), resistance(resistance), hits(hits), figures(figures), movement(movement), sight(sight), skills(skills) { }

public:
  virtual ~UnitSpec() { }
  
  // TODO: there is already productableType(), this should be useless and removed
  const UnitType type;
  
  const Upkeep upkeep;

  const s16 cost;
  const s16 melee;
  RangedInfo ranged;
  const s16 defense;
  const s16 resistance;
  const s16 hits;
  const s16 figures;
  const s16 movement;
  const s16 sight;
  
  const skill_list skills;
  
  prop_value getProperty(Property property) const;
  
  virtual const std::string fullName() const { return productionName(); }
  const std::string& productionName() const override;
  u16 productionCost() const override { return cost; }
  const Upkeep& productionUpkeep() const override { return upkeep; }
  
  template<typename T> const T* as() const
  {
    assert(dynamic_cast<const T*>(this) != nullptr);
    return static_cast<const T*>(this);
  }
};

class RaceUnitSpec : public UnitSpec
{
public:
  RaceUnitSpec(const Race* race, s16 upkeep, s16 cost, s16 melee, RangedInfo ranged, s16 defense, s16 resistance, s16 hits, s16 figures, s16 movement, s16 sight, const skill_list& skills) :
    UnitSpec(UnitType::RACIAL, Upkeep(upkeep,0,1), cost, melee, ranged, defense, resistance, hits, figures, movement, sight, skills), race(race) { }

  const std::string fullName() const override;
  Type productionType() const override { return Type::UNIT; }
  
  const Race* const race;
};

class SummonSpec : public UnitSpec
{
public:
  SummonSpec(School school, s16 upkeep, s16 cost, s16 melee, RangedInfo ranged, s16 defense, s16 resistance, s16 hits, s16 figures, s16 movement, s16 sight, skill_init_list skills) :
    UnitSpec(UnitType::FANTASTIC, Upkeep(0,upkeep,0), cost, melee, ranged, defense, resistance, hits, figures, movement, sight, skills), school(school) { }
  
  const School school;
  
  Type productionType() const override { return Type::SUMMON; }
};

class HeroSpec : public UnitSpec
{
public:
  HeroSpec(HeroType type, u32 requiredFame, items::Slots items, s16 upkeep, s16 cost, s16 melee, RangedInfo ranged, s16 defense, s16 resistance, s16 hits, s16 figures, s16 movement, s16 sight, skill_init_list skills) :
    UnitSpec(UnitType::HERO, Upkeep(upkeep,0,0), cost, melee, ranged, defense, resistance, hits, figures, movement, sight, skills), type(type), items(items), requiredFame(requiredFame) { }
  
  const HeroType type;
  const items::Slots items;
  const u32 requiredFame;
  
  Type productionType() const override { return Type::HERO; }
};


#endif
