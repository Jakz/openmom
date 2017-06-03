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
#include "EventListener.h"

#include "Pathfind.h"
#include "Gfx.h"

#include <list>

class LocalPlayer;

namespace anims
{
  class Animation : public EventListener
  {
  protected:
    bool forceFinished;
    std::unique_ptr<Animation> nextAnim;
    
    virtual void step() = 0;

  public:
    Animation() : forceFinished(false) { }
    
    virtual bool hasFinished() = 0;
    
    std::unique_ptr<Animation>& next() { return nextAnim; }
    void setNext(Animation *animation) { this->nextAnim = std::unique_ptr<Animation>(animation); }
    
    void finish() { forceFinished = true; }
    
    virtual void reset() = 0;
    
    virtual void doStep() = 0;
    
    virtual ~Animation() { }
  };
  
  class InstantAnimation : public Animation
  {
  protected:
    bool _called;
    
  public:
    InstantAnimation() : _called(false) { }
    
    void reset() override { _called = false; }
    void doStep() override { _called = true; }
    bool hasFinished() override { return _called; }
  };
  
  class LambdaAnimation : public InstantAnimation
  {
  protected:
    std::function<void(void)> lambda;
    
  public:
    LambdaAnimation(std::function<void(void)> lambda) : lambda(lambda) { }
    
    void doStep() override {
      lambda();
      InstantAnimation::doStep();
    }
  };
  
  class DiscreteAnimation : public Animation
  {
  protected:
    u32 start;
    u32 duration;
    u32 elapsed;
    
  public:
    DiscreteAnimation(u32 duration) : start(Gfx::fticks), duration(duration), elapsed(0) { }
    void reset() override { start = Gfx::fticks; }
    bool hasFinished() override { return forceFinished || elapsed >= duration; }
    
    void doStep() override
    {
      elapsed = Gfx::fticks - start;
      if (!hasFinished())
        step();
    }
  };
  
  class ContinuousAnimation : public Animation
  {
  protected:
    u32 start;
    u32 duration;
    
  public:
    ContinuousAnimation(u32 duration) : start(Gfx::ticks), duration(duration) { }
    
    void reset() override { start = Gfx::ticks; }
    
    float position() {
      return std::min(1.0f, (Gfx::ticks - start) / (float)duration);
    }
    
    void doStep() override { step(); }
    bool hasFinished() override { return forceFinished || Gfx::ticks > duration + start; }
  };
  
  class ContinuousEndlessAnimation : public ContinuousAnimation
  {
  protected:
    
  public:
    ContinuousEndlessAnimation(u32 duration) : ContinuousAnimation(duration) { }
    
    void reset() override { start = Gfx::ticks; }
    
    bool hasFinished() override { return forceFinished; }
  };
  
  class Blink : public ContinuousAnimation
  {
  private:
    const Color color;
    const SDL_Rect rect;
    u8 maxAlpha;
    
    // TODO: true colors and maybe check duration/alpha
    static const Color SCHOOL_COLORS[];
    
  public:
    Blink(School school) : Blink(800, SCHOOL_COLORS[school], {0,0,320,200}, 220) { }
    
    Blink(Color color, SDL_Rect rect, u8 maxAlpha) : Blink(800, color, rect, maxAlpha) { }
    Blink(u32 duration, Color color, SDL_Rect rect, u8 maxAlpha) : ContinuousAnimation(duration), color(color), rect(rect), maxAlpha(maxAlpha) { }
    
    void step() override;
  };
  
  class UnitMovement : public ContinuousAnimation
  {
  private:
    s16 tx, ty;
    s16 sx, sy;
    const Army* army;
    std::list<pathfind::Route::step_type> moves;
    LocalPlayer *player;
    
  public:
    UnitMovement(LocalPlayer* player, const Army* army, const decltype(moves)& moves);
    
    void step() override;
    bool hasFinished() override;
  };
  
  // TODO
  class CombatAttack : public ContinuousAnimation
  {
  private:
    
  public:
    CombatAttack(const combat::CombatUnit* unit) : ContinuousAnimation(300) { }
    void step() override { }
  };
  
  class CombatMovement : public ContinuousAnimation
  {
  private:
    
  public:
    CombatMovement(const combat::CombatUnit* unit) : ContinuousAnimation(500) { }
    void step() override { }
  };
  /*
   class CombatUnitAttackAnimation
   {
   private:
   s16 sx, sy;
   s16 facing;
   const CombatUnit* unit;
   
   public:
   CombatUnitAttack(Comat)
   
   };
   */
}

#include "SpellEffectAnim.h"

#endif
