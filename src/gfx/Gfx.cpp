//
//  Gfx.cpp
//  OpenMoM
//
//  Created by Jack on 7/18/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "Gfx.h"

#include "Texture.h"

SDL_Surface* Gfx::buffer = nullptr;
SDL_Surface* Gfx::activeBuffer = nullptr;
SDL_Surface* Gfx::canvas = nullptr;

u32 Gfx::ticks = 0;
u32 Gfx::realTicks = 0;

const ColorMap* Gfx::map = nullptr;



void Gfx::init()
{
  buffer = createSurface(320, 200);
  canvas = createSurface(320, 200);
  activeBuffer = canvas;
}

void Gfx::deinit()
{
  SDL_FreeSurface(buffer);
  SDL_FreeSurface(canvas);
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

void Gfx::maskBuffer(TextureID texture, int r, int c)
{
  const Texture& tex = Texture::get(texture);
  
  lock(tex.img);
  lock(buffer);
  
  u32* mp = static_cast<u32*>(tex.img->pixels);
  u32* dp = static_cast<u32*>(buffer->pixels);
  
  int w = tex.w;
  int h = tex.h;
  
  int ox = c*w;
  int oy = r*h;
  
  for (int i = 0; i < w; ++i)
    for (int j = 0; j < h; ++j)
    {
      int p = mp[ox + oy + i + j*tex.img->pitch];
      
      if ((p & 0xFF000000) == 0)
        dp[i + j*buffer->pitch] = 0x0000000;
    }
  
  unlock(buffer);
  unlock(tex.img);
}

void Gfx::colorMapBuffer(int w, int h, ColorMap& map)
{
  lock(buffer);
  u32* pixels = static_cast<u32*>(buffer->pixels);
  for (int yy = 0; yy < h; ++yy)
    for (int xx = 0; xx < w; ++xx)
      pixels[xx+buffer->pitch*yy] = map.get(pixels[xx+buffer->pitch*yy]);
  
  unlock(buffer);
}

void Gfx::maskBufferWithImage(TextureID mask, TextureID snd, u16 r, u16 c, u16 r2, u16 c2)
{
  const Texture& tex1 = Texture::get(mask), &tex2 = Texture::get(snd);
  
  lock(tex1.img);
  lock(tex2.img);
  lock(buffer);
  
  u32* mp = static_cast<u32*>(tex1.img->pixels);
  u32* dp = static_cast<u32*>(buffer->pixels);
  u32* sp = static_cast<u32*>(tex2.img->pixels);
  
  u16 w = tex1.img->w;
  u16 h = tex1.img->h;
  
  u16 ox = c*w;
  u16 oy = r*h;
  
  u16 ox2 = c2*w;
  u16 oy2 = r2*h;
  
  for (int i = 0; i < w; ++i)
    for (int j = 0; j < h; ++j)
    {
      int p = mp[ox + oy + i + j*tex1.img->pitch];
      
      if ((p & 0xFF000000) == 0)
        dp[i + j*buffer->pitch] = sp[ox2+oy2+i+j*tex2.img->pitch];
    }

  unlock(tex1.img);
  unlock(tex2.img);
  unlock(buffer);
}



void Gfx::drawClippedToWidth(TextureID texture, s16 r, s16 c, s16 x, s16 y, s16 t)
{
  const Texture& tex = Texture::get(texture);
  drawClipped(texture, x, y, r*tex.w, c*tex.h, t, tex.h);
}

void Gfx::drawClippedFromWidth(TextureID texture, s16 r, s16 c, s16 x, s16 y, s16 t)
{
  const Texture& tex = Texture::get(texture);
  drawClipped(texture, x, y, r*tex.w + t, c*tex.h, tex.w - t, tex.h);
}

void Gfx::drawClippedFromHeight(TextureID texture, s16 r, s16 c, s16 x, s16 y, s16 t)
{
  const Texture& tex = Texture::get(texture);
  drawClipped(texture, x, y, r*tex.w, c*tex.h + t, tex.w, tex.h - t);
}

void Gfx::drawClipped(TextureID texture, s16 x, s16 y, s16 fx, s16 fy, s16 w, s16 h)
{
  const Texture& tex = Texture::get(texture);
  s16 tw = w != 0 ? (w > 0 ? w : tex.w + w - fx) : tex.w - fx;
  s16 th = h != 0 ? (h > 0 ? h : tex.h + h - fy) : tex.h - fy;
  s16 tx = fx;
  s16 ty = fy;
  s16 dx = x;
  s16 dy = y;
  
  blit(tex.img, activeBuffer, tx, ty, dx, dy, tw, th);
}



void Gfx::rawDraw(TextureID texture, u16 r, u16 c, u16 x, u16 y)
{
  const Texture& tex = Texture::get(texture);
  blit(tex.img, canvas, tex.w*c, tex.h*r, x, y, tex.w, tex.h);
}

void Gfx::draw(TextureID texture, u16 x, u16 y)
{
  const Texture& tex = Texture::get(texture);
  blit(tex.img, activeBuffer, 0, 0, x, y, tex.w, tex.h);
}

void Gfx::draw(TextureID texture, u16 i, u16 x, u16 y)
{
  const Texture& tex = Texture::get(texture);
  draw(texture, i / tex.cols, i % tex.cols, x, y);
}

void Gfx::draw(TextureID texture, u16 r, u16 c, u16 x, u16 y)
{
  const Texture& tex = Texture::get(texture);
  u16 tx = x;
  u16 ty = y;
  u16 tw = 0, th = 0;
  u16 sx = 0, sy = 0;
  
  if (tex.w != -1)
  {
    tw = tex.w;
    sx = tw*c;
  }
  else
  {
    if (tex.h != -1)
    {
      tw = tex.ws[c];
      sx = upTo(tex.ws, c);
    }
    else
    {
      tw = tex.ws[r];
      sx = tw*c;
    }
  }
  
  if (tex.h != -1)
  {
    th = tex.h;
    sy = th*r;
  }
  else
  {
    th = tex.hs[r];
    sy = upTo(tex.hs, r);
  }
  
  blit(tex.img, activeBuffer, sx, sy, tx, ty, tw, th);
}

void Gfx::drawAnimated(TextureID texture, u16 r, u16 x, u16 y, s16 offset)
{
  const Texture& tex = Texture::get(texture);
  
  if (tex.animatedSprites.empty())
    draw(texture, r, (((offset+ticks)/tex.animFactor)%tex.cols), x, y);
  else
    draw(texture, r, (((offset+ticks)/tex.animFactor)%tex.animatedSprites[r]), x, y);
}

void Gfx::draw(SpriteInfo& info, int x, int y)
{
  draw(info.texture, info.x, info.y, x, y);
}