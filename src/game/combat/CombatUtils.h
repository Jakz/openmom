#pragma once

#include "Common.h"

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
  };

  struct combat_position_hash
  {
    std::size_t operator()(const CombatPosition& k) const { return static_cast<u16>(k.position.x<<8 | k.position.y); }
  };

  using position_map = std::unordered_map<CombatPosition,CombatPosition,combat_position_hash>;

}
