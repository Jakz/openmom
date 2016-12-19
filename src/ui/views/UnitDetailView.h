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
  enum Button
  {

    UP_ARROW,
    DOWN_ARROW,
    
    HERO_HIRE,
    HERO_REJECT,
    
    DISMISS,
    OK,
    
    BUTTON_COUNT
  };
  
  enum class Mode
  {
    NORMAL,
    HERO_HIRE
  } mode;
  
  SkillDraw skillDraw;
  ScreenCoord c;
  
  Unit* unit;
  u32 hireCost;
  
  void draw() override;
  void drawPost() override { }
  
  void switchMode(Mode mode);
  
public:
  UnitDetailView(ViewManager* gvm);
  
  void activate() override { }
  void deactivate() override { }
  
  void setHeroHire(Hero* hero, u32 cost);
  void setUnit(Unit* unit);
};

#endif
