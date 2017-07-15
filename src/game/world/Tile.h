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
  
  std::unique_ptr<ManaNode> _node;
  std::unique_ptr<Place> _place;

public:
  TileType type;
  
  struct
  {
    DirJoin joinMask;
    DirJoin roadMask;
    u8 variant;

    s8 animationOffset;
    
  } gfx;

  Position position;

  Resource resource;
  
  City* city;
  Army* army;
  
  bool hasRoad, hasEnchantedRoad;
  std::array<bool, 2> resourceUsed;
  
  std::list<const SpellCast> spells;
  
  
  Tile() { }
  
  Tile(World* const world, Position position) : world(world), position(position),
    resource(Resource::NONE), city(nullptr), army(nullptr), _node(nullptr), _place(nullptr), type(TileType::OCEAN),
  
  gfx({DirJoin::NONE, DirJoin::NONE, 0, static_cast<s8>(Util::randi(10))}),
  hasRoad(false), hasEnchantedRoad(false), resourceUsed({false})
  {
  }
  
  Plane plane() const { return position.plane; }
  u16 x() const { return position.x; }
  u16 y() const { return position.y; }
  
  const std::unique_ptr<ManaNode>& node() const { return _node; }
  const std::unique_ptr<Place>& place() const { return _place; }
  
  void settleCity(City* city);
  void markResourceUsed() { if (resourceUsed[0]) resourceUsed[1] = true; else resourceUsed[0] = true; }
  void unmarkResourceUsed() { if (resourceUsed[1]) resourceUsed[1] = false; else resourceUsed[0] = false; }
  bool isResourceShared() const { return resourceUsed[0] && resourceUsed[1]; }
  
  void placeRoad(bool enchanted) { hasRoad = true; hasEnchantedRoad = enchanted; }
  void placeResource(Resource resource) { this->resource = resource; }
  void placeManaNode(ManaNode* node) { this->_node = std::unique_ptr<ManaNode>(node); }
  void placePlace(Place* place) { this->_place = std::unique_ptr<Place>(place); }

  void unplaceArmy() { army = nullptr; }
  void placeArmy(Army* army);
  
  bool isCorrupted() const { return std::find_if(spells.begin(), spells.end(), [](const SpellCast& cast) { return cast.spell == Spells::CORRUPTION; }) != spells.end(); }
  bool isSolidLand() const { return type != TileType::OCEAN && type != TileType::SHORE; }
    
  void addSpell(const SpellCast& cast) { spells.push_back(cast); }
  
  Tile* neighbor(DirJoin dir) const;
  
  void for_each_neighbor(const std::function<void(Tile*)> lambda) const;
  
  DirJoin computeMask(const std::function<bool(const Tile*)> predicate) const;
};

#endif
