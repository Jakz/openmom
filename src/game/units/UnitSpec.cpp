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

const string& UnitSpec::productionName() const { return i18n::s(name); }

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


// TODO: missing skills


static const RaceUnitSpec raceSpecs[] = {
  RaceUnitSpec(  UnitID::SPEARMEN, RaceID::BARBARIANS,   I18::UNIT_SPEARMEN, 0,  15,     1, 0,   Ranged::NONE, 0,    2, 5, 1, 8,    1, 1,   {}), // LARGE_SHIELD
  RaceUnitSpec( UnitID::SWORDSMEN, RaceID::BARBARIANS,  I18::UNIT_SWORDSMEN, 1,  30,     3, 0,   Ranged::NONE, 0,    2, 5, 1, 6,    1, 1,   {}),
  RaceUnitSpec(    UnitID::BOWMEN, RaceID::BARBARIANS,     I18::UNIT_BOWMEN, 1,  30,     1, 1,  Ranged::ARROW, 8,    1, 5, 1, 6,    1, 1,   {}),
  RaceUnitSpec(   UnitID::CAVALRY, RaceID::BARBARIANS,    I18::UNIT_CAVALRY, 2,  60,     4, 0,   Ranged::NONE, 0,    2, 5, 3, 4,    2, 1,   {}),
  RaceUnitSpec(   UnitID::SHAMANS, RaceID::BARBARIANS,     I18::UNIT_SHAMAN, 1,  50,     2, 2, Ranged::NATURE, 4,    3, 7, 1, 4,    1, 1,   {}),
  RaceUnitSpec(UnitID::BERSERKERS, RaceID::BARBARIANS, I18::UNIT_BERSERKERS, 3, 120,     7, 0,   Ranged::NONE, 0,    3, 7, 3, 6,    1, 1,   {}),
  RaceUnitSpec(  UnitID::SETTLERS, RaceID::BARBARIANS,   I18::UNIT_SETTLERS, 2,  50,     0, 0,   Ranged::NONE, 0,    1, 5,10, 1,    1, 1,   {}), // CREATE_OUTPOST
  
  RaceUnitSpec(  UnitID::SPEARMEN,   RaceID::BEASTMEN,   I18::UNIT_SPEARMEN, 0, 20,      2, 0, Ranged::NONE, 0,      2, 5, 2, 7,    1, 1,   {}),
  RaceUnitSpec( UnitID::SWORDSMEN,   RaceID::BEASTMEN,  I18::UNIT_SWORDSMEN, 1, 40,      4, 0, Ranged::NONE, 0,      2, 5, 2, 6,    1, 1,   {}),
  RaceUnitSpec(    UnitID::BOWMEN,   RaceID::BEASTMEN,     I18::UNIT_BOWMEN, 1, 30,     1, 1, Ranged::ARROW, 8,      1, 5, 1, 6,    1, 1,   {})
};

static const HeroSpec heroSpecs[] = {
  HeroSpec(UnitID::HERO_DWARF, I18::HERO_DWARF, HeroType::HERO, ItemSlots::WARRIOR, 2, -1,    5, 0, Ranged::NONE, 0,     4, 10, 11, 1,    1, 1,   {}), // LUCK, MOUNTAINWALK
  HeroSpec(UnitID::HERO_ORC_WARRIOR, I18::HERO_ORC_WARRIOR, HeroType::HERO, ItemSlots::WARRIOR, 2, -1,   6, 0, Ranged::NONE, 0,    4, 6, 8, 1,    1, 1,   {}) // MOUNTAINWALK
};

static const SummonSpec summonSpecs[] = {
  SummonSpec(UnitID::MAGIC_SPIRIT, I18::UNIT_MAGIC_SPIRIT, 1, 30,    5, 0, Ranged::NONE, 0,    4, 8, 10, 1,     1, 2,   {}), // MELD NON_CORPOREAL

  SummonSpec(UnitID::HELL_HOUNDS, I18::UNIT_HELL_HOUNDS,   1, 40,     3, 0, Ranged::NONE, 0,      2, 6, 4, 4,     2, 1,   {}),
  SummonSpec(UnitID::GREAT_DRAKE, I18::UNIT_GREAT_DRAKE, 30, 900,    30, 0, Ranged::NONE, 0,     10,12,30, 1,     2, 2,   {})  //FLYING TO_HIT3 FIERY_BREATH20
};






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