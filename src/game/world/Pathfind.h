#ifndef _PATHFIND_H_
#define _PATHFIND_H_

#include "common/Common.h"
#include "Army.h"

#include <unordered_set>
#include <list>

class Unit;
class Player;
class World;
class Army;

namespace pathfind
{
  struct OverworldContext
  {
    const World* world;
    const Player* player;
    const unit_list& units;
    const movement_list& movement;
  };

  class MovementStrategy
  {
  public:
    MovementStrategy() { }
    
    bool movementAllowed(const movement_list& movement, const unit_list& units, const Player* owner, World* world, const Position& position) const;
    bool movementAllowedLast(const movement_list& movement, const unit_list& units, const Player* owner, World* world, const Position& position) const;
    
    s16 cost(const movement_list& movement, const unit_list& units, const Player* owner, World* world, const Position& position) const;
    float heuristic(const movement_list& movement, const unit_list& units, const Player* owner, s16 w, s16 h, const Position start, const Position goal) const;
  };
  
  struct PathTileInfo : public Position
  {
    mutable const PathTileInfo* parent;
    mutable s16 cost;
    mutable float hCost;
    mutable s16 gameCost;
    
    //PathTileInfo() = default;
    PathTileInfo(Position pos, const PathTileInfo* parent, s16 gameCost, float hCost)
    : Position(pos), parent(parent), gameCost(gameCost), hCost(hCost), cost(gameCost+hCost) { }
    
    void update(PathTileInfo* parent, s16 gameCost, float hCost)
    {
      this->parent = parent;
      this->gameCost = gameCost;
      this->hCost = hCost;
      this->cost = gameCost + hCost;
    }
    
    bool operator==(const Position pos) const { return pos.x == x && pos.y == y; }
    
    void reset() { parent = nullptr; }
  };
  
  struct PathTileInfoHash
  {
    inline size_t operator()(const PathTileInfo& k) const { return std::hash<u32>()((k.x << 16) | k.y); }
  };
  
  struct RouteStep : public Position
  {
    s16 cost;
    RouteStep() = default;
    RouteStep(Position p, s16 cost) : Position(p), cost(cost) { }
    
    bool operator==(const Position pos) const { return pos.x == x && pos.y == y; }
  };

  class Route
  {
  public:
    using step_type = RouteStep;
    
  private:
    mutable Army* army;
    std::list<step_type> positions, pending;
    
  public:
    bool completed() { return positions.empty(); }
    
    void setArmy(Army* army) const { this->army = army; }
    
    bool stillValid(World* world);
    
    void append(const step_type& info) { positions.push_back(info); }
    void prepend(const step_type& info) { positions.push_front(info); }
    
    s16 dx() { return positions.back().x; }
    s16 dy() { return positions.back().y; }
    
    bool passesBy(Position position) { return std::find(positions.begin(), positions.end(), position) != positions.end(); }
    
    void consumeMovement(World* world);
    
    const std::list<step_type>& pendingPositions() { return pending; }
    
    typedef std::list<step_type>::iterator iterator;
    typedef std::list<step_type>::const_iterator const_iterator;
    iterator begin() { return positions.begin(); }
    iterator end() { return positions.end(); }
    const_iterator begin() const { return positions.begin(); }
    const_iterator end() const { return positions.end(); }
  };
  
  class PathFinder
  {
  private:
    const s16 w, h;
    std::list<PathTileInfo> buffer;
    std::unordered_set<PathTileInfo*> openSet, closedSet;
    std::vector<Position> reachable;
    
    PathTileInfo* findOpenNode(Position position);
    Route* reconstructPath(const PathTileInfo* goal);
    
    void computeReachable(World* world, const Position position, const movement_list& movement, const unit_list& units, const Player* player);
    const PathTileInfo* stepRoute(World* world, const Position goal, const movement_list& movement, const unit_list& units, const Player* player);
    
  public:
    PathFinder(World* world, s16 w, s16 h) : w(w), h(h) { }
    ~PathFinder() { }
    
    void reset()
    {
      buffer.clear();
      closedSet.clear();
      openSet.clear();
      reachable.clear();
    }
    
    Route* computeRoute(World* world, Army* army, const Position goal) { return computeRoute(world, army->getPosition(), army->getUnits(), army->getOwner(), goal); }
    Route* computeRoute(World* world, const Position start, const unit_list& units, const Player* player, Position goal);
    
    static const MovementStrategy strategy;
  };
  
  class PathFinder2
  {
  private:
    
  };
  
}

#endif
