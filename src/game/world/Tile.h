#ifndef _TILE_H_
#define _TILE_H_

#include "common/Common.h"
#include "Places.h"
#include "Spells.h"

#include "common/Util.h"

#include <list>

class World;
class City;

enum : size_t
{
  TILE_COUNT_OCEAN = 2,
  TILE_COUNT_FOREST = 3,
  TILE_COUNT_TUNDRA = 3,
  TILE_COUNT_SWAMP = 3,
  TILE_COUNT_DESERT = 4,
  TILE_COUNT_GRASSLANDS = 4,
  
  TILE_COUNT_HILLS = 16,
  TILE_COUNT_MOUNTAINS = 16,
  
  TILE_COUNT_RIVER_CAP = 1,
  TILE_COUNT_RIVER_CORNER = 3,
  TILE_COUNT_RIVER_STRAIGHT = 3,
  TILE_COUNT_RIVER_T_CROSS = 4,
  TILE_COUNT_RIVER_CROSS = 5,
  
  TILE_COUNT_SHORE = 256,
  TILE_COUNT_DESERT_JOIN = 256,
  TILE_COUNT_TUNDRA_JOIN = 256,
  
};

class Tile
{
private:
  World* world;
  
public:
  TileType type;
  u8 roads;
  
  DirJoin joinMask;
  u8 variant;
  
  Position position;
  
  ManaNode *node;
  Resource resource;
  Place *place;
  
  City* city;
  Army* army;
  
  bool hasRoad, hasEnchantedRoad;
  std::array<bool, 2> resourceUsed;
  
  std::list<const SpellCast> spells;
  
  s8 animationOffset;
  
  Tile() { }
  
  Tile(World* const world, Position position) : world(world), position(position), animationOffset(Util::randomIntUpTo(10)),
    resource(Resource::NONE), city(nullptr), army(nullptr), node(nullptr), place(nullptr), type(TILE_WATER), roads(0),
  
  joinMask(DirJoin::NONE), variant(0),
  hasRoad(false), hasEnchantedRoad(false), resourceUsed({false})
  {
  }
  
  u16 x() const { return position.x; }
  u16 y() const { return position.y; }
  
  void settleCity(City* city);
  void markResourceUsed() { if (resourceUsed[0]) resourceUsed[1] = true; else resourceUsed[0] = true; }
  void unmarkResourceUsed() { if (resourceUsed[1]) resourceUsed[1] = false; else resourceUsed[0] = false; }
  bool isResourceShared() const { return resourceUsed[0] && resourceUsed[1]; }
  
  void placeRoad(bool enchanted) { hasRoad = true; hasEnchantedRoad = enchanted; }
  void placeResource(Resource resource) { this->resource = resource; }
  void placeManaNode(ManaNode* node) { this->node = node; }
  void placePlace(Place* place) { this->place = place; }

  void unplaceArmy() { army = nullptr; }
  void placeArmy(Army* army);
  
  bool isCorrupted() const { return std::find_if(spells.begin(), spells.end(), [](const SpellCast& cast) { return cast.spell == Spells::CORRUPTION; }) != spells.end(); }
  bool isSolidLand() const { return type != TileType::TILE_WATER && type != TileType::TILE_SHORE; }
    
  void addSpell(const SpellCast& cast) { spells.push_back(cast); }
  
  Tile* neighbor(DirJoin dir) const;
  
  void for_each_neighbor(const std::function<void(Tile*)> lambda) const;
  
  DirJoin computeMask(const std::function<bool(const Tile*)> predicate) const;
};

#endif
