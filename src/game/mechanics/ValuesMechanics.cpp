//
//  LairMechanics.cpp
//  OpenMoM
//
//  Created by Jack on 8/1/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "game/mechanics/ValuesMechanics.h"

#include "game/Game.h"

template<> float ValuesMechanics::get<float>(Value value)
{
  switch (value)
  {
    case Value::NODE_MASTERY_MANA_MULTIPLIER: return 2.0f;
    case Value::SCHOOL_MASTERY_MANA_NODE_MULTIPLIER: return 2.0f;
      
    case Value::DIFFICULTY_MANA_NODE_MULTIPLIER:
    {
      switch (game->settings.magicPower()) {
        case MAGIC_POWER_WEAK: return 0.5f;
        case MAGIC_POWER_NORMAL: return 1.0f;
        case MAGIC_POWER_POWERFUL: return 1.5f;
        default: return 0.0f;
      }
    }
      
    default: return 0.0f;
  }
}

template<> s32 ValuesMechanics::get<s32>(Value value)
{
  switch (value)
  {
    case Value::WARPED_NODE_POWER_MALUS: return 5;
      
    default: return 0;
  }
}
