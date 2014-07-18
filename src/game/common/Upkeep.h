//
//  Upkeep.h
//  OpenMoM
//
//  Created by Jack on 7/17/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _UPKEEP_H_
#define _UPKEEP_H_

#include "Common.h"

class Upkeep
{
  private:
    s16 gold, mana, food;
  
  public:
    Upkeep(s16 gold = 0, s16 mana = 16, s16 food = 0) : gold(gold), mana(mana), food(food) { }
  
  public:
    void add(const Upkeep &upkeep)
    {
      gold += upkeep.gold;
      mana += upkeep.mana;
      food += upkeep.food;
    }
};


#endif
