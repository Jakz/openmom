//
//  AlchemyView.h
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _ALCHEMY_VIEW_H_
#define _ALCHEMY_VIEW_H_

#include "View.h"

class ViewManager;

class AlchemyView : public View
{
private:
  enum
  {
    CANCEL,
    OK,
    SWITCH,
    SWITCH2,
    
    BUTTON_COUNT
  };
  
  bool inverted;
  float percent;
  s32 amount;
  
  void draw() override;
  void drawPost() override { }
  
public:
  AlchemyView(ViewManager* gvm);
  
  void activate() override
  {
    percent = 0.0f;
    amount = 0;
  }
  
  bool mouseDragged(u16 x, u16 y, MouseButton b) override;
  bool mouseClicked(u16 x, u16 y, MouseButton b) override { return mouseDragged(x, y, b); }
  
  void deactivate() override { }
};

#endif
