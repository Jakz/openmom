//
//  ResearchView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "IntroView.h"

#include "LBXRepository.h"
#include "Gfx.h"
#include "ViewManager.h"

constexpr u32 BLEND_DURATION = 10;
constexpr u32 FRAME_COUNT = 13;

bool shouldBlendFrame(u32 frame) { return frame <= 2; }

IntroView::IntroView(ViewManager* gvm) : View(gvm), timer(2), currentFrameCount(0), currentFrame(0)
{
  addArea(new Clickable(0, 0, WIDTH, HEIGHT))->setAction([this](){
    this->gvm->switchView(VIEW_START);
  });
}

void IntroView::activate()
{
  currentFrame = 0;
  currentFrameCount = BLEND_DURATION;
  timer.reset();
  timer.mark(Gfx::fticks);
  phase = Phase::BLEND_IN;
}

void IntroView::deactivate()
{
  lbx::Repository::clearCache(LBXID::INTRO);
}

void IntroView::setNextStep(Phase phase, u32 frame)
{
  this->phase = phase;
  currentFrame = frame;
  timer.resetCounter();
  
  switch (phase)
  {
    case Phase::BLEND_IN:
    case Phase::BLEND_OUT:
      this->currentFrameCount = BLEND_DURATION;
      break;
    case Phase::ANIMATE:
      this->currentFrameCount = SpriteInfo(LBXID::INTRO, currentFrame).count();
  }
}


void IntroView::draw()
{
  timer.set(Gfx::fticks);
  
  if (timer == currentFrameCount)
  {
    if (shouldBlendFrame(currentFrame))
    {
      switch (phase)
      {
        case Phase::BLEND_IN:
          currentFrameCount = SpriteInfo(LBXID::INTRO, currentFrame).count();
          phase = Phase::ANIMATE;
          break;
          
        case Phase::ANIMATE:
          currentFrameCount = BLEND_DURATION;
          phase = Phase::BLEND_OUT;
          break;
          
        case Phase::BLEND_OUT:
          ++currentFrame;
          phase = shouldBlendFrame(currentFrame) ? Phase::BLEND_IN : Phase::ANIMATE;
          currentFrameCount = shouldBlendFrame(currentFrame) ? BLEND_DURATION : SpriteInfo(LBXID::INTRO, currentFrame).count();
          break;
      }
    }
    else
    {
      phase = Phase::ANIMATE;
      currentFrameCount = SpriteInfo(LBXID::INTRO, currentFrame).count();
    }
    
    timer.resetCounter();
  }
  
  if (currentFrame == FRAME_COUNT)
  {
    gvm->switchView(VIEW_START);
    return;
  }
  
  const SpriteInfo step = LSI(INTRO, currentFrame);
  
  switch (phase)
  {
    case Phase::ANIMATE:
      Gfx::draw(step.frame(timer), 0, 0);
      break;
      
    case Phase::BLEND_IN:
      Gfx::draw(step, 0, 0);
      Gfx::fillRect(0, 0, WIDTH, HEIGHT, Color(0,0,0,(0xFF/BLEND_DURATION)*(BLEND_DURATION-timer)));
      break;
      
    case Phase::BLEND_OUT:
      Gfx::draw(step.frame(step.count()-1), 0, 0);
      Gfx::fillRect(0, 0, WIDTH, HEIGHT, Color(0,0,0,(0xFF/BLEND_DURATION)*timer));
      break;
  }
  
  
  //TODO: missing copyright notice
}
