//
//  Texture.cpp
//  OpenMoM
//
//  Created by Jack on 7/18/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "Texture.h"

#include "platform/Platform.h"

#include "Gfx.h"
#include "LBX.h"

#include "SDL.h"

const Texture Texture::textures[] =
{
  Texture(TextureID::MAIN_CURSORS,"main/cursors.png",1,5,16,16),
    
  Texture(TextureID::MESSAGE_LEFT,"static/message-left.png",1,1,249,59),
  Texture(TextureID::MESSAGE_RIGHT,"static/message-right.png",1,1,55,59),
  //113 - 29
          
  Texture(TextureID::CITY_ROADS,"city/scape/roads.png",4,1,210,75),
  

  // units ,not combat related sprites)
  

  Texture(TextureID::UNIT_DETAIL_SPECIAL_THINGS,"units/special-things.png",1,1,73,33), // contiene anche mirino minimap
  Texture(TextureID::UNIT_DETAIL_PROPS,"units/props.png",2,12,9,8),
  Texture(TextureID::UNIT_DETAIL_PROPS_EMPTY,"units/props-empty.png",1,12,9,8),
  Texture(TextureID::UNIT_SKILL_ICONS,"units/skill-icons.png",16,16,16,16),
    
    
  /* altezza max help: 200 + 23 pezzo di chiusura da sovrapporre */
  Texture(TextureID::HELP_BACKDROP,"static/help-backdrop.png",1,1,217,223),
      
  Texture(TextureID::COMBAT_MISC_TILES,"combat/misc-tiles.png",3,8,30,16,true,1),
};

u32 Texture::at(index_t x, index_t y, index_t r, index_t c) const
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

index_t Texture::sw(index_t r, index_t c) const
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

index_t Texture::sh(index_t r, index_t c) const
{
  if (h != -1)
    return h;
  else return hs[c];
}






const Texture* Texture::get(TextureID ident)
{
  const Texture* texture = &textures[static_cast<size_t>(ident)];
  
  if (!texture->img)
  {
    Path base = Platform::instance()->getResourcePath();
    texture->img = IMG_Load((base + "data/gfx" + texture->name).c_str());
    
    const SDL_PixelFormat* destFormat = Gfx::format();
    
    SDL_Surface* original = texture->img;
    texture->img = SDL_ConvertSurface(texture->img, destFormat, 0);
    SDL_FreeSurface(original);
  }
  
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
