package com.github.jakz.openmom.data;

public class Ranged
{
  public static enum Type
  {
    none,
    arrow,
    
    nature
  }
  
  int strength;
  Type type;
  int ammo;
  
  public Ranged(int strength, Type type, int ammo)
  {
    this.strength = strength;
    this.type = type;
    this.ammo = ammo;
  }
  
  public Ranged(Type type)
  {
    this(0, type, 0);
  }
  
  @Override
  public String toString()
  {
    if (type == Type.none)
      return "none";
    else
    {
      return strength + ", " + type + ", " + ammo;
    }
  }
}
