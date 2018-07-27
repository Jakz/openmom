//
//  ResearchView.h
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#pragma once

#include "View.h"

class IntroView : public View
{
private:
  enum class Phase
  {
    BLEND_IN,
    BLEND_OUT,
    ANIMATE
  };
  
  DiscreteTimer timer;
  Phase phase;
  u32 currentFrameCount;
  u32 currentFrame;
  
  void setNextStep(Phase phase, u32 frame);
  
  void draw() override;
  void drawPost() override { }
  
public:
  IntroView(ViewManager* gvm);
  
  void activate() override;
  void deactivate() override;
};
