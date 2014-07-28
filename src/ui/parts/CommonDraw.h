//
//  CommonDraw.h
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _COMMON_DRAW_H_
#define _COMMON_DRAW_H_

#include "Common.h"

class UnitSpec;
class Unit;
class Player;

class CommonDraw
{
  public:
    static void drawMovement(u16 v, u16 x, u16 y, u16 c);
    static void drawGoldUpkeep(u16 g, u16 x, u16 y);
    static void drawUpkeep(const Upkeep& upkeep, u16 x, u16 y);
  
    static void drawWizardGem(const Player* player, u16 x, u16 y);
  
    static void drawUnitProps(const UnitSpec* spec, u16 xx, u16 yy, s16 max);
    static void drawUnitPropsComplete(const Unit* unit, u16 xx, u16 yy, s16 max);

};

#endif
