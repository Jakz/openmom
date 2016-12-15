//
//  UnitDraw.h
//  OpenMoM
//
//  Created by Jack on 7/26/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _UNIT_DRAW_H_
#define _UNIT_DRAW_H_

#include "common/Common.h"

class Army;
class Unit;
class UnitSpec;
class Hero;

enum class Facing;

class UnitDraw
{
public:
  enum class CombatAction : u8
  {
    STAY,
    MOVE,
    ATTACK
  };
  
  static void drawStatic(const Army* army, s16 x, s16 y);
  static void drawStatic(const Unit* unit, s16 x, s16 y, bool backdrop, bool grayScale);
  static void rawDrawStatic(const Army* army, s16 x, s16 y);
  static void drawUnitIso(const UnitSpec* unit, s16 x, s16 y, const Unit* realUnit);
	static void drawHeroPortrait(const Hero* unit, s16 x, s16 y);

  static void drawUnitIsoCombat(const Unit* unit, s16 x, s16 y, Facing facing, CombatAction caction);

};

#endif
