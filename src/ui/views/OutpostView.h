//
//  OutpostView.h
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _OUTPOST_VIEW_H_
#define _OUTPOST_VIEW_H_

#include "View.h"

class City;
class ViewManager;

class OutpostView : public View
{
private:
  enum
  {
    OPTIONS_OK,
    OPTIONS_CANCEL,
    OPTIONS_DIFFICULTY,
    OPTIONS_OPPONENTS,
    OPTIONS_LAND_SIZE,
    OPTIONS_MAGIC,
    
    BUTTON_COUNT
  };
  
  void draw() override;
  void drawPost() override { }
  
  constexpr static const u16 BX = 30+185;
  constexpr static const u16 BY = 50+30;
  
  const City* city;
  
public:
  OutpostView(ViewManager* gvm);
  
  bool mouseReleased(u16 x, u16 y, MouseButton b) override;
  
  void activate() override { }
  void deactivate() override { }
  
  void setCity(const City* city) { this->city = city; }
};

#endif
