package com.github.jakz.openmom.lbx;

import java.awt.Graphics2D;
import java.awt.RenderingHints;
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
    rasterize(index, 1);
  }
  
  public void rasterize(int index, int ratio)
  {
    if (rasterized[index] == null)
    {
      BufferedImage image = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
      
      palette.set(0, new Color(0,0,0,0));
      palette.set(232, new Color(0,0,0,128));
      
      int[] cdata = new int[width*height];
      final int count = width*height;
      
      for (int i = 0; i < count; ++i)
      {
        Color c = palette.get(data[index][i]);
        cdata[i] = c.toInt();
      }
      
      image.setRGB(0, 0, width, height, cdata, 0, width);
      
      if (ratio == 1)
        rasterized[index] = image;
      else
      {
        BufferedImage nimage = new BufferedImage(width*ratio, height*ratio, BufferedImage.TYPE_INT_ARGB);
        Graphics2D g2 = nimage.createGraphics();
        g2.setRenderingHint(RenderingHints.KEY_INTERPOLATION, RenderingHints.VALUE_INTERPOLATION_NEAREST_NEIGHBOR);
        g2.drawImage(image, 0, 0, width*ratio, height*ratio, null);
        g2.dispose();
        rasterized[index] = nimage;
      }
    }
  }
  
  public BufferedImage get(int index) { return get(index, 1); }
  public BufferedImage get() { return get(0); }
  
  public BufferedImage get(int index, int ratio)
  {
    rasterize(index, ratio);
    return rasterized[index];
  }
}
