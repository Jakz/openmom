//
//  Gfx.h
//  OpenMoM
//
//  Created by Jack on 7/18/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _GFX_H_
#define _GFX_H_

#include "common/Common.h"

#include <vector>
#include "SDL.h"
#include "SDL_image.h"

enum class TextureID : u16;

#define WIDTH (320)
#define HEIGHT (200)

struct SurfaceWrapper : SpriteSheet
{
private:
  SDL_Surface* data;
  u32* pixels;
 
public:
  SurfaceWrapper(u16 w, u16 h) :
  data(SDL_CreateRGBSurface(0, w, h, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000)),
  pixels(static_cast<u32*>(data->pixels))
  {
  }
  
  ~SurfaceWrapper()
  {
    SDL_FreeSurface(data);
  }
  
  index_t tw() const override { return data->w; }
  index_t th() const override { return data->h; }
  
  index_t sw(index_t r = 0, index_t c = 0) const override { return data->w; }
  index_t sh(index_t r = 0, index_t c = 0) const override { return data->h; }
  
  void lock() const override { SDL_LockSurface(data); }
  void unlock() const override { SDL_UnlockSurface(data); }
  
  u32 at(index_t x, index_t y, index_t c = 0, index_t r = 0) const override { return pixels[x + y*data->w]; }
  void set(index_t x, index_t y, Color c) override { pixels[x + y*data->w] = c; }
  
  friend class Gfx;
};

class CursorManager
{
public:
  static void hideCursor();
  static void showDefaultCursor();
};

class Gfx
{
  static SurfaceWrapper *buffer, *canvas, *activeBuffer;

  static const ColorMap* map;
  static const Palette* palette;

  static Color get(SDL_Surface* surface, u16 x, u16 y) { return static_cast<Color*>(surface->pixels)[x + y*surface->w]; }
  static void set(SDL_Surface* surface, u16 x, u16 y, Color color) { static_cast<Color*>(surface->pixels)[x + y*surface->w] = color; }

  static const SpriteSheet* sheet(SpriteInfo info);
  
private:
  template<typename OutlineGfx> static void drawOutline(const SpriteSheet* src, s16 x, s16 y, s16 r, s16 c, const OutlineGfx& mode);

public:
  static void init();
  static void deinit();
  
  static const SDL_PixelFormat* format() { return canvas->data->format; }

  static inline const u32 color(u8 r, u8 g, u8 b) { return (255 << 24) | (r << 16) | (g << 8) | b; }
  static inline const u32 colora(u8 r, u8 g, u8 b, u8 a) { return (a << 24) | (r << 16) | (g << 8) | b; }

  static inline void bindCanvas() { activeBuffer = canvas; }
  static inline void bindBuffer() { activeBuffer = buffer; }

  static inline void bindColorMap(const ColorMap *map) { Gfx::map = map; }
  static inline void unbindColorMap() { Gfx::map = nullptr; }

  static inline void bindPalette(SpriteInfo info) { bindPalette(info.palette()); }
  static inline void bindPalette(const Palette* palette) { Gfx::palette = palette; }
  static inline void unbindPalette() { Gfx::palette = nullptr; }

  static void alphaBlend(const SDL_Rect& r, Color color);

  static void fillRect(const Rect& rect, Color color) { fillRect(rect.origin.x, rect.origin.y, rect.size.w, rect.size.h, color); }
  static void fillRect(u16 x, u16 y, u16 w, u16 h, Color color);
  static void rect(u16 x, u16 y, u16 w, u16 h, Color color);
  static void drawPixel(Color color, u16 x, u16 y);
  static void drawLine(Color color, u16 x1, u16 y1, u16 x2, u16 y2);
  
  static void canvasBlit(SpriteSheet* gsrc, u16 fx, u16 fy, u16 tx, u16 ty, u16 w, u16 h, u16 c = 0, u16 r = 0) { blit(gsrc, canvas, fx, fy, tx, ty, w, h); }

  static void blit(const SpriteSheet* gsrc, SpriteSheet* gdst, u16 fx, u16 fy, u16 tx, u16 ty, u16 w, u16 h, u16 r = 0, u16 c = 0) { rawBlit(gsrc,gdst,fx,fy,tx,ty,w,h,r,c); }
  static void rawBlit(const SpriteSheet* gsrc, SpriteSheet* gdst, u16 fx, u16 fy, s16 tx, s16 ty, u16 w, u16 h, u16 r = 0, u16 c = 0);

  static void resetBuffer(u16 w = WIDTH, u16 h = HEIGHT, u16 x = 0, u16 y = 0);
  static void mergeBuffer(u16 xf = 0, u16 yf = 0, u16 xt = 0, u16 yt = 0, u16 w = WIDTH, u16 h = HEIGHT, const ColorFilter* filter = nullptr);
  static void mergeBufferDownScaled(u16 xf = 0, u16 yf = 0, u16 xt = 0, u16 yt = 0, u16 w = WIDTH, u16 h = HEIGHT);

  static void drawClipped(const SpriteSheet* sheet, s16 x, s16 y, s16 fx, s16 fy, s16 w, s16 h);
  static void drawClipped(SpriteInfo info, u16 x, u16 y, s16 fx, s16 fy, s16 w, s16 h);

  static void draw(const SpriteSheet* sheet, s16 x, s16 y, u16 r = 0, u16 c = 0);
  
  static void draw(SpriteInfo info, s16 x, s16 y) { draw(info.sheet(), x, y, info.x(), info.y()); }
  static void draw(SpriteInfo info, Point coord) { draw(info, coord.x, coord.y); }
  static void draw(SpriteInfo info, const Palette* palette, Point coord) { bindPalette(palette); draw(info.sheet(), coord.x, coord.y); unbindPalette(); }

  
  static void draw(SpriteInfo info, const Palette* palette, s16 x, s16 y, u16 f) { bindPalette(palette); draw(info.sheet(), x, y, 0, f); unbindPalette(); }
  static void draw(SpriteInfo info, const Palette* palette, s16 x, s16 y) { bindPalette(palette); draw(info.sheet(), x, y, info.x(), info.y()); unbindPalette(); }

  static void drawGlow(SpriteInfo info, s16 x, s16 y, School color);
  static void drawSolidOutline(SpriteInfo info, s16 x, s16 y, Color color);
  
  static void drawGrayScale(SpriteInfo info, s16 x, s16 y) { drawGrayScale(info.sheet(), info.x(), info.y(), x, y); }
  static void drawGrayScale(const SpriteSheet* src, u16 r, u16 c, u16 x, u16 y);
  
  static void drawAnimated(SpriteInfo info, Point c, s16 offset = 0, s16 animFactor = 1) { drawAnimated(info, c.x, c.y, offset, animFactor); }
  static void drawAnimated(SpriteInfo info, u16 x, u16 y, s16 offset = 0, s16 animFactor = 1);
  
  static void rawDraw(SpriteInfo info, u16 x, u16 y);

  static u16 upTo(const std::vector<u16>& ws, u16 i) { u16 r = 0; for (u16 j = 0; j < i; ++j) r += ws[j]; return r; }

  static SDL_Surface *createSurface(u16 w, u16 h) { return SDL_CreateRGBSurface(0, w, h, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000); }

  static void saveScreenshot(const char* path);
  static inline void lock(const SpriteSheet *surface) { surface->lock(); }
  static inline void unlock(const SpriteSheet *surface) { surface->unlock(); }

  static inline SDL_Surface* getCanvas() { return canvas->data; }

  static u32 ticks;
  static u32 fticks;

  static IndexedPalette* mainPalette;
  static IndexedPalette* mainPaletteOpaque;
  static IndexedPalette* loadPalette;
};


#endif
