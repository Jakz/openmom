#pragma once

#include "Animations.h"
#include "Common.h"
#include "CombatUtils.h"

class SummonSpec;



namespace anims
{
  class SpellEffect : public DiscreteAnimation
  {
  public:
    constexpr static u32 TICKS_PER_FRAME = 2;
    
  private:
    SpriteInfo effect;
    bool force;

    ScreenCoord coords;
    u16 counter;
    u16 frame;
    
  public:
    SpellEffect(LocalPlayer* player, SpriteInfo effectId, Position tile);
    SpellEffect(SpriteInfo effectId, combat::CombatCoord tile);
    SpellEffect(LocalPlayer* player, SpriteInfo effectId, int tx, int ty);
    
    void step() override;
    
    bool hasFinished() override
    {
      return force || DiscreteAnimation::hasFinished();
    }
  };
  
  class SummonAnimation : public ContinuousEndlessAnimation
  {
    const SummonSpec* const spec;
    SpriteInfo wizard;
    SpriteInfo creature;
    const Palette* palette;
        
  public:
    SummonAnimation(WizardID wizard, const SummonSpec* spec);
    void step() override;
    void mouseReleased(u16 x, u16 y, MouseButton b) override;
  };

  class SpellDiscoverAnimation : public ContinuousEndlessAnimation
  {
  private:
    SpriteInfo wizard;
    SpriteInfo familiar;
    
  public:
    SpellDiscoverAnimation(WizardID wizard, School school);
    void step() override;
    void mouseReleased(u16 x, u16 y, MouseButton b) override;
  };
  
  
};
