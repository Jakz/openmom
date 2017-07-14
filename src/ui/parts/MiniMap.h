//
//  MiniMap.h
//  OpenMoM
//
//  Created by Jack on 7/23/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _MINI_MAP_H_
#define _MINI_MAP_H_

#include "common/Common.h"

class World;
class Tile;
struct SDL_Surface;

class MiniMap
{
private:
  const World* const world;
  SpriteSheet* maps[PLANE_COUNT];
public:
  MiniMap(const World* const world);
  ~MiniMap();
  
  SpriteSheet* get(Plane plane) const { return maps[plane]; }
  
  void discover(const Rect& rect, Plane plane);
  void discover(const Position& position);

  static Color minimapColor(const Tile *tile);
};

#endif
