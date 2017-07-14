//
//  MiniMap.cpp
//  OpenMoM
//
//  Created by Jack on 7/23/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "MiniMap.h"
#include "World.h"
#include "Gfx.h"
#include "LBX.h"
#include "Viewport.h"
#include "Tile.h"
#include "City.h"
#include "Player.h"
#include "Places.h"
#include "GfxData.h"

MiniMap::MiniMap(const World* world) : world(world), maps{new SurfaceWrapper(world->w, world->h), new SurfaceWrapper(world->w, world->h)}
{
  
}

MiniMap::~MiniMap()
{
  delete maps[0];
  delete maps[1];
}

void MiniMap::discover(const Position& position)
{
  Gfx::lock(maps[position.plane]);
  maps[position.plane]->set(position.x, position.y,  minimapColor(world->get(position)));
  Gfx::unlock(maps[position.plane]);
}

void MiniMap::discover(const Rect& rect, Plane plane)
{
  Gfx::lock(maps[plane]);
  for (s16 x = rect.origin.x; x < rect.origin.x + rect.size.w; ++x)
    for (s16 y = rect.origin.y; y < rect.origin.y + rect.size.h; ++y)
      maps[plane]->set(x, y, minimapColor(world->get(x, y, plane)));
  Gfx::unlock(maps[plane]);
}

Color MiniMap::minimapColor(const Tile* tile)
{
  if (tile->city)
  {
    //TODO: finish
    switch(tile->city->getOwner()->color)
    {
      case GREEN: return {0,190,0};
      case BLUE: return 0xFFFFFFFF;
      case RED: return 0xFFFFFFFF;
      case PURPLE: return {227,125,255};
      case YELLOW: return 0xFFFFFFFF;
      case NEUTRAL: return {190,154,117};
    }
  }
  
  /* first we obtain the sprite we'd draw for that tile */
  SpriteInfo terrainGfx = Viewport::gfxForTerrain(tile);
  
  /* then we get from the index of the sprite the correct minimap color from LBX file */
  u8 colorIndex = lbx::Repository::terrainInfo()[terrainGfx.index()].minimapColor;
  return Gfx::mainPalette->get(colorIndex);
}
