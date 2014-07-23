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
#include "Tile.h"
#include "City.h"
#include "Player.h"
#include "Places.h"
#include "GfxData.h"

MiniMap::MiniMap(const World* world) : world(world), maps{Gfx::createSurface(world->w, world->h), Gfx::createSurface(world->w, world->h)}
{
  
}

void MiniMap::discover(const Position& position)
{
  Gfx::lock(maps[position.plane]);
  u32* pixels = static_cast<u32*>(maps[position.plane]->pixels);
  
  Color color = minimapColor(world->get(position));
  pixels[position.x + position.y*maps[position.plane]->w] = color;
}

Color MiniMap::minimapColor(const Tile *tile)
{
  if (tile->city)
  {
    //TODO: finish
    switch(tile->city->getOwner()->color)
    {
      case GREEN: return RGB(0, 190, 0);
      case BLUE: return 0xFFFFFFFF;
      case RED: return 0xFFFFFFFF;
      case PURPLE: return RGB(227,125,255);
      case YELLOW: return 0xFFFFFFFF;
      case NEUTRAL: return RGB(190,154,117);
    }
  }
  
  if (tile->node && tile->node->school == SORCERY) return RGB(42, 45, 72);
  else if (tile->node && tile->node->school == NATURE) return RGB(53,92,17);
  else return GfxData::tileGfxSpec(tile->type).minimapColor(tile->position.plane, tile->subtype ? 1 : 0);
}