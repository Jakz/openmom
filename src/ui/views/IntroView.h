//
//  ResearchView.h
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#pragma once

#include "View.h"

struct DiscreteTimer
{
private:
  u32 base;
  const u32 ticksPerTick;
  u32 tickCounter;
  u32 counter;
  
public:
  DiscreteTimer(u32 ticksPerTick) :
  ticksPerTick(ticksPerTick), counter(0), base(0) { }
  
  void mark(u32 base) { this->base = base; }
  
  void set(u32 value)
  {
    tick(value - base);
    base = value;
  }
  
  void tick(u32 delta)
  {
    tickCounter += delta;
    if (tickCounter >= ticksPerTick)
      counter += (tickCounter / ticksPerTick);
    tickCounter %= ticksPerTick;
  }
  
  void tick() { tick(1); }
  
  u32 value() const { return counter; }
  operator u32() const { return counter; }
  
  void reset()
  {
    tickCounter = 0;
    counter = 0;
  }
  
  void resetCounter()
  {
    counter = 0;
  }
};

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
  
  void draw() override;
  void drawPost() override { }
  
public:
  IntroView(ViewManager* gvm);
  
  void activate() override;
  void deactivate() override;
};
