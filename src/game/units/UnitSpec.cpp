//
//  Unitspec.cpp
//  OpenMoM
//
//  Created by Jack on 7/21/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "UnitSpec.h"

#include "Unit.h"
#include "Items.h"

#include "Localization.h"

using namespace std;

const string& UnitSpec::productionName() const { return i18n::s(ident); }

s16 UnitSpec::getProperty(Property property) const
{
  switch (property) {
    case Property::MELEE: return melee;
    case Property::RANGED: return ranged;
    case Property::RANGED_TYPE: return static_cast<s16>(rangedType); // TODO: hack
    case Property::SHIELDS: return defense;
    case Property::RESIST: return resistance;
    case Property::HIT_POINTS: return hits;
    case Property::MOVEMENT: return movement;
    case Property::SIGHT: return sight;
    case Property::TO_HIT: return 30; // default
    case Property::TO_DEFEND: return 30;
    case Property::FIGURES: return figures;
    case Property::AMMO: return ammo;
      
    //case Property::UPKEEP: // TODO: commented

    default: return 0;
  }
}

// RaceUnitSpec(UnitID ident, RaceID race, s16 upkeep, s16 cost, s16 melee, s16 ranged, Ranged rangedType, s16 ammo, s16 defense, s16 resistance, s16 hits, s16 figures, s16 movement, s16 sight, skill_init_list skills) :


static const RaceUnitSpec raceSpecs[] = {
  RaceUnitSpec(  UnitID::SPEARMEN, RaceID::BARBARIANS, 0,  15,     1, 0,   Ranged::NONE, 0,    2, 5, 1, 8,    1, 1,   {}),
  RaceUnitSpec( UnitID::SWORDSMEN, RaceID::BARBARIANS, 1,  30,     3, 0,   Ranged::NONE, 0,    2, 5, 1, 6,    1, 1,   {Skills::LARGE_SHIELD}),
  RaceUnitSpec(    UnitID::BOWMEN, RaceID::BARBARIANS, 1,  30,     1, 1,  Ranged::ARROW, 8,    1, 5, 1, 6,    1, 1,   {}),
  RaceUnitSpec(   UnitID::CAVALRY, RaceID::BARBARIANS, 2,  60,     4, 0,   Ranged::NONE, 0,    2, 5, 3, 4,    2, 1,   {}),
  RaceUnitSpec(   UnitID::SHAMANS, RaceID::BARBARIANS, 1,  50,     2, 2, Ranged::NATURE, 4,    3, 7, 1, 4,    1, 1,   {}),
  RaceUnitSpec(UnitID::BERSERKERS, RaceID::BARBARIANS, 3, 120,     7, 0,   Ranged::NONE, 0,    3, 7, 3, 6,    1, 1,   {}),
  RaceUnitSpec(  UnitID::SETTLERS, RaceID::BARBARIANS, 2,  50,     0, 0,   Ranged::NONE, 0,    1, 5,10, 1,    1, 1,   {Skills::CREATE_OUTPOST}),
  
  RaceUnitSpec(  UnitID::SPEARMEN,   RaceID::BEASTMEN, 0, 20,      2, 0,   Ranged::NONE, 0,    2, 5, 2, 7,    1, 1,   {}),
  RaceUnitSpec( UnitID::SWORDSMEN,   RaceID::BEASTMEN, 1, 40,      4, 0,   Ranged::NONE, 0,    2, 5, 2, 6,    1, 1,   {}),
  RaceUnitSpec(    UnitID::BOWMEN,   RaceID::BEASTMEN, 1, 30,      1, 1,  Ranged::ARROW, 8,    1, 5, 1, 6,    1, 1,   {})
};

static const HeroSpec heroSpecs[] = {
  HeroSpec(UnitID::HERO_DWARF, HeroType::HERO, 0, items::Slots::Type::WARRIOR, 2, -1,    5, 0, Ranged::NONE, 0,     4, 10, 11, 1,    1, 1,   {Skills::LUCKY, Skills::MOUNTAINWALK}),
  HeroSpec(UnitID::HERO_ORC_WARRIOR, HeroType::HERO, 0, items::Slots::Type::WARRIOR, 2, -1,   6, 0, Ranged::NONE, 0,    4, 6, 8, 1,    1, 1,   {Skills::MOUNTAINWALK})
};

// SummonSpec(UnitID ident, s16 upkeep, s16 cost, s16 melee, s16 ranged, Ranged rangedType, s16 ammo, s16 defense, s16 resistance, s16 hits, s16 figures, s16 movement, s16 sight, skill_init_list skills) :
static const SummonSpec summonSpecs[] = {
  SummonSpec(UnitID::MAGIC_SPIRIT, School::ARCANE, 1, 30,    5, 0, Ranged::NONE, 0,    4, 8, 10, 1,     1, 2,   {Skills::MELD, Skills::SWIMMING, Skills::NON_CORPOREAL}),
  
  SummonSpec(UnitID::GUARDIAN_SPIRIT, School::LIFE, 1, 80,    10, 0, Ranged::NONE, 0,    4, 10, 10, 1,     1, 2,   {Skills::MELD, Skills::NON_CORPOREAL, Skills::RESISTANCE_TO_ALL}),

  
  
  SummonSpec(UnitID::HELL_HOUNDS, School::CHAOS,  1, 40,     3, 0, Ranged::NONE, 0,      2, 6, 4, 4,     2, 1,   {}),
  SummonSpec(UnitID::GREAT_DRAKE, School::CHAOS, 30, 900,    30, 0, Ranged::NONE, 0,     10,12,30, 1,     2, 2,   {Skills::FLYING, Skills::TO_HIT3, Skills::FIERY_BREATH20})
};

const std::vector<const RaceUnitSpec*> UnitSpec::unitsForRace(RaceID race)
{
  std::vector<const RaceUnitSpec*> units;
  
  for (const auto& u : raceSpecs)
  {
    if (u.race.ident == race)
      units.push_back(&u);
  }
  
  return units;
}

const RaceUnitSpec* UnitSpec::raceSpec(UnitID unit, RaceID race)
{
  const RaceUnitSpec* spec =  find_if(begin(raceSpecs), end(raceSpecs), [&](const RaceUnitSpec& spec) { return spec.ident == unit && spec.race.ident == race; } );
  return spec != end(raceSpecs) ? spec : nullptr;
}

const HeroSpec* UnitSpec::heroSpec(UnitID unit)
{
  const HeroSpec* spec =  find_if(begin(heroSpecs), end(heroSpecs), [&](const HeroSpec& spec) { return spec.ident == unit; } );
  return spec != end(heroSpecs) ? spec : nullptr;
}

const SummonSpec* UnitSpec::summonSpec(UnitID unit)
{
  const SummonSpec* spec =  find_if(begin(summonSpecs), end(summonSpecs), [&](const SummonSpec& spec) { return spec.ident == unit; } );
  return spec != end(summonSpecs) ? spec : nullptr;
}

const UnitSpec* UnitSpec::unitSpec(UnitID unit, RaceID race)
{
  const UnitSpec* spec = heroSpec(unit);
  if (spec) return spec;
  
  spec = summonSpec(unit);
  if (spec) return spec;
  
  spec = raceSpec(unit, race);
  return spec ? spec : nullptr;
}
