//
//  CityView.h
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _CITY_VIEW_H_
#define _CITY_VIEW_H_

#include "View.h"

class ViewManager;

class CityView : public View
{
private:
  enum Button
  {
    BUY,
    CHANGE,
    OK,
    
    BUTTON_COUNT
  };
  
  void draw() override { };
  void drawPost() override { }
  
public:
  CityView(ViewManager* gvm);
  
  void activate() override { }
  void deactivate() override { }
  
  friend class ProductionView;
};

#endif