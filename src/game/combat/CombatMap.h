#pragma once

#include "Common.h"
#include "CombatUtils.h"

namespace combat
{
  /* walls are always made by 12 sections on a 4x4 grid around the town,
   they are enumerated starting from north corner in clockwise manner.
   */
  enum class WallType
  {
    NORTH_CORNER = 0,
    NORTH_EAST_WALL1,
    NORTH_EAST_WALL2,
    EAST_CORNER,
    SOUTH_EAST_WALL1,
    SOUTH_EAST_GATE,
    SOUTH_CORNER,
    SOUTH_WEST_WALL1,
    SOUTH_WEST_WALL2,
    WEST_CORNER,
    NORTH_WEST_WALL1,
    NORTH_WEST_WALL2,
    NO_WALL = -1
  };

  enum class RoadType
  {
    NORMAL,
    ENCHANTED,
    NONE
  };
  
  enum class TileType
  {
    GRASS,
    ROUGH,
    HILLS
  };
  
  enum class TileProp
  {
    NONE,
    ROCK,
    TREE,
    TREES
  };
  
  enum class TileBuilding
  {
    NONE,
    FORTRESS,
    TOWER_WIZARDRY,
    CAVE,
    RUINS,
    KEEP,
    TEMPLE,
    ANCIENT_RUINS,
    
    HOUSE
  };
  
  enum class CombatObject
  {
    MAIN_BUILDING,
    WALL
  };
  
  using TileSubType = u16;
  
  struct CombatEnvironment
  {
    enum class Type
    {
      GRASS,
      DESERT,
      MOUNTAIN,
      TUNDRA,
      OCEAN
    };
    
    Type type;
    Plane plane;
    
    bool operator==(const CombatEnvironment& other) const
    {
      return type == other.type && plane == other.plane;
    }
  };

  class CombatMap;

  struct CombatTile
  {
    CombatMap* map;
    CombatCoord coords;

    TileType type;
    TileProp prop;
    TileBuilding building;
    
    WallType stoneWall;
    WallType fireWall;
    WallType darknessWall;
    RoadType road;
    
    
    bool isStoneWallDestroyed;

    CombatTile() = default;
    CombatTile(CombatMap* map, s8 x, s8 y) : map(map), type(TileType::GRASS), prop(TileProp::NONE), coords(x,y),
    stoneWall(WallType::NO_WALL), fireWall(WallType::NO_WALL), darknessWall(WallType::NO_WALL), building(TileBuilding::NONE), road(RoadType::NONE), isStoneWallDestroyed(false) { }
    
    CombatTile* neighbour(Dir facing) const;
    
    s16 x() const { return coords.x; }
    s16 y() const { return coords.y; }
  };

  class CombatMap
  {
  private:
    using tile_functor = std::function<void(CombatTile*)>;
    
    CombatTile* tiles;
    const int W, H;

    /* this util function is used to be able to place different kind of walls with a single function */
    void placeWall(u16 x, u16 y, std::function<void(CombatTile*,WallType)> lambda);
    
    CombatTile* placeRoadSegment(u16 x, u16 y, Dir dir, u16 length, bool enchanted);
    
  public:
    //TODO: these shouldn't be exposed
    CombatTile* functorOnSegment(u16 x, u16 y, Dir dir, u16 length, tile_functor lambda);
    void functorOnRect(u16 x, u16 y, u16 w, u16 h, tile_functor lambda);
    
  public:
    CombatMap(u16 width, u16 height);
    ~CombatMap() { delete [] tiles; }
    
    const CombatTile* tileAt(s16 x, s16 y) const;
    CombatTile* tileAt(s16 x, s16 y);
    
    void placeStoneWall(u16 x, u16 y);
    void placeFireWall(u16 x, u16 y);
    void placeDarknessWall(u16 x, u16 y);
    
    void placeBuilding(u16 x, u16 y, TileBuilding building) { tileAt(x,y)->building = building; }
    
    void placeCityRoadExit(Dir direction);
    
    void placeSegment(u16 x, u16 y, Dir dir, u16 length, TileType type);
    void placeRect(u16 x, u16 y, u16 w, u16 h, TileType type);
    
    CombatTile* placeRiverSegment(u16 x, u16 y, Dir dir, u16 length);
  };
  
}
