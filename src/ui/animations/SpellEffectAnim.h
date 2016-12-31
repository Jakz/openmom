#pragma once

#include "Animations.h"
#include "Common.h"

class SummonSpec;

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
  
  class SummonAnimation : public ContinuousEndlessAnimation
  {
    SpriteInfo wizard;
    SpriteInfo creature;
    
  public:
    SummonAnimation(WizardID wizard, const SummonSpec* unit);
    void step() override;
    void mouseReleased(u16 x, u16 y, MouseButton b) override;
  };

  
  
};
