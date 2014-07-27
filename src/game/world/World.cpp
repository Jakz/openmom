#include "World.h"

#include "GfxData.h"

void World::calcSubTile(u16 x, u16 y, Plane p)
{
  Tile* t = get(x,y,p);
  
  s16 subtype = 0;
  
  if (t->type == TILE_GRASS || t->type == TILE_FOREST)
    t->tileGfxType = TILE_GFX_PLAIN;
  else if (t->type == TILE_WATER)
  {
    for (int i = 0; i < Util::DIRS_LENGTH; ++i)
    {
      Tile* tile = get(x,y,p,Util::DIRS[i]);
      if (tile && tile->type != TILE_WATER && tile->type != TILE_SHORE)
        subtype |= (1 << i);
    }
    
    if ((subtype & ( 1| 4 | 16 | 64)) == (1 | 4 | 16 | 64))
      subtype = 255;
    
    if (subtype == 255 && p == ARCANUS)
    {
      t->subtype = subtype;
      t->tileGfxType = TILE_GFX_ANIMATED;
    }
    else if (subtype == 128+64+1 && subtype == 128+64) // TODO: maybe ||?
    {
      t->subtype = subtype;
      t->tileGfxType = TILE_GFX_ANIMATED;
    }
    else if (subtype != 0)
    {
      t->subtype = subtype;
      t->tileGfxType = TILE_GFX_BORDER;
    }
    else if (Util::chance(0.08f))
      t->tileGfxType = TILE_GFX_ANIMATED;
    else
      t->tileGfxType = TILE_GFX_PLAIN;
    
    if (subtype != 0)
      t->type = TILE_SHORE;
    
    // check for rivers around
    subtype = 0;
    for (int i = 0; i < Util::DIRS_LENGTH; ++i)
    {
      Tile* tile = get(x,y,p,Util::DIRS[i]);
      if (tile && tile->type == TILE_RIVER)
        subtype |= (1 << i);
    }
    
    if (subtype != 0)
    {
      // TODO: manage rivers
    }
  }
  
  for (const TileType tt : {TILE_DESERT, TILE_TUNDRA})
  {
    if (t->type == tt)
    {
      subtype = 0;
      bool change = false;
      for (int i = 0; i < Util::DIRS_LENGTH; ++i)
      {
        Tile* tile = get(x,y,p,Util::DIRS[i]);
        if (tile && tile->type != tt)
          subtype |= (1 << i);
      }
      
      if ((subtype & ( 1| 4 | 16 | 64)) == (1 | 4 | 16 | 64))
        subtype = 255;
      
      // TODO: there is a commented piece in my original code, check if it was useful
      
      if (change)
      {
        t->tileGfxType = TILE_GFX_BORDER;
        t->subtype = 255;
      }
      if (subtype != 0)
      {
        t->tileGfxType = TILE_GFX_BORDER;
        t->subtype = subtype;
      }
      else
      {
        t->tileGfxType = TILE_GFX_PLAIN;
        t->subtype = Util::randomIntUpTo(GfxData::tileGfxSpec(t->type).count);
      }
    }
  }
  
  if (t->type == TILE_MOUNTAIN)
  {
    subtype = 0;
    
    for (int i = 0; i < Util::DIRS_LENGTH; ++i)
    {
      Tile* tile = get(x,y,p,Util::DIRS[i]);
      if (tile && tile->type == TILE_MOUNTAIN)
        subtype |= (1<<i);
    }
    
    t->subtype = subtype;
  }
  else if (t->type == TILE_HILL)
  {
    subtype = 0;
    
    for (int i = 0; i < Util::DIRS_LENGTH; ++i)
    {
      Tile* tile = get(x,y,p,Util::DIRS[i]);
      if (tile && tile->type == TILE_HILL)
        subtype |= (1<<i);
    }
    
    t->subtype = subtype;
  }
  else if (t->type == TILE_RIVER)
  {
    /*subtype = 0;
    for (int i = 0; i < Util::DIRS_LENGTH; ++i)
    {
      Tile* tile = get(x,y,p,Util::DIRS[i]);
      if (tile && tile->type == TILE_RIVER)
        subtype |= (1<<i);
    }
    
    --subtype;
    
    static constexpr u8 riverTilesCount[] = { 1, 1, 3, 1, 3, 3, 4, 1, 3, 3, 4, 3, 4, 4, 5 };
    
    int baseIndex = 0;
    for (int i = 0; i < subtype; ++i) baseIndex += riverTilesCount[i];
    
    t->subtype = baseIndex + Util::randomIntUpTo(riverTilesCount[subtype]);*/
    // TDO: fix
  }
  else if (t->tileGfxType == TILE_GFX_PLAIN && (t->type == TILE_GRASS || t->type == TILE_SWAMP || t->type == TILE_FOREST || t->type == TILE_DESERT || t->type == TILE_TUNDRA))
  {
    // in the Myrran sprites I haven't been able to find the sixth sprite for grass
    if (t->type == TILE_GRASS && p == MYRRAN)
      t->subtype = Util::randomIntUpTo(GfxData::tileGfxSpec(t->type).count-1);
    else
      t->subtype = Util::randomIntUpTo(GfxData::tileGfxSpec(t->type).count);
  }
  
}