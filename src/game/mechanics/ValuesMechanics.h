//
//  LairMechanics.h
//  OpenMoM
//
//  Created by Jack on 8/1/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#pragma once

#include "Common.h"

class Game;

enum class Value
{
  WARPED_NODE_POWER_MALUS,
  
  NODE_MASTERY_MANA_MULTIPLIER,
  SCHOOL_MASTERY_MANA_NODE_MULTIPLIER,
  DIFFICULTY_MANA_NODE_MULTIPLIER,
};

class ValuesMechanics
{
private:
  Game* game;

public:
  ValuesMechanics(Game* game) : game(game) { }
  
  template<typename T> T get(Value value);
};
