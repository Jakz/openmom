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

#include "PathFind.h"
#include "Gfx.h"

#include <list>

class CombatUnit;
class LocalPlayer;

namespace anims
{
  class Animation
  {
    protected:
      Animation *nextAnim;

    public:
      Animation() : nextAnim(nullptr) { }
    
      virtual bool hasFinished() = 0;
      virtual void step() = 0;
    
      Animation* next() { return nextAnim; }
      void setNext(Animation *animation) { this->nextAnim = animation; }
    
      virtual void reset() = 0;
    
      virtual ~Animation() { }
  };

  class DiscreteAnimation : public Animation
  {
  protected:
    u32 start;
    u32 duration;

  public:
    DiscreteAnimation(u32 duration) : start(Gfx::fticks), duration(duration) { }
    void reset() override { start = Gfx::fticks; }
    bool hasFinished() override { return Gfx::fticks >= duration + start; }
  };

  class ContinuousAnimation : public Animation
  {
    protected:
      u32 start;
      u32 duration;
    
    public:
      ContinuousAnimation(u32 duration) : start(Gfx::ticks), duration(duration) { }
    
      void reset() { start = Gfx::ticks; }
    
      float position() {
        return std::min(1.0f, (Gfx::ticks - start) / (float)duration);
      }
    
      bool hasFinished() override { return Gfx::ticks > duration + start; }
  };

  class Blink : public ContinuousAnimation
  {
  private:
    const Color color;
    const SDL_Rect rect;
    u8 maxAlpha;
    
    // TODO: true colors and maybe check duration/alpha
    static constexpr const Color SCHOOL_COLORS[] = {RGB(0, 0, 0), RGB(255,0,0), RGB(0,0,0), RGB(255,255,255), RGB(0,255,0), RGB(0,0,255)};
    
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
    std::list<PathTileInfo> moves;
    LocalPlayer *player;
    
  public:
    UnitMovement(LocalPlayer* player, const Army* army, const std::list<PathTileInfo>& moves);

    void step() override;
    bool hasFinished() override;
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

#endif
