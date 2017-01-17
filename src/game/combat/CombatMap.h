#pragma once

#include "Common.h"
#include "CombatUtils.h"

/* walls are always made by 12 sections on a 4x4 grid around the town,
 they are enumerated starting from north corner in clockwise manner.
 */
enum class StoneWallType
{
  STONE_N_TOWER = 0,
  STONE_NE_WALL1,
  STONE_NE_WALL2,
  STONE_E_TOWER,
  STONE_SE_WALL,
  STONE_SE_GATE,
  STONE_S_TOWER,
  STONE_SW_WALL1,
  STONE_SW_WALL2,
  STONE_W_TOWER,
  STONE_NW_WALL1,
  STONE_NW_WALL2,
  STONE_NO_WALL = -1
};

class CombatMap;

struct CombatTile
{
  CombatMap* map;
  CombatCoord coords;

  s16 type;
  
  StoneWallType stoneWall;
  bool isStoneWallDestroyed : 1;
  bool hasFireWall : 1;
  bool hasDarknessWall : 1;
  
  CombatTile() = default;
  CombatTile(CombatMap* map, s8 x, s8 y, s16 type) : map(map), type(type), coords(x,y),
  stoneWall(StoneWallType::STONE_NO_WALL), isStoneWallDestroyed(false), hasFireWall(false), hasDarknessWall(false) { }
  
  CombatTile& neighbour(Dir facing);
};

class CombatMap
{
private:
  CombatTile* tiles;
  const int W, H;
  

public:
  CombatMap(u16 width, u16 height);
  ~CombatMap() { delete [] tiles; }
  
  const CombatTile& tileAt(u16 x, u16 y) const { return tiles[x + y*W]; }
  CombatTile& tileAt(u16 x, u16 y) { return tiles[x + y*W]; }
  
  void placeStoneWall(u16 x, u16 y);
};
