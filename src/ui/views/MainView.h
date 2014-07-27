//
//  MainView.h
//  OpenMoM
//
//  Created by Jack on 7/27/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _MAIN_VIEW_H_
#define _MAIN_VIEW_H_

#include "View.h"

class Army;
class Tile;

class MainView : public View
{
private:
  enum Button : u8
  {
    GAME,
    SPELLS,
    ARMIES,
    CITIES,
    MAGIC,
    INFO,
    PLANE,
    NEXT,
    CANCEL_SURVEYOR,
    DONE,
    PATROL,
    WAIT,
    BUILD,
    CANCEL,
    PURIFY,
    OK,
    MELD,
    
    BUTTON_COUNT
  };
  
  enum Substate : u8
  {
    MAIN,
    UNIT,
    SURVEYOR,
    SPELL_CAST
  };
  
  Substate substate;
  
  Tile* hoveredTile;

  void draw() override;
  void drawPost() override;
  
public:
  MainView();
  
  void activate() override { }
  void deactivate() override { }
  
  void switchToSpellCast(LocalPlayer* player);
  void switchToUnitSelection(LocalPlayer* player, Army* army);
  void switchToNormalState(LocalPlayer* player);
  void switchToSurveyor(LocalPlayer* player);
  void updateBuildButton(LocalPlayer* player);
  
  
  void mousePressed(u16 x, u16 y, MouseButton b) override {}
  void mouseClicked(u16 x, u16 y, MouseButton b) override {}
  void mouseReleased(u16 x, u16 y, MouseButton b) override {}
  void mouseMoved(u16 x, u16 y, MouseButton b) override {}
  void mouseDragged(u16 x, u16 y, MouseButton b) override {}
  
  void keyPressed(KeyboardKey key, KeyboardMod mod) override {}
  void keyReleased(KeyboardKey key, KeyboardMod mod) override {}
};

#endif
