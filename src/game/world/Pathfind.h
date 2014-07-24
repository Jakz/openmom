#ifndef _PATHFIND_H_
#define _PATHFIND_H_

#include "Common.h"
#include "Army.h"

#include <unordered_set>
#include <list>

class Unit;
class Player;
class World;
class Army;

class MovementStrategy
{
public:
  MovementStrategy() { }
  
  bool blocked(const movement_list& movement, const unit_list& units, const Player* owner, World* world, const Position& position) const;
  bool blockedLast(const movement_list& movement, const unit_list& units, const Player* owner, World* world, const Position& position) const;
  
  s16 cost(const movement_list& movement, const unit_list& units, const Player* owner, World* world, const Position& position) const;
  float heuristic(const movement_list& movement, const unit_list& units, const Player* owner, s16 w, s16 h, s16 x, s16 y, s16 dx, s16 dy) const;
};

struct PathTileInfo
{
  mutable const PathTileInfo* parent;
  mutable s16 cost;
  mutable float hCost;
  mutable s16 gameCost;
  
  s16 x, y;
  
  PathTileInfo() : x(-1), y(-1) { }
  PathTileInfo(const Position& pos) : x(pos.x), y(pos.y), parent(nullptr) { }
  PathTileInfo(s16 x, s16 y) : x(x), y(y), cost(0), parent(nullptr) { }
  void reset() { parent = nullptr; }
};

namespace std
{
  template<>
  struct hash<PathTileInfo>
  {
    std::size_t operator()(const PathTileInfo& k) const { return static_cast<u32>(k.x << 16 | k.y); }
  };
}

class Route
{
private:
  mutable Army* army;
  std::list<PathTileInfo> positions, pending;
  
public:
  bool completed() { return positions.empty(); }
  
  void setArmy(Army* army) const { this->army = army; }
  
  bool stillValid(World* world);
  
  void append(const PathTileInfo& info) { positions.push_back(info); }
  void prepend(const PathTileInfo& info) { positions.push_front(info); }
  
  s16 dx() { return positions.back().x; }
  s16 dy() { return positions.back().y; }
  
  bool has(s16 x, s16 y) { return std::find_if(positions.begin(), positions.end(), [&](const PathTileInfo& i){return i.x == x && i.y == y;}) != positions.end(); }
  
  void consumeMovement(World* world);
  
  const std::list<PathTileInfo>& pendingPositions() { return pending; }
  
  typedef std::list<PathTileInfo>::iterator iterator;
  typedef std::list<PathTileInfo>::const_iterator const_iterator;
  iterator begin() { return positions.begin(); }
  iterator end() { return positions.end(); }
  const_iterator begin() const { return positions.begin(); }
  const_iterator end() const { return positions.end(); }
};

class PathFinder
{
private:
  const s16 w, h;
  std::unordered_set<const PathTileInfo*> openSet, closedSet;
  PathTileInfo** info;
  
public:
  PathFinder(World* world);
  ~PathFinder();
  
  void reset()
  {
    for (int x = 0; x < w; ++x)
    {
      for (int y = 0; y < h; ++y)
      {
        info[x][y].reset();
      }
    }
    
    closedSet.clear();
    openSet.clear();
  }
  
  const Route* computeRoute(World* world, Army* army, s16 dx, s16 dy) { return computeRoute(world, army->getPosition(), army->getUnits(), army->getOwner(), dx, dy); }
  const Route* computeRoute(World* world, const Position& position, const unit_list& units, const Player* player, s16 x, s16 y);
  
  static const MovementStrategy strategy;
};

#endif
