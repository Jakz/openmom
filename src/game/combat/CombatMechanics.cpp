//
//  CombatMechanics.cpp
//  OpenMoM
//
//  Created by Jack on 7/30/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "CombatMechanics.h"

#include "Combat.h"
#include "CombatMap.h"
#include "Util.h"

using namespace combat;

u16 CombatMechanics::movementCostForTile(const CombatTile* tile, Dir from)
{
  bool isLongMove = from == Dir::EAST || from == Dir::WEST || from == Dir::SOUTH || from == Dir::NORTH;
  
  if (tile->road != RoadType::NONE)
    return isLongMove ? 2 : 1;
  else if (tile->prop == TileProp::TREE)
    return isLongMove ? 4 : 3;
  else if (tile->prop == TileProp::ROCK)
    return isLongMove ? 4 : 3;
  else if (tile->prop == TileProp::TREES)
    return isLongMove ? 5 : 4;
  else if (tile->type == TileType::HILLS)
    return isLongMove ? 5 : 4;
  else
    return isLongMove ? 3 : 2;
}

CombatPosition CombatMechanics::positionForDeployedUnit(CombatMap* map, u16 index, Side side)
{
  static const CombatCoord attackerBase = {7,16};
  static const Dir attackerRowDirection = Dir::SOUTH_EAST;
  
  static const CombatCoord defenderBase = {4,10};
  static const Dir defenderRowDirection = Dir::NORTH_WEST;
  
  static const CoordJump jumps[] = { { Dir::SOUTH_WEST }, { Dir::NORTH_EAST, 2 }, { Dir::SOUTH_WEST, 3 } };
  
  const auto base = side == Side::DEFENDER ? defenderBase : attackerBase;
  const auto dir = side == Side::DEFENDER ? defenderRowDirection : attackerRowDirection;
  const auto facing = side == Side::DEFENDER ? Dir::SOUTH_EAST : Dir::NORTH_WEST;
  
  auto rowStart = base;
  auto current = rowStart;
  for (u16 i = 0, r = 1; i < index; ++r)
  {
    if (r == 3)
    {
      rowStart = rowStart.neighbour(dir);
      current = rowStart;
      r = 0;
    }
    else
    {
      for (u16 c = 0; c < jumps[r-1].length; ++c)
        current = current.neighbour(jumps[r-1].direction);
    }
    
    
    const CombatTile* tile = map->tileAt(current.x, current.y);
    
    if (!isTileBlocked(tile))
      ++i;
    else
      continue;
  }

  printf("deploy %d at %d, %d\n", index, current.x, current.y);
  return CombatPosition(current, facing);
}

CombatCoord CombatMechanics::defaultPositionForObject(CombatObject object)
{
  switch (object)
  {
    case CombatObject::MAIN_BUILDING: return {3,8};
    case CombatObject::WALL: return {3, 6};
  }
}

bool CombatMechanics::isTileBlocked(const CombatTile* tile)
{
  const auto building = tile->building;
  const auto stoneWall = tile->stoneWall;
  
  bool hasBuilding = building != TileBuilding::HOUSE && building != TileBuilding::NONE;
  bool isBlockedByStoneWall = stoneWall == WallType::EAST_CORNER || stoneWall == WallType::WEST_CORNER || stoneWall == WallType::SOUTH_CORNER || stoneWall == WallType::NORTH_CORNER;
  
  return hasBuilding || isBlockedByStoneWall;
}
