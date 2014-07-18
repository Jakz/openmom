//
//  Gfx.cpp
//  OpenMoM
//
//  Created by Jack on 7/18/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "Gfx.h"

SDL_Surface* Gfx::buffer = nullptr;
SDL_Surface* Gfx::activeBuffer = nullptr;
SDL_Surface* Gfx::canvas = nullptr;

u32 Gfx::ticks = 0;
u32 Gfx::realTicks = 0;

ColorMap* Gfx::map = nullptr;



void Gfx::init()
{
  buffer = createSurface(320, 200);
  canvas = createSurface(320, 200);
  activeBuffer = canvas;
}



void Gfx::rect(u16 x, u16 y, u16 w, u16 h, u32 color)
{
  lock(canvas);
  
  u32 *p = static_cast<u32*>(canvas->pixels);
  
  for (int i = 0; i < w; ++i)
  {
    p[y*canvas->pitch + i + x] = color;
    p[(y+h)*canvas->pitch + i + x] = color;
  }
  
  for (int i = 0; i < h; ++i)
  {
    p[(y+i)*canvas->pitch + x] = color;
    p[(y+i)*canvas->pitch + x + w] = color;
  }
  
  unlock(canvas);
}

#define ALPHA_SHIFT (1)
void Gfx::rawBlit(SDL_Surface *gsrc, SDL_Surface *gdst, u16 fx, u16 fy, u16 tx, u16 ty, u16 w, u16 h)
{
  lock(gsrc);
  lock(gdst);
  
  u16 sw = gsrc->w, dw = gdst->w;
  u32* sp = static_cast<u32*>(gsrc->pixels), *dp = static_cast<u32*>(gdst->pixels);
  
  u32 bs = fy*sw + fx, bd = ty*dw + tx;
  
  for (u32 y = 0; y < h; ++y)
  {
    for (u32 x = 0; x < w; ++x)
    {
      if (x+tx < dw && y+ty < gdst->h && x+tx >= 0 && y+ty >= 0)
      {
        u32 cs = bs + y*sw + x;
        u32 cd = bd + y*dw + x;
        
        u32 ps = sp[cs];
        
        if (ps == 0xFF00FF00)
        {
          int pd = dp[cd];
          
          int r = (pd & 0x00FF0000) >> 16;
          int g = (pd & 0x0000FF00) >> 8;
          int b = (pd & 0x000000FF);
          
          r >>= ALPHA_SHIFT;
          g >>= ALPHA_SHIFT;
          b >>= ALPHA_SHIFT;
          
          dp[cd] = 0xFF000000 | (r << 16) | (g << 8) | b;
        }
        else
        {
          // colorMap present
          if (map)
            ps = map->get(ps);
          
          int sa = ((ps & 0xFF000000) >> 24);
          
          if (sa == 0xFF)
            dp[cd] = ps;
          else if (sa == 0x00)
            continue;
          else
          {
            int da = 256 - sa;
            
            int pd = dp[cd];
            int r2 = (pd & 0x00FF0000) >> 16;
            int g2 = (pd & 0x0000FF00) >> 8;
            int b2 = (pd & 0x000000FF);
            
            int r1 = (ps & 0x00FF0000) >> 16;
            int g1 = (ps & 0x0000FF00) >> 8;
            int b1 = (ps & 0x000000FF);
            
            dp[cd] = 0xFF000000 | (((r2*da + r1*sa) >> 8) << 16) | (((g2*da + g1*sa) >> 8) << 8) | ((b2*da + b1*sa) >> 8);
          }
        }
      }
    }
  }
  
  unlock(gsrc);
  unlock(gdst);
}

void Gfx::drawPixel(u32 color, u16 x, u16 y)
{
  lock(activeBuffer);
  static_cast<u32*>(activeBuffer->pixels)[y*activeBuffer->pitch + x] = color;
  unlock(activeBuffer);
}

void Gfx::drawLine(u32 color, u16 x1, u16 y1, u16 x2, u16 y2)
{
  // TODO
}

void Gfx::resetBuffer(u16 w, u16 h)
{
  lock(buffer);
  u32* pixels = static_cast<u32*>(buffer->pixels);
  for (int yy = 0; yy < h; ++yy)
    for (int xx = 0; xx < w; ++xx)
      pixels[xx+buffer->pitch*yy] = 0x00000000;
  unlock(buffer);
}
