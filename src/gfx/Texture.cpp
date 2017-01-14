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
  
  
  Texture(TextureID::TILE_FOG,"tile/fog.png",1,10,20,18),
  
  
  Texture(TextureID::MAIN_CURSORS,"main/cursors.png",1,5,16,16),
  Texture(TextureID::MAIN_MIRROR,"static/mirror.png",1,17,128,129),
  
  Texture(TextureID::MAGIC_SCHOOL_ICONS,"static/magic-school-icons.png",1,5,100,90),
  
  Texture(TextureID::MESSAGE_LEFT,"static/message-left.png",1,1,249,59),
  Texture(TextureID::MESSAGE_RIGHT,"static/message-right.png",1,1,55,59),
  //113 - 29
  Texture(TextureID::MESSAGE_CONFIRM_DIALOG,"static/confirm-dialog.png",1,1,186,142),
  
  
  Texture(TextureID::RIGHT_BACKDROP,"static/right_backdrop.gif",2,1,80,124),
      
  Texture(TextureID::CITY_ROADS,"city/scape/roads.png",4,1,210,75),
  

  // units ,not combat related sprites)
  

  Texture(TextureID::UNIT_DETAIL_SPECIAL_THINGS,"units/special-things.png",1,1,73,33), // contiene anche mirino minimap
  Texture(TextureID::UNIT_DETAIL_PROPS,"units/props.png",2,12,9,8),
  Texture(TextureID::UNIT_DETAIL_PROPS_EMPTY,"units/props-empty.png",1,12,9,8),
  Texture(TextureID::UNIT_SKILL_ICONS,"units/skill-icons.png",16,16,16,16),
  
  Texture(TextureID::UNIT_HERO_ITEM_BACKGROUND,"units/items-background.png",1,6,18,16),
  
    
  /* altezza max help: 200 + 23 pezzo di chiusura da sovrapporre */
  Texture(TextureID::HELP_BACKDROP,"static/help-backdrop.png",1,1,217,223),
  
  Texture(TextureID::COMBAT_BACKDROP,"static/backdrop-combat.png",1,1,402,36),
  Texture(TextureID::COMBAT_BUTTONS,"combat/combat-buttons.png",3,6,26,10),
    
  Texture(TextureID::COMBAT_GRASS_ARCANUS,"combat/arcanus/grass.png",6,8,30,16),
  Texture(TextureID::COMBAT_MISC_TILES,"combat/misc-tiles.png",3,8,30,16,true,1),
    
  Texture(TextureID::DARKNER,"static/darkner.png",1,1,320,200),
};

u32 Texture::at(u16 x, u16 y, u16 r, u16 c) const
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
