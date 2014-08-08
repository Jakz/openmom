#ifndef _COMBAT_H_
#define _COMBAT_H_

#include "Common.h"
#include "Unit.h"
#include "Army.h"

#include <unordered_map>

class HitPoints;

class Damage
{
public:
  const enum class Type {
    SINGLE,
    EACH_SAME,
    EACH_DIFFERENT,
    EACH_FATAL
  } type;
  Damage(Type type) : type(type) { }
  virtual void apply() = 0;
};

class DamageSingle : public Damage
{
public:
  DamageSingle(s16 amount) : Damage(Type::SINGLE), amount(amount) { }
  const s16 amount;
};

class DamageEachSame : public Damage
{
public:
  DamageEachSame(s16 amount) : Damage(Type::EACH_SAME), amount(amount) { }
  const s16 amount;
};

class DamageEachDifferent : public Damage
{
public:
  DamageEachDifferent(hit_points&& amounts) : Damage(Type::EACH_SAME), amounts(amounts) { }
  const hit_points amounts;
};

class DamageEachFatal : public Damage
{
public:
  DamageEachFatal(std::vector<bool>&& amounts) : Damage(Type::EACH_FATAL), amounts(amounts) { }
  const std::vector<bool> amounts;
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
  CombatUnit(Unit* unit) : unit(unit), player(unit->getArmy()->getOwner()), moves(unit->getProperty(Property::MOVEMENT)), selected(false) { }
  
  Player* const getOwner() { return player; }
  
  void resetMoves() { moves = unit->getProperty(Property::MOVEMENT); }
  
  void setPosition(u16 x, u16 y) { this-> x = x; this->y = y; }
  void setPosition(u16 x, u16 y, u16 facing) { setPosition(x,y); this->facing = facing; }
  
  friend bool operator<(const CombatUnit &c1, const CombatUnit &c2);
  
  u16 x, y;
  s16 facing;
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
  
  CombatPosition(CombatUnit *unit) : x(unit->x), y(unit->y) { }
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
  std::list<CombatUnit*> allUnits;
  std::list<CombatUnit*> units[2];
  
  CombatUnit* selectedUnit;
  Player* current;
  
  s16 tiles[W][H];
  
  position_map currents, visited, incoming;
  
public:
  Combat(Army* a1, Army* a2);
  
  void endTurn();
  
  CombatUnit* unitAtTile(u16 x, u16 y);
  bool isTileEmpty(u16 x, u16 y) { return !unitAtTile(x, y); }
  
  const std::list<CombatUnit*>& enemyUnits(Player* player) { return player == players[0] ? units[1] : units[0]; }
  const std::list<CombatUnit*>& friendlyUnits(Player* player) { return player == players[1] ? units[0] : units[1]; }
  
  void attack(CombatUnit *u1, CombatUnit *u2);
  
  s16 relativeFacing(CombatUnit *u1, CombatUnit *u2);
  
  void moveUnit(CombatUnit *unit, u16 x, u16 y);
  
  void castEnchantment(const SpellCast &cast) { spells.push_back(cast); }
  
  const position_map& reachable(CombatUnit* unit);
  
  void deployUnits();
  
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





enum class DamageType : u8
{
  RANGED_MAGICAL_CHAOS,
  RANGED_MAGICAL_NATURE,
  RANGED_MAGICAL_LIFE,
  RANGED_MAGICAL_SORCERY,
  RANGED_MAGICAL_DEATH,
  
  RANGED_BOULDER,
  RANGED_NORMAL,
  
  THROWN,
  
  PHYSICAL_MELEE_ATTACK,
  PHYSICAL_MAGICAL_ATTACK,
  
  AREA_IMMOLATION,
  AREA_BLIZZARD
};

enum class AttackPriority : s8
{
  GAZE_DEFENDER = 0,
  THROWN_ATTACK = 0,
  BREATH_ATTACK = 0,
  
  GAZE_ATTACKER = 1,
  
  FIRST_STRIKE_ATTACK = 3,
  
  MELEE_ATTACK = 4,
  TOUCH_ATTACK = 4,
  
  NOT_AVAILABLE = -1
};

class Attack
{
protected:
  Attack(AttackPriority attackerPriority, AttackPriority defenderPriority, bool availableToAttacker = true, bool availableToDefender = true) :
    attackerPriority(attackerPriority), defenderPriority(defenderPriority), availableToAttacker(availableToAttacker), availableToDefender(availableToDefender) { }
public:
  const bool availableToAttacker, availableToDefender;
  const AttackPriority attackerPriority;
  const AttackPriority defenderPriority;
};

class PhysicalAttack : public Attack
{
public:
  PhysicalAttack(Property defense) : Attack(AttackPriority::MELEE_ATTACK, AttackPriority::MELEE_ATTACK, true, true), defense(defense) { }

  const Property defense;
};



#endif
