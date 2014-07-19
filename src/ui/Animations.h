//
//  Animations.h
//  OpenMoM
//
//  Created by Jack on 7/19/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _ANIMATIONS_H_
#define _ANIMATIONS_H_

#include "Common.h"

class Animation
{
  protected:
    Animation *nextAnim;

  public:
    Animation() : nextAnim(nullptr) { }
  
    virtual bool hasFinished(u32 ticks) = 0;
    virtual void step() = 0;
  
    Animation* next() { return nextAnim; }
    void setNext(Animation *animation) { this->nextAnim = animation; }
  
    virtual void reset(u32 ticks) = 0;
  
    virtual ~Animation() { }
};

class ContinuousAnimation : public Animation
{
  protected:
    u32 start;
    u32 duration;
  
  public:
    ContinuousAnimation(u32 start, u32 duration) : start(start), duration(duration) { }
  
    void reset(u32 ticks) { start = ticks; }
  
    float position(u32 ticks) {
      return std::min(1.0f, (ticks - start) / (float)duration);
    }
  
    bool hasFinished(u32 ticks) override { return ticks > duration + start; }
};


#endif
