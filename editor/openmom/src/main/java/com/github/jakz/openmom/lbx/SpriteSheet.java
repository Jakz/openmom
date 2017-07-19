package com.github.jakz.openmom.lbx;

import java.awt.image.BufferedImage;
import java.util.Arrays;

import com.pixbits.lib.ui.color.Color;
import com.pixbits.lib.ui.color.Palette;

public class SpriteSheet
{
  private final int count;
  private final int width;
  private final int height;
  
  private final byte[][] data;
  private final BufferedImage[] rasterized;
  
  private Palette palette;

  public SpriteSheet(int count, int width, int height)
  {
    this(count, width, height, null);
  }
  
  public SpriteSheet(int count, int width, int height, Palette palette)
  {
    this(count, width, height, palette, new byte[count][]);
  }
  
  public SpriteSheet(int count, int width, int height, Palette palette, byte[][] data)
  {
    this.count = count;
    this.width = width;
    this.height = height;
    this.data = data;
    this.rasterized = new BufferedImage[count];
    this.palette = palette;
  }
  
  public void data(int index, byte[] data) { this.data[index] = data; }
  
  public void palette(Palette palette) { this.palette = palette; }
  public Palette palette() { return palette; }
  
  public void rasterize(int index)
  {
    if (rasterized[index] == null)
    {
      BufferedImage image = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
      
      int[] cdata = new int[width*height];
      final int count = width*height;
      
      for (int i = 0; i < count; ++i)
      {
        Color c = palette.get(data[index][i]);
        cdata[i] = c.toInt();
      }
      
      image.setRGB(0, 0, width, height, cdata, 0, width);
      rasterized[index] = image;
    }
  }
  
  public BufferedImage get(int index)
  {
    rasterize(index);
    return rasterized[index];
  }
}
