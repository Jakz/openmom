package com.github.jakz.openmom.data.effect;

public class PropertyBonusEffect extends Effect
{
  public int value;
  public String property;
  
  public PropertyBonusEffect(EffectType type, String property, int value)
  {
    super(type);
    this.property = property;
    this.value = value;
  }

  public String toString() {
    return new StringBuilder().append("(").append(type).append(", ").append(property).append(", ").append(value).append(")").toString();
  }
}
