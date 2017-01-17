#ifndef _COMBAT_H_
#define _COMBAT_H_

#include "Common.h"
#include "Unit.h"
#include "Army.h"
#include "CombatUtils.h"

#include <unordered_map>

class HitPoints;
class CombatMechanics;
class CombatMap;
struct CombatTile;

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
  CombatUnit(Unit* unit) : unit(unit), player(unit->getArmy()->getOwner()), moves(unit->getProperty(Property::MOVEMENT)), selected(false) { }
  
  Player* const getOwner() { return player; }
  
  void resetMoves() { moves = unit->getProperty(Property::MOVEMENT); }
  
  Unit* getUnit() const { return unit; }
  s16 getProperty(Property property) const { return unit->getProperty(property); }
  
  void setPosition(u16 x, u16 y) { position.position = CombatCoord(x,y); }
  void setPosition(u16 x, u16 y, Dir facing) { position = CombatPosition(x,y,facing); }
  void setPosition(CombatCoord tile) { position.position = tile; }
  void setFacing(Dir facing) { position.facing = facing; }
  
  u16 x() const { return position.position.x; }
  u16 y() const { return position.position.y; }
  Dir facing() const { return position.facing; }
  
  bool hasMoves() const { return moves > 0; }
  
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


class Combat
{
private:
  cast_list spells;
  Player* players[2];
  std::vector<CombatUnit*> allUnits;
  std::list<CombatUnit*> units[2];
  
  CombatMechanics* const mechanics;
  
  CombatUnit* selectedUnit;
  Player* current;
  
  std::unique_ptr<CombatMap> _map;
  
  position_map currents, visited, incoming;
  
public:
  static constexpr size_t W = 10;
  static constexpr size_t H = 20;
  
  Combat(Army* a1, Army* a2, CombatMechanics* mechanics);
  ~Combat();
  
  void endTurn();
  
  
  const CombatTile* tileAt(u16 x, u16 y);
  CombatUnit* unitAtTile(u16 x, u16 y);
  bool isTileEmpty(u16 x, u16 y) { return !unitAtTile(x, y); }

  CombatMap* map() { return _map.get(); }
  const std::list<CombatUnit*>& enemyUnits(Player* player) { return player == players[0] ? units[1] : units[0]; }
  const std::list<CombatUnit*>& friendlyUnits(Player* player) { return player == players[1] ? units[0] : units[1]; }
  
  void attack(CombatUnit *u1, CombatUnit *u2);
  
  Dir relativeFacing(CombatUnit *u1, CombatUnit *u2);
  
  void moveUnit(CombatUnit *unit, u16 x, u16 y);
  
  void castEnchantment(const SpellCast &cast) { spells.push_back(cast); }
  
  const position_map& reachable(CombatUnit* unit);
  
  void deployUnits();
  
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
};

#endif
