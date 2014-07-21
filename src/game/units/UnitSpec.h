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

#include <vector>

enum class ItemSlots : u8;

enum class UnitID : u16
{
  
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
  SIGHT,
  TO_HIT, // +% to hit expressed in centesimal (eg. 10 = 10%)
  TO_DEFEND,
  XP, // bonus xp to each turn, of course default is 1 but can modified by certain skills
  FIGURES,
  AMMO,
  ALIVE_FIGURES,
  THROWN_ATTACK //TODO: implement to get it from skills
};

enum class AttackType : u8
{
  NORMAL,
  MAGIC,
  MITHRIL,
  ADAMANTIUM
};

enum class HeroType : u8
{
  HERO,
  CHAMPION,
  SPECIAL
};


class UnitSpec : public Productable
{
protected:
  const UnitID ident;
  const I18 name;
  
  UnitSpec(UnitID ident, I18 name, Upkeep upkeep, s16 cost, s16 melee, s16 ranged, Ranged rangedType, s16 ammo, s16 defense, s16 resistance, s16 hits, s16 figures, s16 movement, s16 sight, skill_init_list skills) :
  ident(ident), name(name), upkeep(upkeep), cost(cost), melee(melee), ranged(ranged), rangedType(rangedType), ammo(ammo),
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
  virtual u16 productionCost() { return cost; }
  virtual const Upkeep& productionUpkeep() { return upkeep; }
  
  Type productionType() const override { return Type::UNIT; }
};

class RaceUnitSpec : public UnitSpec
{
public:
  RaceUnitSpec(UnitID ident, I18 name, s16 upkeep, s16 cost, s16 melee, s16 ranged, Ranged rangedType, s16 ammo, s16 defense, s16 resistance, s16 hits, s16 figures, s16 movement, s16 sight, skill_init_list skills) :
    UnitSpec(ident, name, Upkeep(upkeep,0,1), cost, melee, ranged, rangedType, ammo, defense, resistance, hits, figures, movement, sight, skills) { }
};

class SummonSpec : public UnitSpec
{
public:
  SummonSpec(UnitID ident, I18 name, s16 upkeep, s16 cost, s16 melee, s16 ranged, Ranged rangedType, s16 ammo, s16 defense, s16 resistance, s16 hits, s16 figures, s16 movement, s16 sight, skill_init_list skills) :
    UnitSpec(ident, name, Upkeep(0,upkeep,0), cost, melee, ranged, rangedType, ammo, defense, resistance, hits, figures, movement, sight, skills) { }
};

class HeroSpec : public UnitSpec
{
public:
  HeroSpec(UnitID ident, I18 name, HeroType type, ItemSlots items, s16 upkeep, s16 cost, s16 melee, s16 ranged, Ranged rangedType, s16 ammo, s16 defense, s16 resistance, s16 hits, s16 figures, s16 movement, s16 sight, skill_init_list skills) :
    UnitSpec(ident, name, Upkeep(upkeep,0,0), cost, melee, ranged, rangedType, ammo, defense, resistance, hits, figures, movement, sight, skills), type(type), items(items) { }
  
  const HeroType type;
  const ItemSlots items;
  
  Type productionType() const override { return Type::HERO; }
};

// TODO: SpriteInfo management of units

#endif
