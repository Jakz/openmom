//
//  CitiesView.h
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#pragma once
#include "View.h"

#include <vector>

class ViewManager;

class City;

class CitiesView : public View
{
private:
  enum
  {
    OK,
    PREV1,
    NEXT1,
    PREV2,
    NEXT2,
    
    BUTTON_COUNT
  };
  
  s16 offset;
  const City* city;
  
  void draw() override;
  void drawPost() override { }
  
  std::vector<const City*> cities;
  
public:
  CitiesView(ViewManager* gvm);
  
  void activate() override;
  void deactivate() override { offset = 0; }
  
  bool mouseMoved(u16 x, u16 y, MouseButton b) override;
  
  void scrollDown() { ++offset; }
  void scrollUp() { --offset; }
  void updateScrollButtons();
};
