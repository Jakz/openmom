#pragma once

#include "Common.h"

#include <unordered_set>

namespace combat
{
  enum class Side
  {
    ATTACKER,
    DEFENDER
  };
  
  struct CoordJump
  {
    Dir direction;
    u16 length;
    
    CoordJump(Dir dir, u16 length = 1) : direction(dir), length(length) { }
  };

  struct CombatCoord : public Point
  {
    CombatCoord() = default;
    CombatCoord(s16 x, s16 y) : Point(x,y) { }
    CombatCoord(const Point& point) : Point(point) { }
    
    operator bool() const { return isValid(); }
    CombatCoord neighbour(Dir facing) const;
    
    static constexpr const s16 DIRS[12][2] = {{0,-2},{0,-1},{1,-1},{1,0},{0,1},{1,1},{0,2},{-1,1},{0,1},{-1,0},{-1,-1},{0,-1}};
    static constexpr const u16 DIRS_LENGTH = std::extent<decltype(DIRS)>::value;
    
    static const s16* dirs(int facing, bool even) { return dirs(static_cast<Dir>(facing), even); }
    static const s16* dirs(Dir facing, bool even)
    {
      switch (facing) {
        case Dir::NORTH: return DIRS[0];
        case Dir::NORTH_EAST: return even ? DIRS[1] : DIRS[2];
        case Dir::EAST: return DIRS[3];
        case Dir::SOUTH_EAST: return even ? DIRS[4] : DIRS[5];
        case Dir::SOUTH: return DIRS[6];
        case Dir::SOUTH_WEST: return even ? DIRS[7] : DIRS[8];
        case Dir::WEST: return DIRS[9];
        case Dir::NORTH_WEST: return even ? DIRS[10] : DIRS[11];
        default: return nullptr;
      }
    }
  };
  
  
  struct CombatPosition
  {
    CombatCoord position;
    Dir facing;
    
    CombatPosition(CombatCoord coord, Dir f = Dir::NORTH) : position(coord), facing(f) { }
    CombatPosition(s8 x = -1, s8 y = -1, Dir f = Dir::NORTH) : position(x, y), facing(f) { }
    
    bool operator==(const CombatPosition& position) const { return this->position == position.position; }
    bool operator==(const CombatCoord& position) const { return this->position == position; }
  };
  
  struct combat_coord_hash
  {
    size_t operator()(const CombatCoord& position) const { return std::hash<size_t>()(position.x || position.y << sizeof(decltype(position.y))*8); }
  };
  
  struct combat_position_hash
  {
    size_t operator()(const CombatPosition& position) const { return std::hash<size_t>()(position.position.x || position.position.y << sizeof(decltype(position.position.y))*8); }
  };
  
  class combat_pathfind_info
  {
  public:
    using set_t = std::unordered_set<CombatPosition, combat_position_hash>;
    
  private:
    CombatCoord start;
    set_t reachable;
    
  public:
    combat_pathfind_info() = default;
    combat_pathfind_info(CombatCoord start, set_t reachable) : start(start), reachable(reachable) { }
    
    using iterator = set_t::const_iterator;
    
    const iterator begin() const { return reachable.begin(); }
    const iterator end() const { return reachable.end(); }
    
    void clear()
    {
      reachable.clear();
      start = CombatCoord();
    }
    
    bool contains(CombatCoord position) const
    {
      return std::find(begin(), end(), position) != end();
    }
  };
}
