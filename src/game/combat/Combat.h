#ifndef _COMBAT_H_
#define _COMBAT_H_

#include "Common.h"
#include "Unit.h"
#include "Army.h"

#include <unordered_map>

class HitPoints;
class CombatMechanics;

enum class Facing
{
  NORTH = 0,
  NORTH_EAST,
  EAST,
  SOUTH_EAST,
  SOUTH,
  SOUTH_WEST,
  WEST,
  NORTH_WEST,
  
  INVALID = -1
};

enum class CombatModifier : u8
{
  MOD_HALF,
  MOD_NONE
};

struct CombatTile
{
  s8 x, y;
  CombatTile(s8 x, s8 y) : x(x), y(y) { }
  
  operator bool() const { return x != -1; }
  CombatTile neighbour(Facing facing) const;
};

struct CombatPosition
{
  CombatTile position;
  Facing facing;
  
  CombatPosition(s8 x = -1, s8 y = -1, Facing f = Facing::NORTH) : position(x, y), facing(f) { }
};

class CombatUnit
{
private:
  Unit* unit;
  Player* player;
  
public:
  CombatUnit(Unit* unit) : unit(unit), player(unit->getArmy()->getOwner()), moves(unit->getProperty(Property::MOVEMENT)), selected(false) { }
  
  Player* const getOwner() { return player; }
  
  void resetMoves() { moves = unit->getProperty(Property::MOVEMENT); }
  
  Unit* getUnit() const { return unit; }
  s16 getProperty(Property property) const { return unit->getProperty(property); }
  
  void setPosition(u16 x, u16 y) { position.position = CombatTile(x,y); }
  void setPosition(u16 x, u16 y, Facing facing) { position = CombatPosition(x,y,facing); }
  void setPosition(CombatTile tile) { position.position = tile; }
  void setFacing(Facing facing) { position.facing = facing; }
  
  u16 x() const { return position.position.x; }
  u16 y() const { return position.position.y; }
  Facing facing() const { return position.facing; }
  
  bool hasMoves() const { return moves > 0; }
  
  bool operator<(const CombatUnit &c2) const
  {
    if (position.position.x < position.position.x) return true;
    else if (position.position.y < position.position.y) return true;
    return false;
  }
  
  CombatPosition position;
  u16 moves;
  bool selected;
};

class CombatInterface
{
public:
  virtual CombatUnit* unitAtTile(u16 x, u16 y) = 0;
  virtual bool isPositionEmpty(u16 x, u16 y) = 0;
  
  virtual void attack(CombatUnit* u1, CombatUnit* u2) = 0;
  virtual void moveUnit(CombatUnit* u1, u16 x, u16 y) = 0;
  
  virtual void endTurn();
};

namespace std
{
  template<>
  struct hash<CombatPosition>
  {
    std::size_t operator()(const CombatPosition& k) const { return static_cast<u16>(k.position.x<<8 | k.position.y); }
  };
}

typedef std::unordered_map<CombatPosition,CombatPosition> position_map;

class Combat
{
private:
  static constexpr u8 W = 11;
  static constexpr u8 H = 19;
  
  cast_list spells;
  Player* players[2];
  std::vector<CombatUnit*> allUnits;
  std::list<CombatUnit*> units[2];
  
  CombatMechanics* const mechanics;
  
  CombatUnit* selectedUnit;
  Player* current;
  
  s16 tiles[W][H];
  
  position_map currents, visited, incoming;
  
public:
  Combat(Army* a1, Army* a2, CombatMechanics* mechanics);
  
  void endTurn();
  
  CombatUnit* unitAtTile(u16 x, u16 y);
  bool isTileEmpty(u16 x, u16 y) { return !unitAtTile(x, y); }
  
  const std::list<CombatUnit*>& enemyUnits(Player* player) { return player == players[0] ? units[1] : units[0]; }
  const std::list<CombatUnit*>& friendlyUnits(Player* player) { return player == players[1] ? units[0] : units[1]; }
  
  void attack(CombatUnit *u1, CombatUnit *u2);
  
  Facing relativeFacing(CombatUnit *u1, CombatUnit *u2);
  
  void moveUnit(CombatUnit *unit, u16 x, u16 y);
  
  void castEnchantment(const SpellCast &cast) { spells.push_back(cast); }
  
  const position_map& reachable(CombatUnit* unit);
  
  void deployUnits();
  void sortUnits();
  
  const std::vector<CombatUnit*>& getUnits() { return allUnits; }

  void deselect()
  {
    selectedUnit->selected = false;
    selectedUnit = nullptr;
  }
  
  void select(CombatUnit *unit)
  {
    if (selectedUnit) selectedUnit->selected = false;
    
    selectedUnit = unit;
    unit->selected = true;
  }
  
  const cast_list& getSpells() const { return spells; }
  
  
  
  static constexpr const s16 DIRS[12][2] = {{0,-2},{0,-1},{1,-1},{1,0},{0,1},{1,1},{0,2},{-1,1},{0,1},{-1,0},{-1,-1},{0,-1}};
  static constexpr const u16 DIRS_LENGTH = std::extent<decltype(DIRS)>::value;
  
  static const s16* dirs(int facing, bool even) { return dirs(static_cast<Facing>(facing), even); }
  static const s16* dirs(Facing facing, bool even)
  {
    switch (facing) {
      case Facing::NORTH: return DIRS[0];
			case Facing::NORTH_EAST: return even ? DIRS[1] : DIRS[2];
			case Facing::EAST: return DIRS[3];
			case Facing::SOUTH_EAST: return even ? DIRS[4] : DIRS[5];
			case Facing::SOUTH: return DIRS[6];
			case Facing::SOUTH_WEST: return even ? DIRS[7] : DIRS[8];
			case Facing::WEST: return DIRS[9];
			case Facing::NORTH_WEST: return even ? DIRS[10] : DIRS[11];
			default: return nullptr;
		}
  }
};

#endif
