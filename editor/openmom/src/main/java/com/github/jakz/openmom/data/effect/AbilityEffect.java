package com.github.jakz.openmom.data.effect;

public class AbilityEffect extends Effect
{
  public String ability;
  
  public AbilityEffect(String ability)
  {
    super(EffectType.ability);
    this.ability = ability;
  }
  
  public String toString()
  {
    return "(" + type.toString() +", " + ability + ")";
  }
}
