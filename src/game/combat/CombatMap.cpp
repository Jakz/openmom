#include "CombatMap.h"

#include "Util.h"

using namespace combat;

CombatTile* CombatTile::neighbour(Dir facing) const
{
  auto coords = this->coords.neighbour(facing);
  return map->tileAt(coords.x, coords.y);
}

CombatMap::CombatMap(u16 width, u16 height) : W(width), H(height), tiles(new CombatTile[width*height])
{
  for (int i = 0; i < W; ++i)
    for (int j = 0; j < H; ++j)
      *tileAt(i,j) = CombatTile(this, i, j, Util::randomIntUpTo(4));
}

const CombatTile* CombatMap::tileAt(s16 x, s16 y) const
{
  if (x < 0 || x >= W || y < 0 || y >= H)
    return nullptr;
  
  return &tiles[x + y*W];
}

CombatTile* CombatMap::tileAt(s16 x, s16 y)
{
  if (x < 0 || x >= W || y < 0 || y >= H)
    return nullptr;
  
  return &tiles[x + y*W];
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

CombatTile* CombatMap::placeRoadSegment(u16 x, u16 y, Dir dir, u16 length, bool enchanted)
{
  auto* tile = tileAt(x, y);
  auto* ptile = tile;
  size_t i = 0;
  
  do
  {
    ptile = tile;
    tile->road = enchanted ? RoadType::ENCHANTED : RoadType::NORMAL;
    tile = tile->neighbour(dir);
    ++i;
  } while (i < length);
  
  return ptile;
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

void CombatMap::placeCityRoadExit(Dir direction)
{
  // TODO: for now just straight segments, should be fixed by understanding how the original game places roads
  switch (direction)
  {
    case Dir::SOUTH_EAST:
    {
      placeRoadSegment(3, 10, Dir::SOUTH_EAST, 10, false);
      break;
    }
      
    case Dir::SOUTH_WEST:
    {
      placeRoadSegment(1, 10, Dir::SOUTH_WEST, 3, false);
      break;
    }
      
    case Dir::NORTH_EAST:
    {
      placeRoadSegment(3, 5, Dir::NORTH_EAST, 6, false);
      break;
    }
      
    default:
      assert(false);
      break;
  }
}
