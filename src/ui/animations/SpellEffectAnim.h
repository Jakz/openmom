#pragma once

#include "Animations.h"
#include "Common.h"

namespace anims
{
  class SpellEffect : public DiscreteAnimation
  {
  public:
    const static sprite_ref schools[];
    constexpr static u32 TICKS_PER_FRAME = 2;
    
  private:
    SpriteInfo effect;
    bool force;

    ScreenCoord coords;
    u16 frame;
    
  public:
    SpellEffect(LocalPlayer* player, SpriteInfo effectId, Position tile);
    SpellEffect(LocalPlayer* player, SpriteInfo effectId, int tx, int ty);
    
    void step() override;
    
    bool hasFinished() override
    {
      return force || DiscreteAnimation::hasFinished();
    }
  };

  
  
};
