//
//  Texture.cpp
//  OpenMoM
//
//  Created by Jack on 7/18/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "Texture.h"

#include "Gfx.h"
#include "LBX.h"

#include "SDL.h"

const Texture Texture::textures[] =
{
  Texture(TextureID::ARCANUS_TILES,"tile/arcanus/tile_map_1.png",10,10,20,18),
  Texture(TextureID::ARCANUS_SHORE,"tile/arcanus/tile_map_shore.png",26,10,20,18),
  Texture(TextureID::MYRRAN_TILES,"tile/myrran/tile_map_1.png",10,10,20,18),
  Texture(TextureID::MYRRAN_SHORE,"tile/myrran/tile_map_shore.png",26,10,20,18),
  
  Texture(TextureID::ARCANUS_DESERT,"tile/arcanus/tiles-desert.png",26,10,20,18),
  Texture(TextureID::MYRRAN_DESERT,"tile/myrran/tiles-desert.png",26,10,20,18),
  
  Texture(TextureID::ARCANUS_TUNDRA,"tile/arcanus/tiles-tundra.png",26,10,20,18),
  Texture(TextureID::MYRRAN_TUNDRA,"tile/myrran/tiles-tundra.png",26,10,20,18),
  
  Texture(TextureID::ARCANUS_MOUNTAINS,"tile/arcanus/mountains.png",2,17,20,18),
  Texture(TextureID::MYRRAN_MOUNTAINS,"tile/myrran/mountains.png",2,17,20,18),
  
  Texture(TextureID::ARCANUS_RIVERS,"tile/arcanus/rivers.png",28,8,20,18),
  Texture(TextureID::MYRRAN_RIVERS,"tile/myrran/rivers.png",28,8,20,18),
  
  Texture(TextureID::ARCANUS_ANIMATED,"tile/arcanus/animated.png",39,4,20,18,true,2),
  Texture(TextureID::MYRRAN_ANIMATED,"tile/myrran/animated.png",34,4,20,18,true,2),
  
  Texture(TextureID::ARCANUS_NODES,"tile/arcanus/nodes.png",3,4,20,18,true,5),
  Texture(TextureID::MYRRAN_NODES,"tile/myrran/nodes.png",3,4,20,18,true,5),
  
  
  Texture(TextureID::TILE_NODE_AURAS,"tile/node-auras.png",5,6,20,18,true,2),
  Texture(TextureID::TILE_FOG,"tile/fog.png",1,10,20,18),
  Texture(TextureID::TILE_RESOURCES,"tile/resources.png",1,11,20,18),
  Texture(TextureID::TILE_PLACES,"tile/places.png",1,10,20,18),
  
  
  Texture(TextureID::MAIN_CURSORS,"main/cursors.png",1,5,16,16),
  Texture(TextureID::MAIN_LOW_BUTTONS,"main/next-turn.png",2,2,80,27),
  Texture(TextureID::MAIN_MIRROR_BACKDROP,"static/mirror-backdrop.png",1,1,180,196),
  Texture(TextureID::MAIN_MIRROR,"static/mirror.png",1,17,128,129),
  Texture(TextureID::MAIN_RIGHT_BACKDROPS,"main/right-backdrops.png",1,2,80,98),
  
  Texture(TextureID::MAGIC_GLOBAL_SPELLS,"magic/global-spells.png",2,13,110,113),
  Texture(TextureID::MAGIC_SPELL_EFFECTS,"magic/spell-effects.png",4,{12,6,18,12,12,12,28,24,20,17,37,21,17},60,54,true,1),
  Texture(TextureID::MAGIC_SCHOOL_ICONS,"static/magic-school-icons.png",1,5,100,90),
  
  Texture(TextureID::MAGIC_COMBAT_SPELL_EFFECT0,"magic/effects/combat-effect-0.png",1,4,28,30,true,1),
  Texture(TextureID::MAGIC_COMBAT_SPELL_EFFECT1,"magic/effects/combat-effect-1.png",1,8,28,34,true,1),
  Texture(TextureID::MAGIC_COMBAT_SPELL_EFFECT2,"magic/effects/combat-effect-2.png",1,13,28,34,true,1),
  Texture(TextureID::MAGIC_COMBAT_SPELL_EFFECT3,"magic/effects/combat-effect-3.png",1,8,28,34,true,1),
  Texture(TextureID::MAGIC_COMBAT_SPELL_EFFECT4,"magic/effects/combat-effect-4.png",1,10,28,34,true,1),
  Texture(TextureID::MAGIC_COMBAT_SPELL_EFFECT5,"magic/effects/combat-effect-5.png",1,4,28,30,true,1),
  Texture(TextureID::MAGIC_COMBAT_SPELL_EFFECT6,"magic/effects/combat-effect-6.png",1,15,28,34,true,1),
  Texture(TextureID::MAGIC_COMBAT_SPELL_EFFECT7,"magic/effects/combat-effect-7.png",1,14,28,34,true,1),
  Texture(TextureID::MAGIC_COMBAT_SPELL_EFFECT8,"magic/effects/combat-effect-8.png",1,11,28,34,true,1),
  Texture(TextureID::MAGIC_COMBAT_SPELL_EFFECT9,"magic/effects/combat-effect-9.png",1,14,28,34,true,1),
  Texture(TextureID::MAGIC_COMBAT_SPELL_EFFECT10,"magic/effects/combat-effect-10.png",1,10,28,34,true,1),
  Texture(TextureID::MAGIC_COMBAT_SPELL_EFFECT11,"magic/effects/combat-effect-11.png",1,4,28,30,true,1),
  Texture(TextureID::MAGIC_COMBAT_SPELL_EFFECT12,"magic/effects/combat-effect-12.png",1,8,28,34,true,1),
  Texture(TextureID::MAGIC_COMBAT_SPELL_EFFECT13,"magic/effects/combat-effect-13.png",1,10,28,34,true,1),
  Texture(TextureID::MAGIC_COMBAT_SPELL_EFFECT14,"magic/effects/combat-effect-14.png",1,35,60,54,true,1),
  Texture(TextureID::MAGIC_COMBAT_SPELL_EFFECT15,"magic/effects/combat-effect-15.png",1,10,28,34,true,1),
  Texture(TextureID::MAGIC_COMBAT_SPELL_EFFECT16,"magic/effects/combat-effect-16.png",1,10,28,34,true,1),
  Texture(TextureID::MAGIC_COMBAT_SPELL_EFFECT17,"magic/effects/combat-effect-17.png",1,8,28,34,true,1),
  Texture(TextureID::MAGIC_COMBAT_SPELL_EFFECT18,"magic/effects/combat-effect-18.png",1,10,28,34,true,1),
  Texture(TextureID::MAGIC_COMBAT_SPELL_EFFECT19,"magic/effects/combat-effect-19.png",1,14,28,34,true,1),
  Texture(TextureID::MAGIC_COMBAT_SPELL_EFFECT20,"magic/effects/combat-effect-20.png",1,8,28,34,true,1),
  Texture(TextureID::MAGIC_COMBAT_SPELL_EFFECT21,"magic/effects/combat-effect-21.png",1,10,28,34,true,1),
  Texture(TextureID::MAGIC_COMBAT_SPELL_EFFECT22,"magic/effects/combat-effect-22.png",1,16,28,34,true,1),
  Texture(TextureID::MAGIC_COMBAT_SPELL_EFFECT23,"magic/effects/combat-effect-23.png",1,16,28,30,true,1),
  Texture(TextureID::MAGIC_COMBAT_SPELL_EFFECT24,"magic/effects/combat-effect-24.png",1,4,184,200,true,1),
  Texture(TextureID::MAGIC_COMBAT_SPELL_EFFECT25,"magic/effects/combat-effect-25.png",1,8,28,34,true,1),
  Texture(TextureID::MAGIC_COMBAT_SPELL_EFFECT26,"magic/effects/combat-effect-26.png",1,8,28,34,true,1),
  Texture(TextureID::MAGIC_COMBAT_SPELL_EFFECT27,"magic/effects/combat-effect-27.png",1,16,28,34,true,1),
  
  Texture(TextureID::MESSAGE_LEFT,"static/message-left.png",1,1,249,59),
  Texture(TextureID::MESSAGE_RIGHT,"static/message-right.png",1,1,55,59),
  Texture(TextureID::MESSAGE_BUILDING_BACKDROP,"static/message-building-backdrop.png",1,1,41,43),
  //113 - 29
  Texture(TextureID::MESSAGE_CONFIRM_DIALOG,"static/confirm-dialog.png",1,1,186,142),
  Texture(TextureID::MESSAGE_CONFIRM_BUTTONS,"static/confirm-buttons.png",2,2,66,17),
  
  Texture(TextureID::SPELL_BOOK_SCHOOL_ICONS,"spellbook/school-symbols.png",1,5,5,5),
  
  Texture(TextureID::RIGHT_BACKDROP,"static/right_backdrop.gif",2,1,80,124),
    
  Texture(TextureID::CITY_TRADE_HOUSING_BACKDROP,"city/tradegoods-housing-backdrop.png",1,1,53,33),
  Texture(TextureID::CITY_VIEW_BUTTONS,"city/city-buttons-1.png",3,3,{30,36,30},10),
  Texture(TextureID::CITY_BACKGROUND,"city/city-background.png",5,1, 205, 96),
  Texture(TextureID::CITY_WATER,"city/city-water.png",4,6,16,96, true,5),
  Texture(TextureID::CITY_LANDSCAPE,"city/scape/landscape.png",12,1,205,19),
  
  Texture(TextureID::CITY_RACES_LABOUR,"city/city-races-labour.png",3,14,10,16),
  Texture(TextureID::CITY_PRODUCTION,"city/production.png",5,4,{7,7,14,14},7),
  Texture(TextureID::CITY_BUILDINGS_1,"city/scape/buildings-1.png",6,5,40,33),
  Texture(TextureID::CITY_BUILDINGS_2,"city/scape/buildings-2.png",13,9,40,33,true,2),
  Texture(TextureID::CITY_FORTRESS,"city/scape/fortress.png",1,9,40,48,true,2),
  Texture(TextureID::CITY_ROADS,"city/scape/roads.png",4,1,210,75),
  Texture(TextureID::CITY_HOUSES,"city/scape/houses.png",3,5,12,20),
  Texture(TextureID::CITY_WALLS,"city/scape/walls.png",4, {1,1,4,4}, 205, 11, true, 2),
  
  Texture(TextureID::OUTPOST_VIEW_BACKDROP,"static/backdrop-outpost.png",1,1,262,103),
  Texture(TextureID::OUTPOST_HOUSES,"city/outpost-houses.png",2,3,12,13),
  
  Texture(TextureID::MAGIC_BUTTONS,"magic/view/buttons.png",2,2, {58,26},15),
  Texture(TextureID::MAGIC_GEMS,"magic/view/gems.png",1,7,42,45),
  Texture(TextureID::MAGIC_PORTRAITS,"magic/view/portraits.png",1,14,33,35),
  Texture(TextureID::MAGIC_MANA_WANDS,"magic/view/mana-wands.png",1,6,16,72),
  
  Texture(TextureID::ALCHEMY_BACKDROP,"magic/view/backdrop-alchemy.png",1,1,166,84),
  Texture(TextureID::ALCHEMY_BUTTONS,"magic/view/buttons-alchemy.png",2,4, {49,49,26,26},16),
  
  Texture(TextureID::ARMIES_BUTTONS,"armies/buttons.png",2,4, {41,41,59,59},15),
  Texture(TextureID::ARMIES_ARROWS,"armies/arrows.png",3,2,9,10),
  Texture(TextureID::ARMIES_BACKDROP_ITEMS,"armies/backdrop-items.png",1,1,285,197),
  
  Texture(TextureID::RESEARCH_BACKDROP,"static/backdrop-research.png",1,1,320,200),
  Texture(TextureID::RESEARCH_MISC,"static/research-misc.png",1,2,16,16),
  Texture(TextureID::RESEARCH_BEAM,"magic/view/research-beam.png",1,10,77,147,true,1),
  Texture(TextureID::RESEARCH_TOME,"magic/view/research-tome.png",42,54), // TODO: taken from screenshot, palette was wrong
  Texture(TextureID::RESEARCH_MASCOTS,"magic/view/research-mascots.png",1,5,32,32), // TODO: taken fron screenshot, palette was wrong
  Texture(TextureID::RESEARCH_LAB_BACKDROP,"static/backdrop-research-lab.png",1,1,320,200),
  Texture(TextureID::RESEARCH_WIZARDS,"magic/research-wizards.png",1,14,84,107),
  
  
  Texture(TextureID::SUMMON_BACKDROP,"static/backdrop-summon.png",179,136),
  Texture(TextureID::SUMMON_WIZARDS,"magic/summon-wizards.png",1,14,65,91),
  Texture(TextureID::SUMMON_ANIMATION,"magic/summon-animation.png",2,15, {85,70}, {47,30}, true, 1),
  
  // units ,not combat related sprites)
  
  Texture(TextureID::UNIT_DETAIL_SKILL_ARROWS,"units/static/skill-pane-arrows.png",2,2,9,10),
  Texture(TextureID::UNIT_DETAIL_SPECIAL_THINGS,"units/special-things.png",1,1,73,33), // contiene anche mirino minimap
  Texture(TextureID::UNIT_DETAIL_TARGET,"units/static/target-backdrop.png",1,1,238,200),
  Texture(TextureID::UNIT_DETAIL_PROPS,"units/props.png",2,12,9,8),
  Texture(TextureID::UNIT_DETAIL_PROPS_EMPTY,"units/props-empty.png",1,12,9,8),
  Texture(TextureID::UNIT_SKILL_ICONS,"units/skill-icons.png",16,16,16,16),
  
  
  Texture(TextureID::UNITS_STATIC_STANDARD,"units/standard-units.png",14,11,18,16),
  
  Texture(TextureID::UNIT_HERO_ITEM_BACKGROUND,"units/items-background.png",1,6,18,16),
  
  
  Texture(TextureID::ITEM_CRAFT_BACKDROP,"static/backdrop-itemcraft.png",320,200),
  Texture(TextureID::ITEM_CRAFT_BUTTONS,"magic/view/item-craft-buttons.png",4,10,31,13),
  Texture(TextureID::ITEM_CRAFT_ARROWS,"magic/view/item-craft-arrows.png",2,2,9,7),
  
  /* altezza max help: 200 + 23 pezzo di chiusura da sovrapporre */
  Texture(TextureID::HELP_BACKDROP,"static/help-backdrop.png",1,1,217,223),
  
  Texture(TextureID::COMBAT_BACKDROP,"static/backdrop-combat.png",1,1,402,36),
  Texture(TextureID::COMBAT_BUTTONS,"combat/combat-buttons.png",3,6,26,10),
  
  Texture(TextureID::COMBAT_ENCHANTMENTS,"combat/enchantments.png",3,10,17,16),
  
  Texture(TextureID::COMBAT_GRASS_ARCANUS,"combat/arcanus/grass.png",6,8,30,16),
  Texture(TextureID::COMBAT_MISC_TILES,"combat/misc-tiles.png",3,8,30,16,true,1),
  
  Texture(TextureID::WIZARD_PORTRAITS,"wizards/portraits.png",1,14,109,104),
  Texture(TextureID::WIZARD_PORTRAITS_GEMS,"wizards/portraits-gems.png",5,14,41,43),
  Texture(TextureID::WIZARD_SPELLBOOKS,"wizards/spellbooks.png",3,6,9,21),
  
  Texture(TextureID::DARKNER,"static/darkner.png",1,1,320,200),
  
  Texture(TextureID::NEW_GAME_BACKDROP,"newgame/backdrop.png",1,1,320,200),
  Texture(TextureID::NEW_GAME_PARTIALS,"newgame/backdrops-right.png",1,2,153,200),
  Texture(TextureID::NEW_GAME_BUTTONS1,"newgame/buttons1.png",1,6,{63,63,64,64,64,64},15)
};

Color Texture::at(u16 x, u16 y, u16 r, u16 c) const
{
  static_assert(sizeof(textures)/sizeof(textures[0]) == static_cast<size_t>(TextureID::TEXTURES_COUNT), "");
  
  u16 sx, sy;
  u16 tw = img->w;
  
  
  if (w != -1)
    sx = c*w;
  else
    sx = h != -1 ? sx = upTo(ws, c) : c*w;

  if (h != -1)
    sy = r*h;
  else
    sy = upTo(hs, r);
  
  return static_cast<u32*>(img->pixels)[sx+x + (sy+y)*img->w];
}

u16 Texture::sw(u16 r, u16 c) const
{
  if (w != -1)
    return w;
  else
  {
    if (h != -1)
      return ws[c];
    else
      return ws[r];
  }
}

u16 Texture::sh(u16 r, u16 c) const
{
  if (h != -1)
    return h;
  else return hs[c];
}






const Texture* Texture::get(TextureID ident) {
  const Texture* texture = &textures[static_cast<size_t>(ident)];
  
  if (!texture->img)
    texture->img = IMG_Load(("data/gfx/"+texture->name).c_str());
  
  return texture;
}




void Texture::load()
{
  for (const Texture &texture : textures)
    get(texture.ident);
}

void Texture::unload()
{
  for (const Texture &texture : textures)
    if (texture.img)
      SDL_FreeSurface(texture.img);
}
