//
//  UnitDetailView.h
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _UNIT_DETAIL_VIEW_H_
#define _UNIT_DETAIL_VIEW_H_

#include "View.h"
#include "SkillDraw.h"

class ViewManager;

class Unit;

class UnitDetailView : public View
{
private:
  enum button
  {

    UP_ARROW,
    DOWN_ARROW,
    
    LOW_ACTION,
    HIGH_ACTION,
    
    OK = LOW_ACTION,
    DISMISS = HIGH_ACTION,
    
    HERO_REJECT = LOW_ACTION,
    HERO_HIRE = HIGH_ACTION,
    
    BUTTON_COUNT
  };
  
  enum class Mode
  {
    NORMAL,
    VIEW_ONLY,
    HERO_HIRE
  } mode;
  
  SkillDraw skillDraw;
  Point c;
  
  const Unit* unit;
  u32 hireCost;
  
  void draw() override;
  void drawPost() override { }
  
  void switchMode(Mode mode);
  
  void buttonClicked(button button);
  
public:
  UnitDetailView(ViewManager* gvm);
  
  void activate() override { }
  void deactivate() override { }
  
  bool mousePressed(u16 x, u16 y, MouseButton b) override;
  
  void setHeroHire(Hero* hero, u32 cost);
  void setUnit(const Unit* unit, bool withButtons = true);
};

#endif
