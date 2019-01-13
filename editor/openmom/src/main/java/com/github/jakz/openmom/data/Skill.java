package com.github.jakz.openmom.data;

import java.util.List;

import com.github.jakz.openmom.data.effect.Effect;

public class Skill
{
  public String identifier;
  public SkillType type;
  
  public Visuals visuals;
  
  public List<Effect> effects;
  
  public static class Visuals
  {
    public SpriteInfo icon;
    public String i18n;
  }
}
