//
//  CombatMechanics.h
//  OpenMoM
//
//  Created by Jack on 7/30/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _COMBAT_MECHANICS_H_
#define _COMBAT_MECHANICS_H_

#include "Common.h"

enum class CombatModifier : u8;

class Game;
struct AttackInfo;

class CombatMechanics
{
private:
  Game* const game;
public:
  CombatMechanics(Game* game) : game(game) { }
  
  virtual u32 totalMeleeHitsThatLandOnTarget(const AttackInfo& info);
  
  virtual u32 hitsThatReachTarget(const AttackInfo& info, u32 total);
  virtual u32 hitsDefendedByTarget(const AttackInfo& info, u32 total);
};

#endif
