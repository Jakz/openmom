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

bool MovementStrategy::movementAllowed(const movement_list& movement, const unit_list& units, const Player* owner, World* world, const Position& position) const
{
  Tile* t = world->get(position);
  
  if ((t->type == TILE_WATER || t->type == TILE_SHORE))
    return movement.find(&Effects::NON_CORPOREAL) != movement.end() || movement.find(&Effects::FLYING) != movement.end();
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
  
  if (movement.find(&Effects::FLYING) != movement.end()) // TODO: check values, things are missing too?
    return 2;
  
  return 2;
}

float MovementStrategy::heuristic(const movement_list& movement, const unit_list& units, const Player* owner, s16 w, s16 h, s16 x, s16 y, s16 dx, s16 dy) const
{
  s16 ax = std::abs(x - dx);
  s16 tx = ax > w/2 ? w - ax : ax;
  s16 ty = std::abs(y - dy);
  
  return std::sqrt(static_cast<float>(tx*tx + ty*ty));
}


bool Route::stillValid(World *world)
{
  pending.clear();
  
  /* go through the route and check that movement is still allowed on each tile of the route */
  return std::any_of(positions.begin(), positions.end(), [world,this] (const PathTileInfo& p) {
    Position pos = Position(p.x,p.y,army->getPosition().plane);
    return !PathFinder::strategy.movementAllowed(army->getOwner()->game()->mapMechanics.movementTypeOfArmy(army->getUnits()), army->getUnits(), army->getOwner(), world, pos);
  });
}


void Route::consumeMovement(World *world)
{
  Tile* tile = world->get(army->getPosition());
  
  if (army->isPlaced())
    tile->unplaceArmy();
  
  pending.push_back(PathTileInfo(army->getPosition()));
  
  /* TODO: not good! it must consider cost of single cells, not going just this way */
  s16 availMoves = army->availableMoves();
  
  PathTileInfo& currentMove = positions.front();
  PathTileInfo lastMove = PathTileInfo(-1, -1);
  s16 totalCost = 0;
  
  while (availMoves > currentMove.cost)
  {
    army->getOwner()->fog()->setRange(Position(currentMove.x, currentMove.y, army->getPosition().plane), army->sightRange());
    
    pending.push_back(currentMove);
    availMoves -= currentMove.gameCost;
    totalCost += currentMove.gameCost;
    
    lastMove = positions.front();
    positions.pop_front();
    
    if (!positions.empty())
      currentMove = positions.front();
    else
      break;
  }
  
  if (lastMove.x != -1)
  {
    world->get(lastMove.x, lastMove.y, army->getPosition().plane)->placeArmy(army);
    
    auto it = army->begin();
    while (it != army->end())
      (*it)->useMoves(totalCost);
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

Route* PathFinder::computeRoute(World* world, const Position& position, const unit_list& units, const Player* player, s16 dx, s16 dy)
{
  LOGD2("[pathfind] computing route from (%d,%d) to (%d,%d)", position.x, position.y, dx, dy)
  
  reset();
  
  // TODO: maybe it's redundant, search for calls around
  const movement_list movement = player->game()->mapMechanics.movementTypeOfArmy(units);
  
  const s16 x = position.x;
  const s16 y = position.y;
  
  if (dx < 0) dx = w + dx;
  else if (dx >= w) dx %= w;
  
  if (!strategy.movementAllowedLast(movement, units, player, world, position))
    return nullptr;
  
  auto& startTile = info[x][y];
  startTile.cost = 0;
  startTile.hCost = strategy.heuristic(movement, units, player, w, h, x, y, dx, dy);
  openSet.insert(&startTile);
  
  while (!openSet.empty())
  {
    const PathTileInfo *current = *min_element(openSet.begin(), openSet.end(), [](const PathTileInfo* t1, const PathTileInfo*t2) { return t1->cost + t1->hCost < t2->cost + t2->hCost; });
    
    if (current->x == dx && current->y == dy)
      break;
    
    openSet.erase(current);
    closedSet.insert(current);
    
    computeReachable(world, Position(current->x, current->y, position.plane), movement, units, player);
    
#if DEBUG >= 2
    {
      std::stringstream ss;
      ss << "[pathfinder] reachable: ";
      for (const auto& p : reachable) ss << "(" << p.x << "," << p.y << ") ";
      LOGD2("%s", ss.str().c_str());
    }
#endif

    for (const Position& tp : reachable)
    {
      s16 xp = tp.x, yp = tp.y;
      
      int gameCost = strategy.cost(movement, units, player, world, tp);
      int cost = current->cost + gameCost;
      bool better = false;
      PathTileInfo* n = &info[xp][yp];
      info[xp][yp].gameCost = gameCost;
      
      if (closedSet.find(n) != closedSet.end())
        continue;
      
      if (openSet.find(n) == openSet.end())
      {
        openSet.insert(n);
        better = true;
      }
      else if (cost < n->cost)
        better = true;
      
      if (better)
      {
        n->parent = current;
        n->cost = cost;
        n->hCost = strategy.heuristic(movement, units, player, w, h, xp, yp, dx, dy);
        openSet.erase(n);
        closedSet.insert(n); // TODO: check if should be openSet or closedSet
      }
    }
  }
  
  // if, after the whole search, the parent of destination is null then no path has been found
  if (!info[dx][dy].parent)
    return nullptr;
  
  Route* r = new Route();
  const PathTileInfo* l = &info[dx][dy];
  
  while (!(l->x == x && l->y == y))
  {
    r->prepend(*l);
    l = l->parent;
  }
  
  return r;
}



PathFinder::PathFinder(World* world, s16 w, s16 h) : w(w), h(h)
{
  info = new PathTileInfo*[w];
  
  for (int x = 0; x < w; ++x)
  {
    info[x] = new PathTileInfo[h];
  }
  
  for (int x = 0; x < w; ++x)
    for (int y = 0; y < h; ++y)
      info[x][y] = PathTileInfo(x, y);
}

PathFinder::~PathFinder()
{
  for (int x = 0; x < w; ++x)
    delete [] info[x];
  
  delete [] info;
}



