//
//  LairMechanics.cpp
//  OpenMoM
//
//  Created by Jack on 8/1/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "ValuesMechanics.h"

#include "Game.h"

template<> float ValuesMechanics::get<float>(GameValue value)
{
  switch (value)
  {
    case GameValue::NODE_MASTERY_MANA_MULTIPLIER: return 2.0f;
    case GameValue::SCHOOL_MASTERY_MANA_NODE_MULTIPLIER: return 2.0f;
      
    case GameValue::DIFFICULTY_MANA_NODE_MULTIPLIER:
    {
      switch (game->settings.magicPower()) {
        case MAGIC_POWER_WEAK: return 0.5f;
        case MAGIC_POWER_NORMAL: return 1.0f;
        case MAGIC_POWER_POWERFUL: return 1.5f;
        default: return 0.0f;
      }
    }
      
    default: assert(false); return 0.0f;
  }
}

template<> value_t ValuesMechanics::get<value_t>(GameValue value)
{
  switch (value)
  {
    case GameValue::WARPED_NODE_POWER_MALUS: return 5;
    
    case GameValue::MAX_RETORTS_FOR_NEW_WIZARD: return 6;
      
    default: assert(false); return 0;
  }
}
