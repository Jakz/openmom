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

class Player;
class Army;
class Unit;
class UnitSpec;
class Hero;
class Level;

enum class Dir;

class UnitDraw
{
private:
  static void bindPlayerColorPalette(PlayerColor color);
  static void unbindPlayerColorPalette();
  
public:
  enum class CombatAction : u8
  {
    STAY,
    MOVE,
    ATTACK
  };
  
  static void drawStatic(const Army* army, s16 x, s16 y, bool forceDraw = false);
  static void drawStatic(const Unit* unit, s16 x, s16 y, bool backdrop, bool grayScale);
  static void rawDrawStatic(const Army* army, s16 x, s16 y);
  static void drawUnitIso(const UnitSpec* unit, s16 x, s16 y, const Unit* realUnit, const Player* owner);
	static void drawHeroPortrait(const Hero* unit, s16 x, s16 y);

  static void drawUnitIsoCombat(const Unit* unit, s16 x, s16 y, Dir facing, CombatAction caction);
  
  static void drawUnitLevel(const Level* level, u16 x, u16 y, u16 spacing, bool shadowed);
  
  static Color colorForHealth(float percent);
  static void drawHealthBar(u16 x, u16 y, u16 width, float percent);
  static std::string stringForDoubleMovement(s16 value, bool hideZero = false);

};

#endif
