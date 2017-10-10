package com.github.jakz.openmom.data;

import java.util.List;

public class Race
{
  public String identifier;
  public int cityGrowthModifier;
  public float outpostGrowthChance;
  public int baseProduction;
  public int foodProductionPerFarmer;
  public float taxIncomeMultiplier;
  public float miningBonusMultiplier;
  public float manaProducedPerCitizen;
  public List<String> cityNames;
  
  public Visuals visuals;
  
  public static class Visuals
  {
    public HouseType houseType;
    public Graphics graphics;
    public String i18n;
    public String i18nUnit;
        
    public static class Graphics
    {
      public SpriteInfo worker, farmer, rebel;
    }
  }
  
  
}
