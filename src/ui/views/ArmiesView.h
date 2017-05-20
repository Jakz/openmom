//
//  ArmiesView.h
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _ARMIES_VIEW_H_
#define _ARMIES_VIEW_H_

#include "View.h"

class ViewManager;
class Army;
class Unit;

class ArmiesView : public View
{
private:
  enum
  {
    ITEMS,
    OK,
    PREV1,
    NEXT1,
    PREV2,
    NEXT2,
    
    BUTTON_COUNT
  };
  
  s16 offset;
  const Unit* unit;
  const Army* army;
  
  void draw() override;
  void drawPost() override { }
  
public:
  ArmiesView(ViewManager* gvm);
  
  void activate() override { updateScrollButtons(); }
  void deactivate() override { offset = 0; }
  
  bool mouseMoved(u16 x, u16 y, MouseButton b) override;
  
  void scrollDown() { ++offset; }
  void scrollUp() { --offset; }
  void updateScrollButtons();
  

};

#endif
