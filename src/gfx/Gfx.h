//
//  Gfx.h
//  OpenMoM
//
//  Created by Jack on 7/18/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _GFX_H_
#define _GFX_H_

#include "Common.h"
#include "ColorMap.h"

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

enum TextureID : u16;

#define WIDTH (320)
#define HEIGHT (200)

class Gfx
{
    static SDL_Surface *buffer, *canvas, *activeBuffer;
    static u32 ticks, realTicks;
  
    static const ColorMap* map;
  
  public:
    static void init();
    static void deinit();
  
    static inline const u32 color(u8 r, u8 g, u8 b) { return (255 << 24) | (r << 16) | (g << 8) | b; }
    static inline const u32 colora(u8 r, u8 g, u8 b, u8 a) { return (a << 24) | (r << 16) | (g << 8) | b; }
  
    static inline void bindCanvas() { activeBuffer = canvas; }
    static inline void bindBuffer() { activeBuffer = buffer; }
  
    static inline void bindColorMap(const ColorMap *map) { Gfx::map = map; }
    static inline void unbindColorMap() { Gfx::map = nullptr; }
  
    static void rect(u16 x, u16 y, u16 w, u16 h, u32 color);
  
    static void blit(SDL_Surface* gsrc, SDL_Surface* gdst, u16 fx, u16 fy, u16 tx, u16 ty, u16 w, u16 h) { rawBlit(gsrc,gdst,fx,fy,tx,ty,w,h); }
    static void rawBlit(SDL_Surface* gsrc, SDL_Surface* gdst, u16 fx, u16 fy, u16 tx, u16 ty, u16 w, u16 h);
  
    static void drawPixel(u32 color, u16 x, u16 y);
    static void drawLine(u32 color, u16 x1, u16 y1, u16 x2, u16 y2);
  
    static void resetBuffer(u16 w = WIDTH, u16 h = HEIGHT);
    static void mergeBuffer(u16 xf = 0, u16 yf = 0, u16 xt = 0, u16 yt = 0, u16 w = WIDTH, u16 h = HEIGHT) { blit(buffer, canvas, xf, yf, xt, yt, w, h); }
    static void maskBuffer(TextureID texture, int r, int c);
    static void colorMapBuffer(int w, int h, ColorMap& map);
    static void maskBufferWithImage(TextureID mask, TextureID snd, u16 r, u16 c, u16 r2, u16 c2);
  
    static inline void drawClippedToWidth(TextureID texture, s16 r, s16 c, s16 x, s16 y, s16 t);
    static inline void drawClippedFromWidth(TextureID texture, s16 r, s16 c, s16 x, s16 y, s16 t);
    static inline void drawClippedFromHeight(TextureID texture, s16 r, s16 c, s16 x, s16 y, s16 t);
    static void drawClipped(TextureID texture, s16 x, s16 y, s16 fx, s16 fy, s16 w, s16 h);
  
    static void drawGrayScale(TextureID texture, u16 r, u16 c, u16 x, u16 y);
    //   public static void drawGlow(Texture texture, int r, int c, int x, int y, School color)
    //  public static void drawGlow(Texture texture, int i, int x, int y, School school)
    //  public static void drawGlow(SpriteInfo info, int x, int y, School school)
    // public static void drawGrayScale(SpriteInfo info, int x, int y)
    static void draw(SpriteInfo& info, int x, int y);
  
    static void rawDraw(TextureID texture, u16 r, u16 c, u16 x, u16 y);
    static void draw(TextureID texture, u16 x, u16 y);
    static void draw(TextureID texture, u16 i, u16 x, u16 y);
    static void draw(TextureID texture, u16 r, u16 c, u16 x, u16 y);
  
    static void drawAnimated(TextureID texture, u16 r, u16 x, u16 y, s16 offset);

  
  
    static u32 getRealTicks() { return realTicks; }
  
  
    static u16 upTo(const std::vector<u16>& ws, u16 i) { u16 r = 0; for (u16 j = 0; j < i; ++j) r += ws[j]; return r; }
  
    static SDL_Surface *createSurface(u16 w, u16 h) { return SDL_CreateRGBSurface(0, w, h, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000); }
  
    static inline void lock(SDL_Surface *surface) { SDL_LockSurface(surface); }
    static inline void unlock(SDL_Surface *surface) { SDL_UnlockSurface(surface); }
  
    static inline SDL_Surface* getCanvas() { return canvas; }
};


#endif
