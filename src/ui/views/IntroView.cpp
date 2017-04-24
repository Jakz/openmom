//
//  ResearchView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "IntroView.h"

#include "Gfx.h"
#include "ViewManager.h"

constexpr u32 TICKS_PER_FRAME = 4;

u32 framesForStep;

IntroView::IntroView(ViewManager* gvm) : View(gvm), currentFrame(0), currentStep(0), counter(0), splitCounter(0)
{

}

void IntroView::activate()
{
  currentFrame = 0;
  currentStep = 0;
  framesForStep = SpriteInfo(LSI(INTRO,3)).count();
}

void IntroView::draw()
{
  u32 fticks = Gfx::fticks;
  u32 delta = fticks - counter;
  splitCounter += delta;
  counter = fticks;
  
  const SpriteInfo step = LSI(INTRO,3);
  
  Gfx::draw(step.frame(splitCounter % framesForStep), 0, 0);
}
