//
//  CombatMechanics.cpp
//  OpenMoM
//
//  Created by Jack on 7/30/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "CombatMechanics.h"

#include <limits>

#include "Combat.h"
#include "CombatMap.h"
#include "common/Util.h"

using namespace combat;

u16 CombatMechanics::movementCostForTile(const CombatUnit* unit, const CombatTile* tile, Dir from)
{
  bool isLongMove = from == Dir::EAST || from == Dir::WEST || from == Dir::SOUTH || from == Dir::NORTH;
  
  if (unit->isFlying())
    return isLongMove ? 3 : 2;
  else if (tile->road != RoadType::NONE)
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

CombatPosition CombatMechanics::positionForDeployedUnit(CombatMap* map, const CombatUnit* unit, u16 index)
{
  static const CombatCoord attackerBase = {4,13};//{7,16};
  static const Dir attackerRowDirection = Dir::SOUTH_EAST;
  
  static const CombatCoord defenderBase = {4,10};
  static const Dir defenderRowDirection = Dir::NORTH_WEST;
  
  static const CoordJump jumps[] = { { Dir::SOUTH_WEST }, { Dir::NORTH_EAST, 2 }, { Dir::SOUTH_WEST, 3 } };
  
  const auto base = unit->side() == Side::DEFENDER ? defenderBase : attackerBase;
  const auto dir = unit->side() == Side::DEFENDER ? defenderRowDirection : attackerRowDirection;
  const auto facing = unit->side() == Side::DEFENDER ? Dir::SOUTH_EAST : Dir::NORTH_WEST;
  
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
    
    if (!isTileBlocked(tile, unit))
      ++i;
    else
      continue;
  }

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

bool CombatMechanics::isTileBlocked(const CombatTile* tile, const CombatUnit* unit)
{
  assert(unit && tile);
  const auto building = tile->building;
  const auto stoneWall = tile->stoneWall;
  const auto isWallDestroyed = tile->isStoneWallDestroyed;
  
  /* if tile has a building which is not an house then movement is blocked */
  bool hasBuilding = building != TileBuilding::HOUSE && building != TileBuilding::NONE;
  /* if tile has a stone wall tower then it is blocked */
  bool isBlockedByStoneTower = !isWallDestroyed && (stoneWall == WallType::EAST_CORNER || stoneWall == WallType::WEST_CORNER || stoneWall == WallType::SOUTH_CORNER || stoneWall == WallType::NORTH_CORNER);
  
  /* if tile has a non broken stone wall and unit doesn't have flying/non corporeal of teleport*/
  const auto* skills = unit->skills();
  bool isBlockedByStoneWall = unit->isAttacker() && stoneWall != WallType::NO_WALL && !isWallDestroyed && !(skills->has(MovementType::FLYING) || skills->has(MovementType::NON_CORPOREAL) || skills->has(MovementType::TELEPORT));
  
  return hasBuilding || isBlockedByStoneTower || isBlockedByStoneWall;
}

combat_pathfind_info CombatMechanics::reachableTiles(const Combat* combat, const CombatUnit* unit, s16 movement)
{
  static constexpr size_t DIRECTIONS = 8;
  
  const auto& map = combat->_map;
  
  struct node
  {
    const CombatTile* tile;
    s16 cost;
    Dir from;
    
    node() : node(nullptr, std::numeric_limits<s16>::max(), Dir::INVALID) { }
    node(const CombatTile* tile, s16 cost, Dir from) : tile(tile), cost(cost), from(from) { }
  };

  std::unordered_set<const CombatTile*> closedSet;
  std::list<node> openSet;
  std::unordered_map<const CombatTile*, node> closedMap;
  
  
  LOGD3("[combat][pathfind] computing reachable tiles from (%d,%d)", unit->x(), unit->y())
  
  openSet.push_back({ map->tileAt(unit->position), 0, Dir::INVALID });
  
  while (!openSet.empty())
  {
    const auto node = *openSet.begin();
    openSet.erase(openSet.begin());
    
    if (closedSet.find(node.tile) != closedSet.end())
      continue;
    
    closedSet.insert(node.tile);
    
    for (size_t i = 0; i < DIRECTIONS; ++i)
    {
      const Dir dir = static_cast<Dir>(i);
      const auto* neighbour = node.tile->neighbour(dir);
      
      bool alreadyExplored = closedSet.find(neighbour) != closedSet.end();
      bool blockedByEnvironment = !neighbour || isTileBlocked(neighbour, unit);
      bool blockedByOtherUnit = neighbour && combat->unitAtTile(neighbour->coords) != nullptr;
      
      /* tile is available to be reached */
      if (!alreadyExplored && neighbour && !blockedByEnvironment && !blockedByOtherUnit)
      {
        auto& entry = closedMap[neighbour];
        s16 cost = node.cost + movementCostForTile(unit, neighbour, dir);
        
        /* the new path found to reach neighbour is cheaper that previous one 
           then this route path should be used instead
         */
        if (cost < entry.cost)
          entry = { neighbour, cost, dir };
        
        if (cost < movement)
          openSet.push_back(entry);
      }
    }
  }
  
  combat_pathfind_info::set_t reachable;

  std::transform(closedMap.begin(), closedMap.end(), std::inserter(reachable, reachable.end()), [](const decltype(closedMap)::value_type& entry) {
    return CombatPosition(entry.first->x(), entry.first->y(), entry.second.from);
  });
  
  return combat_pathfind_info(unit->position, reachable);
}
