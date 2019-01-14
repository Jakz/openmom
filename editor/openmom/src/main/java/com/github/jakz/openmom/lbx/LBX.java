package com.github.jakz.openmom.lbx;

import com.pixbits.lib.ui.color.Palette;

public class LBX
{
  private final static String BASE = "../../data/lbx/";
  
  public static SpriteSheet loadSprite(String lbxfile, int index)
  {
    return internalLoadSprite(BASE+lbxfile, index);
  }
  
  public static int getEntryCount(String lbxFile)
  {
    return internalGetEntryCount(BASE+lbxFile);
  }
  
  public static native void init();
  public static native void clearCache();
  
  private static native SpriteSheet internalLoadSprite(String path, int index);
  public static native int internalGetEntryCount(String path);
}
