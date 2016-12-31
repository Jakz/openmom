//
//  Unitspec.h
//  OpenMoM
//
//  Created by Jack on 7/21/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _UNIT_SPEC_H_
#define _UNIT_SPEC_H_

#include "Common.h"
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

enum class Ranged : u8
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
  THROWN_ATTACK //TODO: implement to get it from skills
};

enum class MovementBaseType
{
  WALKING,
  SWIMMING,
  FLYING
};

enum class HeroType : u8
{
  HERO,
  CHAMPION,
  SPECIAL
};

class RaceUnitSpec;
class HeroSpec;
class SummonSpec;

class UnitSpec : public Productable
{
protected:
  UnitSpec(UnitID ident, Upkeep upkeep, s16 cost, s16 melee, s16 ranged, Ranged rangedType, s16 ammo, s16 defense, s16 resistance, s16 hits, s16 figures, s16 movement, s16 sight, skill_init_list skills) :
  ident(ident), upkeep(upkeep), cost(cost), melee(melee), ranged(ranged), rangedType(rangedType), ammo(ammo),
  defense(defense), resistance(resistance), hits(hits), figures(figures), movement(movement), sight(sight), skills(skills) { }

public:
  const Upkeep upkeep;

  const s16 cost;
  const s16 melee;
  const s16 ranged;
  const Ranged rangedType;
  const s16 ammo;
  const s16 defense;
  const s16 resistance;
  const s16 hits;
  const s16 figures;
  const s16 movement;
  const s16 sight;
  
  const skill_list skills;
  
  s16 getProperty(Property property) const;
  
  const std::string& productionName() const override;
  u16 productionCost() const override { return cost; }
  const Upkeep& productionUpkeep() const override { return upkeep; }
  
  const UnitID ident;
  
  static const UnitSpec* unitSpec(UnitID unit, RaceID race = RaceID::BARBARIANS);
  static const RaceUnitSpec* raceSpec(UnitID unit, RaceID race);
  static const HeroSpec* heroSpec(UnitID unit);
  static const SummonSpec* summonSpec(UnitID unit);
  static const std::vector<const RaceUnitSpec*> unitsForRace(RaceID race);
  
  template<typename T> const T* as() const { return static_cast<const T*>(this); }
};

class RaceUnitSpec : public UnitSpec
{
public:
  RaceUnitSpec(UnitID ident, RaceID race, s16 upkeep, s16 cost, s16 melee, s16 ranged, Ranged rangedType, s16 ammo, s16 defense, s16 resistance, s16 hits, s16 figures, s16 movement, s16 sight, skill_init_list skills) :
    UnitSpec(ident, Upkeep(upkeep,0,1), cost, melee, ranged, rangedType, ammo, defense, resistance, hits, figures, movement, sight, skills), race(Race::race(race)) { }
  
  Type productionType() const override { return Type::UNIT; }
  
  const Race& race;
};

class SummonSpec : public UnitSpec
{
public:
  SummonSpec(UnitID ident, School school, s16 upkeep, s16 cost, s16 melee, s16 ranged, Ranged rangedType, s16 ammo, s16 defense, s16 resistance, s16 hits, s16 figures, s16 movement, s16 sight, skill_init_list skills) :
    UnitSpec(ident, Upkeep(0,upkeep,0), cost, melee, ranged, rangedType, ammo, defense, resistance, hits, figures, movement, sight, skills), school(school) { }
  
  const School school;
  
  Type productionType() const override { return Type::SUMMON; }
};

class HeroSpec : public UnitSpec
{
public:
  HeroSpec(UnitID ident, HeroType type, u32 requiredFame, items::Slots::Type items, s16 upkeep, s16 cost, s16 melee, s16 ranged, Ranged rangedType, s16 ammo, s16 defense, s16 resistance, s16 hits, s16 figures, s16 movement, s16 sight, skill_init_list skills) :
    UnitSpec(ident, Upkeep(upkeep,0,0), cost, melee, ranged, rangedType, ammo, defense, resistance, hits, figures, movement, sight, skills), type(type), items(items), requiredFame(requiredFame) { }
  
  const HeroType type;
  const items::Slots::Type items;
  const u32 requiredFame;
  
  Type productionType() const override { return Type::HERO; }
};


#endif
