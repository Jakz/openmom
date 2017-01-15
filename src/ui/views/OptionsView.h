//
//  OptionsView.h
//  OpenMoM
//
//  Created by Jack on 1/15/17.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#pragma once

#include "View.h"

class City;
class ViewManager;

class OptionsView : public View
{
private:
  void draw() override;
  void drawPost() override { }
  
public:
  OptionsView(ViewManager* gvm);
  
  void mouseReleased(u16 x, u16 y, MouseButton b) override;
  
  void activate() override { }
  void deactivate() override { }
};
