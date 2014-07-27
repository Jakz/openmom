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
  World* world;
  
public:
  
  TileType type;
  TileGfxType tileGfxType;
  s16 subtype;
  u8 roads;
  
  Position position;
  
  ManaNode *node;
  Resource resource;
  Place *place;
  
  City* city;
  Army* army;
  
  bool hasRoad, hasEnchantedRoad;
  
  std::list<const SpellCast> spells;
  
  s8 animationOffset;
  
  Tile() { }
  
  Tile(World* const world, Position position) : world(world), position(position), subtype(0), tileGfxType(TILE_GFX_NONE), animationOffset(Util::randomIntUpTo(10)), resource(Resource::NONE), city(nullptr), army(nullptr), place(nullptr)
  {
  }
  
  void settleCity(City* city);
  
  void placeRoad(bool enchanted) { hasRoad = true; hasEnchantedRoad = enchanted; }
  void placeResource(Resource resource) { this->resource = resource; }
  void placeManaNode(ManaNode* node) { this->node = node; }
  void placePlace(Place* place) { this->place = place; }

  void unplaceArmy() { army = nullptr; }
  void placeArmy(Army* army);
  
  bool isCorrupted() const { return std::find_if(spells.begin(), spells.end(), [](const SpellCast& cast) { return cast.spell == Spells::CORRUPTION; }) != spells.end(); }
    
  void addSpell(const SpellCast& cast) { spells.push_back(cast); }
};

#endif
