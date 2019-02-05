//
//  LairMechanics.h
//  OpenMoM
//
//  Created by Jack on 8/1/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#pragma once

#include "common/Common.h"

class Game;

enum class GameValue
{
  WARPED_NODE_POWER_MALUS,
  
  NODE_MASTERY_MANA_MULTIPLIER,
  SCHOOL_MASTERY_MANA_NODE_MULTIPLIER,
  DIFFICULTY_MANA_NODE_MULTIPLIER,
  
  MAX_RETORTS_FOR_NEW_WIZARD
};

class ValuesMechanics
{
private:
  Game* const game;

public:
  ValuesMechanics(Game* game) : game(game) { }
  
  template<typename T> T get(GameValue value);
};
