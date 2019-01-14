package com.github.jakz.openmom.data.effect;

public class SpecialAttackEffect extends Effect
{
  public final String kind;
  public final int value;
  
  public SpecialAttackEffect(String kind, int value)
  {
    super(EffectType.special_attack);
    this.kind = kind;
    this.value = value;
  }
  
  public String toString() {
    return new StringBuilder().append("(").append(type).append(", ").append(kind).append(", ").append(value).append(")").toString();
  }
}
