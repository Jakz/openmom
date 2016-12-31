//
//  Texture.h
//  OpenMoM
//
//  Created by Jack on 7/18/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "Common.h"
#include "ColorMap.h"

#include <string>
#include <vector>
#include <initializer_list>

class SDL_Surface;

enum class TextureID : u16
{
  ARCANUS_TILES,
  ARCANUS_SHORE,
  MYRRAN_TILES,
  MYRRAN_SHORE,
  
  ARCANUS_DESERT,
  MYRRAN_DESERT,
  
  ARCANUS_TUNDRA,
  MYRRAN_TUNDRA,
  
  ARCANUS_MOUNTAINS,
  MYRRAN_MOUNTAINS,
  
  ARCANUS_RIVERS,
  MYRRAN_RIVERS,
  
  ARCANUS_ANIMATED,
  MYRRAN_ANIMATED,
  
  ARCANUS_NODES,
  MYRRAN_NODES,
  
  
  TILE_NODE_AURAS,
  TILE_FOG,

  TILE_RESOURCES,  
  
  MAIN_CURSORS,
  MAIN_MIRROR,
  
  MAGIC_GLOBAL_SPELLS,
  MAGIC_SPELL_EFFECTS,
  MAGIC_SCHOOL_ICONS,
  
  MAGIC_COMBAT_SPELL_EFFECT0,
  MAGIC_COMBAT_SPELL_EFFECT1,
  MAGIC_COMBAT_SPELL_EFFECT2,
  MAGIC_COMBAT_SPELL_EFFECT3,
  MAGIC_COMBAT_SPELL_EFFECT4,
  MAGIC_COMBAT_SPELL_EFFECT5,
  MAGIC_COMBAT_SPELL_EFFECT6,
  MAGIC_COMBAT_SPELL_EFFECT7,
  MAGIC_COMBAT_SPELL_EFFECT8,
  MAGIC_COMBAT_SPELL_EFFECT9,
  MAGIC_COMBAT_SPELL_EFFECT10,
  MAGIC_COMBAT_SPELL_EFFECT11,
  MAGIC_COMBAT_SPELL_EFFECT12,
  MAGIC_COMBAT_SPELL_EFFECT13,
  MAGIC_COMBAT_SPELL_EFFECT14,
  MAGIC_COMBAT_SPELL_EFFECT15,
  MAGIC_COMBAT_SPELL_EFFECT16,
  MAGIC_COMBAT_SPELL_EFFECT17,
  MAGIC_COMBAT_SPELL_EFFECT18,
  MAGIC_COMBAT_SPELL_EFFECT19,
  MAGIC_COMBAT_SPELL_EFFECT20,
  MAGIC_COMBAT_SPELL_EFFECT21,
  MAGIC_COMBAT_SPELL_EFFECT22,
  MAGIC_COMBAT_SPELL_EFFECT23,
  MAGIC_COMBAT_SPELL_EFFECT24,
  MAGIC_COMBAT_SPELL_EFFECT25,
  MAGIC_COMBAT_SPELL_EFFECT26,
  MAGIC_COMBAT_SPELL_EFFECT27,
  
  MESSAGE_LEFT,
  MESSAGE_RIGHT,
  //113 - 29
  MESSAGE_CONFIRM_DIALOG,
  
  SPELL_BOOK_SCHOOL_ICONS,
  
  RIGHT_BACKDROP,
    
  CITY_BACKGROUND,
  CITY_WATER,
  CITY_LANDSCAPE,
  
  CITY_RACES_LABOUR,
  CITY_PRODUCTION,
  CITY_BUILDINGS_1,
  CITY_BUILDINGS_2,
  CITY_FORTRESS,
  CITY_ROADS,
  CITY_HOUSES,
  CITY_WALLS,
  
  OUTPOST_VIEW_BACKDROP,
  OUTPOST_HOUSES,
  
  MAGIC_GEMS,
  MAGIC_PORTRAITS,
  MAGIC_MANA_WANDS,
  
  ALCHEMY_BACKDROP,
    
  RESEARCH_MISC,
  RESEARCH_BEAM,
  RESEARCH_TOME, // TODO: taken from screenshot, palette was wrong
  RESEARCH_MASCOTS, // TODO: taken fron screenshot, palette was wrong
  RESEARCH_LAB_BACKDROP,
  RESEARCH_WIZARDS,
  
    
  // units
  
  UNIT_DETAIL_SPECIAL_THINGS, // contiene anche mirino minimap
  UNIT_DETAIL_TARGET,
  UNIT_DETAIL_PROPS,
  UNIT_DETAIL_PROPS_EMPTY,
  UNIT_SKILL_ICONS,
    
  UNITS_STATIC_STANDARD,
  
  UNIT_HERO_ITEM_BACKGROUND,
  
    
  /* altezza max help: 200 + 23 pezzo di chiusura da sovrapporre */
  HELP_BACKDROP,
  
  COMBAT_BACKDROP,
  COMBAT_BUTTONS,
  
  COMBAT_ENCHANTMENTS,
  
  COMBAT_GRASS_ARCANUS,
  COMBAT_MISC_TILES,
  
  WIZARD_SPELLBOOKS,
  
  DARKNER,
    
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
  
    Color at(u16 x, u16 y, u16 r, u16 c) const override;
  
    u16 span(u16 i) const { return ws.empty() ? w : ws[i]; }
  
  
    static inline u16 upTo(const std::vector<u16>& ws, u16 i) { u16 r = 0; for (u16 j = 0; j < i; ++j) r += ws[j]; return r; }

  
    static const Texture* get(TextureID ident);
    static void load();
    static void unload();
};


#endif
