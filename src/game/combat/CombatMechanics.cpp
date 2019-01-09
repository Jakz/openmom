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

#define COMBAT_LOG(y, ...) LOGG("[combat]", y, __VA_ARGS__)

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
    case CombatObject::MAIN_BUILDING: return { 3, 8 };
    case CombatObject::WALL: return { 3, 6 };
    default: assert(false); return { 0, 0 };
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
    value_t cost;
    Dir from;
    
    node() : node(nullptr, std::numeric_limits<s16>::max(), Dir::INVALID) { }
    node(const CombatTile* tile, value_t cost, Dir from) : tile(tile), cost(cost), from(from) { }
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
        value_t cost = node.cost + movementCostForTile(unit, neighbour, dir);
        
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

void CombatMechanics::castCombatInstant(const SpellCast& cast, const CombatUnit *unit)
{
  
}

#pragma mark CombatFormulas

value_t CombatFormulas::passingRollsf(value_t count, float chance)
{
  value_t passed = 0;
  for (value_t i = 0; i < count; ++i)
    if (Math::chance(chance)) ++passed;
  return passed;
}

value_t CombatFormulas::passingRolls(value_t count, value_t chance)
{
  u32 passed = 0;
  for (u32 i = 0; i < count; ++i)
    if (Math::chance(chance)) ++passed;
  return passed;
}

value_t CombatFormulas::computePhysicalDamage(const CombatUnit* target, value_t strength, School school, value_t toHit)
{
  /* make attack rolls for each alive figure */
  unit_figure_value registered_hits = unit_figure_value(target->getAliveFigures(), [strength, toHit](size_t index) { return passingRolls(strength, toHit); });

  /* build the set of hits for each living figure */
  unit_figure_value defender_hp = unit_figure_value(target->getAliveFigures(), target, [](const CombatUnit* unit, size_t index) { return unit->getUnit()->health()->hitsOfFigure(index); });

  /* total amount of hits that should be handled*/
  value_t totalPossibleHits = registered_hits.sum();
  value_t effectiveHits = 0;
  auto currentDefender = defender_hp.begin();

  /* physical damage for multi figure units will make a separate defense roll for each unblocked damage on the lead figure */
  /* while there's still damage to be applied and there are still defenders */
  while (totalPossibleHits > 0 && currentDefender != defender_hp.end())
  {
    /* figure makes defense rolls which are removed from potential hits */
    value_t defendedHitsByFigure = passingRolls(target->getDefenseForSchool(school), target->getProperty(Property::TO_DEFEND));
    totalPossibleHits - defendedHitsByFigure;

    /* we apply to the figure at most its hp as damage */
    value_t hitsToFigure = std::min(*currentDefender, totalPossibleHits);

    /* we update values */
    totalPossibleHits -= hitsToFigure;
    currentDefender -= hitsToFigure;
    effectiveHits += hitsToFigure;

    assert(totalPossibleHits == 0 || *currentDefender == 0);

    /* we go to next unit */
    ++currentDefender;
  }

  return effectiveHits ;
}


value_t CombatFormulas::computeAreaDamage(CombatUnit* target, value_t strength, School school, value_t toHit)
{
  /* make attack rolls for each alive figure */
  unit_figure_value registered_hits = unit_figure_value(target->getAliveFigures(), [strength, toHit] (size_t index) { return passingRolls(strength, toHit); });
  
  /* make defend rolls for each alive figure */
  unit_figure_value defended_hits = unit_figure_value(target->getAliveFigures(),
                                                      [target, school] (size_t index) { return passingRolls(target->getDefenseForSchool(school), target->getProperty(Property::TO_DEFEND)); }
                                                      );
  
  /* remove blocked damage */
  unit_figure_value effective_hits = registered_hits - defended_hits;
  effective_hits.clampNegativeValuesToZero();
  /* clamp values, damage per figure is limited by maximum hp per figure */
  effective_hits.forEach([target](value_t& v) { v = std::min(v, target->getProperty(Property::HIT_POINTS)); });

  value_t totalDamage = effective_hits.sum();

  COMBAT_LOG("computing area damage of magnitude %d to %s", strength, target->getUnit()->name().c_str());
  COMBAT_LOG("  attacking with %d%% hit, defending with %d shields and %d%% chance", toHit, target->getDefenseForSchool(school), target->getProperty(Property::TO_DEFEND));
  for (s32 i = 0; i < target->getAliveFigures(); ++i)
    COMBAT_LOG("    damage for figure %d: %2d - %2d = %2d (%2d)", i+1, registered_hits[i], defended_hits[i], registered_hits[i] - defended_hits[i], effective_hits[i]);
  COMBAT_LOG("  total damage: %d", totalDamage);

  return totalDamage;
}
