package com.github.jakz.openmom.data.effect;

import java.util.List;

public class CompoundEffect extends Effect
{
  protected CompoundEffect()
  {
    super(EffectType.compound);
  }

  public List<Effect> effects;
}
