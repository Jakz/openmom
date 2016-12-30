//
//  ProductionView.h
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _PRODUCTION_VIEW_H_
#define _PRODUCTION_VIEW_H_

#include "View.h"
#include "SkillDraw.h"

#include <list>
#include <string>

class City;
class Productable;
class ViewManager;

class ProductionView : public View
{
private:
  enum { OK, CANCEL, BUTTON_COUNT  };
  
  enum Side : u8 { SIDE_LEFT, SIDE_RIGHT };
  
  
  City* city;
  std::list<const Productable*> left, right;
  s16 selected;
  Side which;
  std::string unlockedProductableString;
  SkillDraw skillDraw;
  
  void draw() override;
  void drawPost() override { }
  
public:
  ProductionView(ViewManager* gvm);
  
  void activate() override { }
  void deactivate() override { }
  
  void mousePressed(u16 x, u16 y, MouseButton b) override;
  
  void setCty(City* city);
};

#endif
