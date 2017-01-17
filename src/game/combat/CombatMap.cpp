#include "CombatMap.h"

#include "Util.h"

CombatTile& CombatTile::neighbour(Dir facing)
{
  auto coords = this->coords.neighbour(facing);
  return map->tileAt(coords.x, coords.y);
}

CombatMap::CombatMap(u16 width, u16 height) : W(width), H(height), tiles(new CombatTile[width*height])
{
  for (int i = 0; i < W; ++i)
    for (int j = 0; j < H; ++j)
      tileAt(i,j) = CombatTile(this, i, j, Util::randomIntUpTo(32));
}

void CombatMap::placeStoneWall(u16 x, u16 y)
{
  auto& current = tileAt(x,y);
  
  current.stoneWall = StoneWallType::STONE_N_TOWER;
  current = current.neighbour(Dir::SOUTH_EAST);
  current.stoneWall = StoneWallType::STONE_NE_WALL1;
  current = current.neighbour(Dir::SOUTH_EAST);
  current.stoneWall = StoneWallType::STONE_NE_WALL2;
  current = current.neighbour(Dir::SOUTH_EAST);
  current.stoneWall = StoneWallType::STONE_E_TOWER;
}
