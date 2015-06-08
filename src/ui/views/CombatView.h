//
//  CombatView.h
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _COMBAT_VIEW_H_
#define _COMBAT_VIEW_H_

#include "View.h"

class ViewManager;
class CombatUnit;
class Combat;

class CombatView : public View
{
private:
  enum Button
  {
    OPTIONS_OK,
    OPTIONS_CANCEL,
    OPTIONS_DIFFICULTY,
    OPTIONS_OPPONENTS,
    OPTIONS_LAND_SIZE,
    OPTIONS_MAGIC,
    
    BUTTON_COUNT
  };
  
  enum class SubState
  {
    COMBAT,
    SPELL_CAST
  };
  
  void draw() override;
  void drawPost() override { }
  
  void drawUnitProps(CombatUnit* unit, bool onTheLeft);
  
  Coord hover;
  
  Combat* combat;
  
public:
  CombatView(ViewManager* gvm);
  
  void activate() override { }
  void deactivate() override { }
  
  ScreenCoord coordsForTile(u16 x, u16 y);

  void mouseReleased(u16 x, u16 y, MouseButton b) override;
  void mouseMoved(u16 x, u16 y, MouseButton b) override;
  
};

#endif 