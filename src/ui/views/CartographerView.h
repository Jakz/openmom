//
//  CartographerView.h
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#pragma once

#include "common/Common.h"
#include "View.h"

class CartographerView : public ViewWithQueue
{
private:
  Plane plane;
  
  void draw() override;
  void drawPost() override { }
  
  void setup();
  
  inline Point positionForTile(u16 x, u16 y) { return Point(16 + x*4,27 + y*4); }
  
  void drawCity(const City* city);
  
  void drawLegend(u32 row, PlayerColor color, const std::string& name);
  
public:
  CartographerView(ViewManager* gvm);
  
  void activate() override;
  void deactivate() override { }
};
