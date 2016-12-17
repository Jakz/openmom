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
  NORTH_WEST
};

enum class CombatModifier : u8
{
  MOD_HALF,
  MOD_NONE
};

class CombatUnit
{
private:
  Unit* unit;
  Player* player;
  
public:
  CombatUnit(Unit* unit) : unit(unit), player(unit->getArmy()->getOwner()), moves(unit->getProperty(Property::MOVEMENT)), selected(false), facing(Facing::NORTH) { }
  
  Player* const getOwner() { return player; }
  
  void resetMoves() { moves = unit->getProperty(Property::MOVEMENT); }
  
  Unit* getUnit() const { return unit; }
  s16 getProperty(Property property) const { return unit->getProperty(property); }
  
  void setPosition(u16 x, u16 y) { this-> x = x; this->y = y; }
  void setPosition(u16 x, u16 y, Facing facing) { setPosition(x,y); this->facing = facing; }
  
  bool hasMoves() const { return moves > 0; }
  
  bool operator<(const CombatUnit &c2) const
  {
    if (x < c2.x) return true;
    else if (y < c2.y) return true;
    return false;
  }
  
  u16 x, y;
  Facing facing;
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

struct CombatPosition
{
  s8 x, y;
  s8 f;
  
  CombatPosition(CombatUnit *unit) : x(unit->x), y(unit->y), f(-1) { }
  CombatPosition(s8 x = -1, s8 y = -1, s8 f = -1) : x(x), y(x), f(x) { }
};

namespace std
{
  template<>
  struct hash<CombatPosition>
  {
    std::size_t operator()(const CombatPosition& k) const { return static_cast<u16>(k.x<<8 | k.y); }
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
  
  static const s16* dirs(s8 dir, bool even)
  {
    switch (dir) {
			case 0: return DIRS[0];
			case 1: return even ? DIRS[1] : DIRS[2];
			case 2: return DIRS[3];
			case 3: return even ? DIRS[4] : DIRS[5];
			case 4: return DIRS[6];
			case 5: return even ? DIRS[7] : DIRS[8];
			case 6: return DIRS[9];
			case 7: return even ? DIRS[10] : DIRS[11];
			default: return nullptr;
		}
  }
};

#endif
