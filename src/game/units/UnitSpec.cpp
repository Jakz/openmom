//
//  Unitspec.cpp
//  OpenMoM
//
//  Created by Jack on 7/21/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "UnitSpec.h"

#include "Unit.h"
#include "Localization.h"


const std::string& UnitSpec::productionName() const { return i18n::s(name); }

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