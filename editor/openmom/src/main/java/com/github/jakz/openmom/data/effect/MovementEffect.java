package com.github.jakz.openmom.data.effect;

public class MovementEffect extends Effect
{
  public String movementType;
  
  public MovementEffect(String movementType)
  {
    super(EffectType.movement);
    this.movementType = movementType;
  }
  
  public String toString()
  {
    return "(" + type.toString() +", " + movementType + ")";
  }
}
