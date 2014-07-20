#ifndef _TILE_H_
#define _TILE_H_

#include "Common.h"
#include "Places.h"
#include "Spells.h"

#include "Util.h"

#include <list>

class World;
class City;

class Tile
{
private:
  const World *world;
  
public:
  
  TileType type;
  TileGfxType tileGfxType;
  s16 subtype;
  u8 roads;
  
  Position position;
  
  ManaNode *node;
  TileResource resource;
  Place *place;
  
  City* city;
  Army* army;
  
  bool hasRoad, hasEnchantedRoad;
  
  std::list<SpellCast> spells;
  
  s8 animationOffset;
  
  Tile() { }
  
  Tile(const World* world, Position position) : world(world), position(position), subtype(0), tileGfxType(TILE_GFX_NONE), animationOffset(Util::randomIntUpTo(10))
  {
  }
  
  void settleCity(City* city)
  {
    this->city = city;
  }
};

#endif
