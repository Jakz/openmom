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
  
  void draw() override { };
  void drawPost() override { }
  
public:
  CombatView(ViewManager* gvm);
  
  void activate() override { }
  void deactivate() override { }
};

#endif 