//
//  CombatMechanics.cpp
//  OpenMoM
//
//  Created by Jack on 7/30/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "CombatMechanics.h"

#include "Combat.h"
#include "Util.h"


u32 CombatMechanics::totalMeleeHitsThatLandOnTarget(const AttackInfo& info)
{
  return info.attacker->getProperty(Property::MELEE) * info.attacker->getProperty(Property::ALIVE_FIGURES);
}

u32 CombatMechanics::hitsThatReachTarget(const AttackInfo& info, u32 total)
{
  return Util::passingRolls(total, info.attacker->getProperty(Property::TO_HIT));
}

u32 CombatMechanics::hitsDefendedByTarget(const AttackInfo& info, u32 total)
{
  //TODO: maybe pass attack type in info to distinguish between magic ranged attacks
  //TODO: wrong behavior, shields * figure should be
  return Util::passingRolls(total, info.attacker->getProperty(Property::SHIELDS));
}
