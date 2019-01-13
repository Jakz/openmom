package com.github.jakz.openmom.data.effect;

public class Effect
{
  EffectType type;
  
  protected Effect(EffectType type)
  {
    this.type = type;
  }
  
  public String toString()
  {
    return "(" + type.toString() + ")";
  }
  
  public static Effect unknown() { return new Effect(EffectType.unknown); }
}
