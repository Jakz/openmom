#include "Pathfind.h"

#include "Skill.h"
#include "Unit.h"
#include "World.h"
#include "Player.h"
#include "Game.h"

#if DEBUG >= 2
#include <sstream>
#endif

using namespace std;
using namespace pathfind;

bool MovementStrategy::movementAllowed(const movement_list& movement, const unit_list& units, const Player* owner, World* world, const Position& position) const
{
  Tile* t = world->get(position);
  
  if ((t->type == TILE_WATER || t->type == TILE_SHORE))
    return movement.contains(MovementType::NON_CORPOREAL) || movement.contains(MovementType::FLYING);
  else
    return true;
}

bool MovementStrategy::movementAllowedLast(const movement_list& movement, const unit_list& units, const Player* owner, World* world, const Position& position) const
{
  Tile* t = world->get(position);
  
  if (t && t->army && t->army->size() + owner->selectedCount() > 9)
    return true;
  
  return movementAllowed(movement, units, owner, world, position);
}

s16 MovementStrategy::cost(const movement_list& movement, const unit_list& units, const Player* owner, World* world, const Position& position) const
{
  Tile* t = world->get(position);
  
  if (t->hasEnchantedRoad)
    return 0;
  
  if (movement.contains(MovementType::FLYING)) // TODO: check values, things are missing too?
    return 2;
  
  return 2;
}

float MovementStrategy::heuristic(const movement_list& movement, const unit_list& units, const Player* owner, s16 w, s16 h, const Position start, const Position goal) const
{
  s16 ax = std::abs(start.x - goal.x);
  s16 tx = ax > w/2 ? w - ax : ax;
  s16 ty = std::abs(start.y - goal.y);
  
  return std::sqrt(static_cast<float>(tx*tx + ty*ty));
}


bool Route::stillValid(World *world)
{
  pending.clear();
  
  /* go through the route and check that movement is still allowed on each tile of the route */
  return std::any_of(positions.begin(), positions.end(), [world,this] (const Route::step_type& p) {
    return !PathFinder::strategy.movementAllowed(army->getOwner()->game()->mapMechanics.movementTypeOfArmy(army->getUnits()), army->getUnits(), army->getOwner(), world, p);
  });
}


void Route::consumeMovement(World *world)
{
  Tile* tile = world->get(army->getPosition());
  
  if (army->isPlaced())
    tile->unplaceArmy();
  
  pending.emplace_back(army->getPosition(), 0);
  
  s16 availMoves = army->availableMoves();
  
  Route::step_type currentMove = positions.front();
  optional<Route::step_type> lastMove;
  s16 totalCost = 0;
  
  while (availMoves > 0)
  {
    army->getOwner()->fog()->setRange(Position(currentMove.x, currentMove.y, army->getPosition().plane), army->sightRange());
    
    pending.push_back(currentMove);
    availMoves -= currentMove.cost;
    totalCost += currentMove.cost;
    
    lastMove = positions.front();
    positions.pop_front();
    
    if (!positions.empty())
      currentMove = positions.front();
    else
      break;
  }
  
  if (lastMove.isPresent())
  {
    world->get(lastMove->x, lastMove->y, army->getPosition().plane)->placeArmy(army);
    
    for (Unit* unit : *army)
      unit->useMoves(totalCost);
  }
}



const MovementStrategy PathFinder::strategy;

void PathFinder::computeReachable(World* world, const Position position, const movement_list& movement, const unit_list& units, const Player* player)
{
  reachable.clear();
  
  /* find all reachable tiles around the position: they are all 8 surrounding tiles
     assuming that the unit is allowed to walk in there
   */
  for (int xx = -1; xx <= 1; ++xx)
  {
    for (int yy = -1; yy <= 1; ++yy)
    {
      if (xx == 0 && yy == 0)
        continue;
      else
      {
        Position np = position.relative(xx, yy);
        
        // ignore tiles that goes out of the map vertically
        if (!np.wrapAndCheckValidity(w, h))
          continue;
        
        if (strategy.movementAllowed(movement, units, player, world, np))
          reachable.push_back(np);
      }
    }
  }
}

PathTileInfo* PathFinder::findOpenNode(Position position)
{
  auto it = std::find_if(openSet.begin(), openSet.end(), [&position] (PathTileInfo* node) { return *node == position; });
  return it != openSet.end() ? *it : nullptr;
}

const PathTileInfo* PathFinder::stepRoute(World *world, const Position goal, const movement_list &movement, const unit_list &units, const Player *player)
{
  PathTileInfo* current = * min_element(openSet.begin(), openSet.end(), [](const PathTileInfo* t1, const PathTileInfo*t2) { return t1->cost + t1->hCost < t2->cost + t2->hCost; });
  
  if (*current == goal)
    return current;
  
  openSet.erase(current);
  closedSet.insert(current);
  
  computeReachable(world, *current, movement, units, player);
  
#if DEBUG >= 2
  {
    std::stringstream ss;
    ss << "[pathfind] reachable: ";
    for (const auto& p : reachable) ss << "(" << p.x << "," << p.y << ") ";
    LOGD2("%s", ss.str().c_str());
  }
#endif
  
  for (const Position& tp : reachable)
  {
    int gameCost = strategy.cost(movement, units, player, world, tp);
    int testCost = current->cost + gameCost;
    
    PathTileInfo* testOpen = findOpenNode(tp);
    
    if (!testOpen || testCost < testOpen->gameCost)
    {
      if (!testOpen)
      {
        buffer.emplace_back(tp, current, testCost, strategy.heuristic(movement, units, player, w, h, tp, goal));
        openSet.insert(&buffer.back());
      }
      else
        testOpen->update(current, testCost, strategy.heuristic(movement, units, player, w, h, tp, goal));
    }
  }

  return nullptr;
}

Route* PathFinder::computeRoute(World* world, const Position start, const unit_list& units, const Player* player, Position goal)
{
  LOGD2("[pathfind] computing route from (%d,%d) to (%d,%d)", start.x, start.y, goal.x, goal.y)
  
  reset();
  
  // TODO: maybe it's redundant, search for calls around
  const movement_list movement = player->game()->mapMechanics.movementTypeOfArmy(units);
  
  goal.wrapAndCheckValidity(w, h);

  if (!strategy.movementAllowedLast(movement, units, player, world, goal))
    return nullptr;
  
  buffer.emplace_back(start, nullptr, 0, strategy.heuristic(movement, units, player, w, h, start, goal));
  openSet.insert(&buffer.back());
  
  while (!openSet.empty())
  {
    const PathTileInfo* finished = stepRoute(world, goal, movement, units, player);
    
    if (finished)
    {
      /* we finished and found a path */
      return reconstructPath(finished);
      reset();
    }
  }
  
  /* if, after the whole search, we didn't reach the goal tile then there is no route */
  reset();
  return nullptr;
}

Route* PathFinder::reconstructPath(const PathTileInfo* info)
{
  Route* r = new Route();
  
  while (info->parent)
  {
    r->prepend(RouteStep(*info, info->gameCost));
    info = info->parent;
  }
  
#if DEBUG >= 2
  {
    std::stringstream ss;
    ss << "[pathfind] found route: ";
    for (const auto& p : *r) ss << "(" << p.x << "," << p.y << ") ";
    LOGD2("%s", ss.str().c_str());
  }
#endif
  return r;
}


