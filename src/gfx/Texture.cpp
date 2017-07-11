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


#include <bitset>

const size_t TILE_COUNT = 1524;
const size_t TILE_PER_PLANE = 0x2FA;
constexpr size_t TILE_WIDTH = 20;
constexpr size_t TILE_HEIGHT = 18;

template<size_t SIZE = 256> using tile_mapping = std::array<const lbx::LBXSpriteData*, SIZE>;

tile_mapping<256> createJoiningTileTextureAtlas8dirs(size_t gfxOffset, size_t arrayIndex, Plane plane, std::bitset<TILE_COUNT>& used, const char* fileName)
{
  using namespace lbx;
  constexpr size_t ATLAS_WIDTH = 8;
  constexpr size_t ATLAS_HEIGHT = 32;
  
  SDL_Surface* atlas = SDL_CreateRGBSurface(0, ATLAS_WIDTH*TILE_WIDTH, ATLAS_HEIGHT*TILE_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
  
  const LBXArrayData* mapping = Repository::arrayFor(LBXID::TERRTYPE, arrayIndex);
  const size_t delta = plane == Plane::ARCANUS ? 0 : TILE_PER_PLANE;

  tile_mapping<256> result;
  
  for (size_t i = 0; i < 256; ++i)
  {
    u16 gfxIndex = mapping->get<u16>(i);
    u16 adjustedIndex = gfxIndex + delta + gfxOffset;
    
    const LBXSpriteData* sprite = Repository::spriteFor(LSI(TERRAIN, adjustedIndex));
    
    result[i] = sprite;
    used[adjustedIndex] = true;
    
    const size_t bx = 20 * (i % ATLAS_WIDTH);
    const size_t by = 18 * (i / ATLAS_WIDTH);
    
    for (size_t x = 0; x < TILE_WIDTH; ++x)
    {
      for (size_t y = 0; y < TILE_HEIGHT; ++y)
      {
        Color* pixel = reinterpret_cast<Color*>(atlas->pixels) + bx + x + (by + y)*atlas->w;
        *pixel = sprite->getPalette()->get(sprite->at(x, y, 0, 0));
      }
    }
  }
  
  IMG_SavePNG(atlas, fileName);
  SDL_FreeSurface(atlas);

  return result;
}

tile_mapping<16> createJoiningTileTextureAtlas4dirs(size_t gfxOffset, size_t arrayIndex, Plane plane, std::bitset<TILE_COUNT>& used, const char* fileName)
{
  using namespace lbx;
  constexpr size_t ATLAS_WIDTH = 16;
  constexpr size_t ATLAS_HEIGHT = 1;
  
  SDL_Surface* atlas = SDL_CreateRGBSurface(0, ATLAS_WIDTH*TILE_WIDTH, ATLAS_HEIGHT*TILE_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
  
  const LBXArrayData* mapping = Repository::arrayFor(LBXID::TERRTYPE, arrayIndex);
  const size_t delta = plane == Plane::ARCANUS ? 0 : TILE_PER_PLANE;
  
  tile_mapping<16> result;
  
  static const u16 indices[] = { 0, 1, 4, 1+4, 16, 1+16, 4+16, 1+4+16, 64, 64+1, 64+4, 64+1+4, 64+16, 64+1+16, 64+4+16, 64+1+4+16 };
  
  for (size_t i = 0; i < 16; ++i)
  {
    u16 gfxIndex = mapping->get<u16>(indices[i]);
    u16 adjustedIndex = gfxIndex + delta + gfxOffset;
    
    const LBXSpriteData* sprite = Repository::spriteFor(LSI(TERRAIN, adjustedIndex));
    
    result[i] = sprite;
    used[adjustedIndex] = true;
    
    const size_t bx = 20 * (i % ATLAS_WIDTH);
    const size_t by = 18 * (i / ATLAS_WIDTH);
    
    for (size_t x = 0; x < TILE_WIDTH; ++x)
    {
      for (size_t y = 0; y < TILE_HEIGHT; ++y)
      {
        Color* pixel = reinterpret_cast<Color*>(atlas->pixels) + bx + x + (by + y)*atlas->w;
        *pixel = sprite->getPalette()->get(sprite->at(x, y, 0, 0));
      }
    }
  }
  
  IMG_SavePNG(atlas, fileName);
  SDL_FreeSurface(atlas);
  
  return result;
}

void Texture::createMapTextureAtlas()
{
  using namespace lbx;
  

  
  constexpr size_t ATLAS_WIDTH = 10;
  constexpr size_t ATLAS_HEIGHT = 26;
  
  
  SDL_Surface* atlas = SDL_CreateRGBSurface(0, ATLAS_WIDTH*TILE_WIDTH, ATLAS_HEIGHT*TILE_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
  Repository::loadLBXTerrain();
  Repository::loadLBXTerrainMap();
  Repository::loadMultipleLBXSpriteData(LBXID::TERRAIN);
  
  std::bitset<TILE_COUNT> used;
  
  createJoiningTileTextureAtlas8dirs(0, 0, Plane::ARCANUS, used, "arcanus-shore.png");
  createJoiningTileTextureAtlas8dirs(0, 0, Plane::MYRRAN, used, "myrran-shore.png");
  createJoiningTileTextureAtlas8dirs(-2 + 0x124, 0, Plane::ARCANUS, used, "arcanus-desert.png");
  createJoiningTileTextureAtlas8dirs(-2 + 0x124, 0, Plane::MYRRAN, used, "myrran-desert.png");
  createJoiningTileTextureAtlas8dirs(-2 + 0x25A, 0, Plane::ARCANUS, used, "arcanus-tundra.png");
  createJoiningTileTextureAtlas8dirs(-2 + 0x25A, 0, Plane::MYRRAN, used, "myrran-tundra.png");
  
  createJoiningTileTextureAtlas4dirs(0, 1, Plane::ARCANUS, used, "arcanus-mountains.png");
  createJoiningTileTextureAtlas4dirs(-0x103 + 0x113, 1, Plane::ARCANUS, used, "arcanus-hills.png");
  createJoiningTileTextureAtlas4dirs(0, 1, Plane::MYRRAN, used, "myrran-mountains.png");
  createJoiningTileTextureAtlas4dirs(-0x103 + 0x113, 1, Plane::MYRRAN, used, "myrran-hills.png");
  
  atlas = SDL_CreateRGBSurface(0, 40*TILE_WIDTH, 40*TILE_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
  
  {
    for (size_t i = 0; i < TILE_COUNT; ++i)
    {
      const LBXSpriteData* sprite = Repository::spriteFor(LSI(TERRAIN, i));
      
      const size_t bx = 20 * (i % 40);
      const size_t by = 18 * (i / 40);
      
      for (size_t x = 0; x < TILE_WIDTH; ++x)
      {
        for (size_t y = 0; y < TILE_HEIGHT; ++y)
        {
          Color* pixel = reinterpret_cast<Color*>(atlas->pixels) + bx + x + (by + y)*atlas->w;
          Color color = sprite->getPalette()->get(sprite->at(x, y, 0, 0));
          
          if (used[i])
            color = color.blend(Color(255,0,0,128));
          
          *pixel = color;
        }
      }
    }
  }
  
  IMG_SavePNG(atlas, "atlas.png");
  SDL_FreeSurface(atlas);
  
  /*for (size_t i = 0, c = 0; i < TILE_COUNT; ++i)
  {
    const LBXSpriteData* sprite = Repository::spriteFor(LSI(TERRAIN, i));

    //for (size_t k = 0; k < 1; ++k)
    int k = 0;
    {
      const size_t bx = 20 * ((c+k) % ATLAS_WIDTH);
      const size_t by = 18 * ((c+k) / ATLAS_WIDTH);
      
      for (size_t x = 0; x < TILE_WIDTH; ++x)
      {
        for (size_t y = 0; y < TILE_HEIGHT; ++y)
        {
          u32* pixel = reinterpret_cast<u32*>(atlas->pixels) + bx + x + (by + y)*atlas->w;
          *pixel = sprite->getPalette()->get(sprite->at(x, y, k, 0));
        }
      }
    }
    
    c += 1;
  }*/
  

}
