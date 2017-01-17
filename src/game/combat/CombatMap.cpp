#include "CombatMap.h"

#include "Util.h"

CombatTile* CombatTile::neighbour(Dir facing)
{
  auto coords = this->coords.neighbour(facing);
  return map->tileAt(coords.x, coords.y);
}

CombatMap::CombatMap(u16 width, u16 height) : W(width), H(height), tiles(new CombatTile[width*height])
{
  for (int i = 0; i < W; ++i)
    for (int j = 0; j < H; ++j)
      *tileAt(i,j) = CombatTile(this, i, j, Util::randomIntUpTo(32));
}

void CombatMap::placeWall(u16 x, u16 y, std::function<void(CombatTile*,WallType)> lambda)
{
  auto* current = tileAt(x,y);
  
  static const WallType sections[] = {
    WallType::NORTH_CORNER,
    WallType::NORTH_EAST_WALL1,
    WallType::NORTH_EAST_WALL2,
    WallType::EAST_CORNER,
    WallType::SOUTH_EAST_WALL1,
    WallType::SOUTH_EAST_GATE,
    WallType::SOUTH_CORNER,
    WallType::SOUTH_WEST_WALL1,
    WallType::SOUTH_WEST_WALL2,
    WallType::WEST_CORNER,
    WallType::NORTH_WEST_WALL1,
    WallType::NORTH_WEST_WALL2
  };
  
  static const Dir neighbours[] = {
    Dir::SOUTH_EAST,
    Dir::SOUTH_EAST,
    Dir::SOUTH_EAST,
    Dir::SOUTH_WEST,
    Dir::SOUTH_WEST,
    Dir::SOUTH_WEST,
    Dir::NORTH_WEST,
    Dir::NORTH_WEST,
    Dir::NORTH_WEST,
    Dir::NORTH_EAST,
    Dir::NORTH_EAST
  };
  
  lambda(current, sections[0]);
  
  for (size_t i = 0; i < std::extent<decltype(neighbours)>::value; ++i)
  {
    current = current->neighbour(neighbours[i]);
    lambda(current, sections[i+1]);
  }
}

void CombatMap::placeStoneWall(u16 x, u16 y)
{
  placeWall(x, y, [](CombatTile* tile, WallType type) { tile->stoneWall = type; });
}

void CombatMap::placeFireWall(u16 x, u16 y)
{
  placeWall(x, y, [](CombatTile* tile, WallType type) { tile->fireWall = type; });
}

void CombatMap::placeDarknessWall(u16 x, u16 y)
{
  placeWall(x, y, [](CombatTile* tile, WallType type) { tile->darknessWall = type; });
}
