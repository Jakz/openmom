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

class ViewManager;

class Unit;

class UnitDetailView : public View
{
private:
  enum Button
  {
    DISMISS,
    OK,
    UP_ARROW,
    DOWN_ARROW,
    
    BUTTON_COUNT
  };
  
  Unit* unit;
  
  void draw() override;
  void drawPost() override { }
  
public:
  UnitDetailView(ViewManager* gvm);
  
  void activate() override { }
  void deactivate() override { }
  
  void setUnit(Unit* unit);
};

#endif
