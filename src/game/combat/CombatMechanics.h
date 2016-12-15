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

class CombatMechanics
{
private:
  Game &game;
public:
  CombatMechanics(Game &game) : game(game) { }
};

#endif
