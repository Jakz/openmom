package com.github.jakz.openmom.data.effect;

public class ParametricAbilityEffect extends Effect
{
  public final String kind;
  public final int value;
  
  public ParametricAbilityEffect(String kind, int value)
  {
    super(EffectType.parametric_ability);
    this.kind = kind;
    this.value = value;
  }
  
  public String toString() {
    return new StringBuilder().append("(").append(type).append(", ").append(kind).append(", ").append(value).append(")").toString();
  }
}
