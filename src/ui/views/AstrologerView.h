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

class AstrologerView : public ViewWithQueue
{
private:
  
  void draw() override;
  void drawPost() override { }
  
  void setup();
  
  Point dialogBase();
  
  void drawRow(int section, int row, const std::string& caption, float percent, PlayerColor color);

public:
  AstrologerView(ViewManager* gvm);
  
  void activate() override;
  void deactivate() override { }
  
  bool mouseReleased(u16 x, u16 y, MouseButton b) override;
};
