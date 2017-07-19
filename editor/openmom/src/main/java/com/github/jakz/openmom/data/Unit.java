package com.github.jakz.openmom.data;

import java.util.List;

public class Unit
{
  public Unit()
  {
    visuals = new Visuals();
  }
  
  public String identifier;
  public UnitType type;
  public int upkeep;
  public int cost;
  public int melee;
  public Ranged ranged;
  public int defense;
  public int resistance;
  public int hits;
  public int figures;
  public int movement;
  public int sight;
  
  public List<Skill> skills;
  
  public Visuals visuals;
  
  public static class Visuals
  {
    public String i18n;
    public SpriteInfo icon;
    public SpriteInfo figure;
  }
  
}
