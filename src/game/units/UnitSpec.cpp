//
//  Unitspec.cpp
//  OpenMoM
//
//  Created by Jack on 7/21/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "UnitSpec.h"

#include "GfxData.h"
#include "Unit.h"
#include "Items.h"

#include "Localization.h"

using namespace std;

//TODO: GfxData here is not nice
const string& UnitSpec::productionName() const { return i18n::s(GfxData::unitGfx(this).name); }

s16 UnitSpec::getProperty(Property property) const
{
  switch (property) {
    case Property::MELEE: return melee;
    case Property::RANGED: return ranged.strength;
    case Property::RANGED_TYPE: return static_cast<s16>(ranged.type); // TODO: hack
    case Property::SHIELDS: return defense;
    case Property::RESIST: return resistance;
    case Property::HIT_POINTS: return hits;
    case Property::MOVEMENT: return movement;
    case Property::SIGHT: return sight;
    case Property::TO_HIT: return 30; // default
    case Property::TO_DEFEND: return 30;
    case Property::FIGURES: return figures;
    case Property::AMMO: return ranged.ammo;
      
    //case Property::UPKEEP: // TODO: commented

    default: return 0;
  }
}


const std::string RaceUnitSpec::fullName() const { return i18n::s(GfxData::raceGfxSpec(race).unitName) + " " + productionName(); }
