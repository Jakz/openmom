package com.github.jakz.openmom.data;

import java.awt.image.BufferedImage;
import java.util.Objects;

import javax.swing.ImageIcon;

import com.github.jakz.openmom.lbx.SpriteCache;

public class SpriteInfoLBX implements SpriteInfo
{
  public final String lbx;
  public final int index;
  
  private BufferedImage image;
  private ImageIcon icon;
  
  public SpriteInfoLBX(String lbx, int index)
  {
    this.lbx = lbx;
    this.index = index;
  }
  
  @Override public boolean equals(Object o)
  {
    if (o instanceof SpriteInfoLBX)
    {
      SpriteInfoLBX l = (SpriteInfoLBX)o;
      return l.index == index && l.lbx.equals(lbx);
    }
    else
      return false;
  }
  
  @Override public int hashCode()
  {
    return Objects.hash(index, lbx);
  }
  
  public BufferedImage getImage(int ratio)
  {
    if (image != null)
      return image;
    else
    {
      image = SpriteCache.instance.get(this).get(0, ratio);
      return image;
    }
  }
  
  public ImageIcon getIcon(int ratio)
  {
    if (icon != null)
      return icon;
    else
    {
      icon = new ImageIcon(getImage(ratio));
      return icon;
    }
  }
  
  @Override
  public String toString() { return lbx + ", " + index; }
  
}
