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

enum TextureID : u16
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
  TILE_ROADS,
  TILE_ROADS_ENCHANTED,
  TILE_RESOURCES,
  TILE_PLACES,
  
  
  MAIN_CURSORS,
  MAIN_BACKDROP,
  MAIN_MIRROR_BACKDROP,
  MAIN_MIRROR,
  MAIN_RIGHT_BACKDROPS,
  MAIN_LOW_BUTTONS,
  MAIN_SMALL_BUTTONS,
  MAIN_UNITS_RIGHT_BACKDROP,
  
  
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
  MESSAGE_BUILDING_BACKDROP,
  //113 - 29
  MESSAGE_CONFIRM_DIALOG,
  MESSAGE_CONFIRM_BUTTONS,
  MESSAGE_ERROR_BACKDROP,
  
  SPELL_BOOK_BACKDROP,
  SPELL_BOOK_CORNERS,
  SPELL_BOOK_SCHOOL_ICONS,
  SPELL_BOOK_DUMMY_TEXT,
  
  RIGHT_BACKDROP,
  UPPER_MENU,
  
  CITY_MAP_TILE,
  
  CITY_TRADE_HOUSING_BACKDROP,
  CITY_PRODUCTION_BACKDROP,
  CITY_PRODUCTION_ITEMS,
  CITY_VIEW_BACKDROP,
  CITY_VIEW_BUTTONS,
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
  
  MAGIC_BACKDROP,
  MAGIC_BUTTONS,
  MAGIC_GEMS,
  MAGIC_PORTRAITS,
  MAGIC_MANA_WANDS,
  
  ALCHEMY_BACKDROP,
  ALCHEMY_BUTTONS,
  
  ARMIES_BACKDROP,
  ARMIES_BUTTONS,
  ARMIES_ARROWS,
  ARMIES_BACKDROP_ITEMS,
  
  RESEARCH_BACKDROP,
  RESEARCH_MISC,
  RESEARCH_BEAM,
  RESEARCH_TOME, // TODO: taken from screenshot, palette was wrong
  RESEARCH_MASCOTS, // TODO: taken fron screenshot, palette was wrong
  RESEARCH_LAB_BACKDROP,
  RESEARCH_WIZARDS,
  
  
  SUMMON_BACKDROP,
  SUMMON_WIZARDS,
  SUMMON_ANIMATION,
  
  // units
  
  UNIT_DETAIL_BACKDROP,
  UNIT_DETAIL_BACKDROP_BUTTONS,
  UNIT_DETAIL_SKILL_ARROWS,
  UNIT_DETAIL_BUTTONS,
  UNIT_DETAIL_SPECIAL_THINGS, // contiene anche mirino minimap
  UNIT_DETAIL_TARGET,
  UNIT_DETAIL_HERO_BACKDROP,
  UNIT_DETAIL_MOVEMENT,
  UNIT_DETAIL_PROPS,
  UNIT_DETAIL_PROPS_EMPTY,
  UNIT_SKILL_ICONS,
  
  UNITS_MISC_SYMBOLS,
  UNITS_COLOR_BACKDROP,
  
  UNITS_STATIC_STANDARD,
  UNITS_FANTASTIC,
  UNITS_HEROES,
  
  UNIT_HERO_PORTRAITS,
  UNIT_HERO_ITEM_BACKGROUND,
  
  
  ITEM_CRAFT_BACKDROP,
  ITEM_CRAFT_BUTTONS,
  ITEM_CRAFT_ARROWS,
  ITEMS,
  
  
  FONT_HELP,
  
  FONT_TINY,
  FONT_TINY_COMPACT,
  FONT_TINY_COMPACT_CRYPT,
  FONT_SERIF_CRYPT,
  FONT_YELLOW_SMALL,
  FONT_SERIF,
  FONT_TEAL_MEDIUM,
  FONT_MEDIUM_BOLD,
  FONT_HUGE_SERIF,
  
  UNIT_MAGIC_SPIRIT,
  
  UNIT_GREAT_DRAKE,
  UNIT_HELL_HOUND,
  
  UNIT_BARBARIANS_SPEARMEN,
  UNIT_BARBARIANS_SWORDSMEN,
  UNIT_BARBARIANS_BOWMEN,
  UNIT_BARBARIANS_CAVALRY,
  UNIT_BARBARIANS_SHAMAN,
  UNIT_BARBARIANS_BERSERKERS,
  UNIT_BARBARIANS_SETTLERS,
  
  UNIT_HERO_DWARF,
  
  /* altezza max help: 200 + 23 pezzo di chiusura da sovrapporre */
  HELP_BACKDROP,
  
  COMBAT_BACKDROP,
  COMBAT_BUTTONS,
  
  COMBAT_ENCHANTMENTS,
  
  COMBAT_GRASS_ARCANUS,
  COMBAT_MISC_TILES,
  
  WIZARD_PORTRAITS,
  WIZARD_PORTRAITS_GEMS,
  WIZARD_SPELLBOOKS,
  
  DARKNER,
  
  NEW_GAME_BACKDROP,
  NEW_GAME_PARTIALS,
  NEW_GAME_BUTTONS1,
  
  TEXTURES_COUNT
};

class Texture : public SpriteSheet
{
  private:
    static Texture textures[];


  public:
    const TextureID ident;
    const std::string name;
    mutable SDL_Surface *img;
    const s16 cols, rows;
    const s16 w, h;
    std::vector<u16> ws, hs;
  
    const bool animated;
    std::vector<u16> animatedSprites;
  
    const u16 animFactor;
  
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
  
    u16 tw() const override { return img->w; }
    u16 th() const override { return img->h; }
  
    u16 sw(u16 r, u16 c) const override;
    u16 sh(u16 r, u16 c) const override;
  
    Color at(u16 x, u16 y, u16 r, u16 c) const override;
  
    u16 span(u16 i) const { return ws.empty() ? w : ws[i]; }
  
  
    static inline u16 upTo(const std::vector<u16>& ws, u16 i) { u16 r = 0; for (u16 j = 0; j < i; ++j) r += ws[j]; return r; }

  
    static const Texture* get(TextureID ident);
    static void load();
    static void unload();
};


#endif
