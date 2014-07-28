//
//  ArmiesItemsView.h
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _ARMIES_ITEMS_VIEW_H_
#define _ARMIES_ITEMS_VIEW_H_

#include "View.h"

class ViewManager;

class ArmiesItemsView : public View
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
  ArmiesItemsView(ViewManager* gvm);
  
  void activate() override { }
  void deactivate() override { }
};

#endif
