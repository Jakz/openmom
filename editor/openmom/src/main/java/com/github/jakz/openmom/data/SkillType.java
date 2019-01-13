package com.github.jakz.openmom.data;

public enum SkillType
{
  _native,
  hero,
  spell
  ;
  
  @Override
  public String toString()
  {
    if (name().startsWith("_"))
      return name().substring(1);
    else
      return name();
  }
}
