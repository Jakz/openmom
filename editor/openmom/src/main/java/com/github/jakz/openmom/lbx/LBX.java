package com.github.jakz.openmom.lbx;

import com.pixbits.lib.ui.color.Palette;

public class LBX
{
  private final static String BASE = "../../data/lbx/";
  
  public static SpriteSheet loadSprite(String lbxfile, int index)
  {
    return internalLoadSprite(BASE+lbxfile, index);
  }
  
  public static native void init();
  public static native void clearCache();
  private static native SpriteSheet internalLoadSprite(String path, int index);
}
