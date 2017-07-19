package com.github.jakz.openmom.lbx;

import java.util.HashMap;
import java.util.Map;

import com.github.jakz.openmom.data.SpriteInfoLBX;

public class SpriteCache
{
  private final Map<SpriteInfoLBX, SpriteSheet> cache;
  
  public SpriteCache()
  {
    cache = new HashMap<>();
  }
  
  public SpriteSheet get(SpriteInfoLBX si)
  {
    return cache.computeIfAbsent(si, k -> LBX.loadSprite(k.lbx, k.index));
  }
  
  public SpriteSheet get(String lbx, int index)
  {
    return get(new SpriteInfoLBX(lbx, index));
  }
  
  public final static SpriteCache instance = new SpriteCache();
}
