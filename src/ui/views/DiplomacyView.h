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

class DiplomacyView : public ViewWithQueue
{
private:
  enum Phase
  {
    FADING_IN,
    SPEAKING,
    WAIT
  };
  
  enum Mood
  {
    HAPPY,
    ANGRY,
    NEUTRAL
  };
  
  void draw() override;
  void drawPost() override { }
    
  s32 eyesColorLevel;
  
  void setEyeColor(s32 color);
  
  void setup();
  
  
  DiscreteTimer timer;
  
  const Wizard* wizard;
  Phase phase;
  Mood mood;

public:
  DiplomacyView(ViewManager* gvm);
  
  void activate() override;
  void deactivate() override { }
  
  bool mouseReleased(u16 x, u16 y, MouseButton b) override;
};
