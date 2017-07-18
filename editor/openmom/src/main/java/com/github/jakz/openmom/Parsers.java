package com.github.jakz.openmom;

import java.util.List;

import com.github.jakz.openmom.data.SpriteInfoLBX;

public class Parsers
{
  private static void assertTrue(boolean condition)
  {
    if (!condition)
      throw new RuntimeException();
  }
   
  public static SpriteInfoLBX parseSpriteInfoLBX(Object yaml)
  {
    List<Object> data = (List<Object>)yaml;
    assertTrue(data.size() == 3);
    assertTrue("lbx".equals(data.get(0)));
    System.out.println(data.get(1).getClass().getName());
    System.out.println(data.get(2).getClass().getName());
    return null;
  }
}
