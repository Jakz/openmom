//
//  Texture.h
//  OpenMoM
//
//  Created by Jack on 7/18/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "common/Common.h"
#include "ColorMap.h"

#include <string>
#include <vector>
#include <initializer_list>

class SDL_Surface;

enum class TextureID : u16
{  
  MAIN_CURSORS,
  MAIN_MIRROR,
    
  MESSAGE_LEFT,
  MESSAGE_RIGHT,
  //113 - 29
  
  CITY_ROADS,
  
  // units
  
  UNIT_DETAIL_SPECIAL_THINGS, // contiene anche mirino minimap
  UNIT_DETAIL_PROPS,
  UNIT_DETAIL_PROPS_EMPTY,
  UNIT_SKILL_ICONS,
        
    
  /* altezza max help: 200 + 23 pezzo di chiusura da sovrapporre */
  HELP_BACKDROP,
      
  COMBAT_MISC_TILES,
        
  TEXTURES_COUNT
};

class Texture : public SpriteSheet
{
private:
  static const Texture textures[];

  mutable s16 w, h;

public:
  const TextureID ident;
  const std::string name;
  mutable SDL_Surface *img;
  mutable s16 cols, rows;

  std::vector<u16> ws, hs;

  mutable bool animated;
  mutable std::vector<u16> animatedSprites;

  mutable u16 animFactor;

  Texture(TextureID ident, std::string name, u16 rows, u16 cols, u16 w, u16 h) : Texture(ident, name, rows, cols, w, h, false, 0) { }
  Texture(TextureID ident, std::string name, u16 w, u16 h) : Texture(ident, name, 1, 1, w, h, false, 0) { }
  Texture(TextureID ident, std::string name, u16 rows, u16 cols, std::initializer_list<u16> ws, std::initializer_list<u16> hs) : Texture(ident, name, rows, cols, ws, hs, false, 0) { }

  Texture(TextureID ident, std::string name, u16 rows, u16 cols, std::initializer_list<u16> ws, u16 h) :
    img(nullptr), ident(ident), name(name), rows(rows), cols(cols), w(-1), h(h), ws(ws), animated(false), animFactor(0) { }

  Texture(TextureID ident, std::string name, u16 rows, u16 cols, u16 w, u16 h, bool animated, u16 animFactor) :
    img(nullptr), ident(ident), name(name), rows(rows), cols(cols), w(w), h(h), animated(animated), animFactor(animFactor) { }

  Texture(TextureID ident, std::string name, u16 rows, std::initializer_list<u16> cols, u16 w, u16 h, bool animated, u16 animFactor) :
    img(nullptr), ident(ident), name(name), rows(rows), animatedSprites(cols), w(w), h(h), animated(animated), animFactor(animFactor), cols(-1) { }

  Texture(TextureID ident, std::string name, u16 rows, u16 cols, std::initializer_list<u16> ws, std::initializer_list<u16> hs, bool animated, u16 animFactor) :
    img(nullptr), ident(ident), name(name), rows(rows), cols(cols), w(-1), h(-1), ws(ws), hs(hs), animated(animated), animFactor(animFactor) { }

  u16 tw() const override { return static_cast<u16>(img->w); }
  u16 th() const override { return static_cast<u16>(img->h); }

  u16 sw(u16 r = 0, u16 c = 0) const override;
  u16 sh(u16 r = 0, u16 c = 0) const override;

  u32 at(u16 x, u16 y, u16 r, u16 c) const override;

  u16 span(u16 i) const { return ws.empty() ? w : ws[i]; }


  static inline u16 upTo(const std::vector<u16>& ws, u16 i) { u16 r = 0; for (u16 j = 0; j < i; ++j) r += ws[j]; return r; }


  static const Texture* get(TextureID ident);
  static void load();
  static void unload();
};


#endif
