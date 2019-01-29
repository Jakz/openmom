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

struct SDL_Surface;

enum class TextureID : u16
{  
  MAIN_CURSORS,
    
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

  ADDITIONAL_SKILL_ICONS,
        
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

  index_t tw() const override { return static_cast<u16>(img->w); }
  index_t th() const override { return static_cast<u16>(img->h); }

  index_t sw(index_t r = 0, index_t c = 0) const override;
  index_t sh(index_t r = 0, index_t c = 0) const override;

  u32 at(index_t x, index_t y, index_t r, index_t c) const override;


  static inline u16 upTo(const std::vector<u16>& ws, u16 i) { u16 r = 0; for (u16 j = 0; j < i; ++j) r += ws[j]; return r; }


  static const Texture* get(TextureID ident);
  static void load();
  static void unload();
};


#endif
