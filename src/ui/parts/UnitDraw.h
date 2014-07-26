//
//  UnitDraw.h
//  OpenMoM
//
//  Created by Jack on 7/26/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _UNIT_DRAW_H_
#define _UNIT_DRAW_H_

#include "Common.h"

class Army;
class Unit;
class UnitSpec;
class Hero;

class UnitDraw
{
public:
  enum class CombatAction : u8
  {
    STAY,
    MOVE,
    ATTACK
  };
  
  static void drawStatic(const Army* army, u16 x, u16 y);
  static void drawStatic(const Unit* unit, u16 x, u16 y, bool backdrop, bool grayScale);
  static void rawDrawStatic(const Army* army, u16 x, u16 y);
  static void drawUnitIso(const UnitSpec* unit, u16 x, u16 y, const Unit* realUnit);
	static void drawHeroPortrait(const Hero* unit, u16 x, u16 y);

  static void drawUnitIsoCombat(const Unit* unit, u16 x, u16 y, s16 facing, CombatAction caction);

};

#endif
