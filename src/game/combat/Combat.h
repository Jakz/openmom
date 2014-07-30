#ifndef _COMBAT_H_
#define _COMBAT_H_

#include "Common.h"
#include "Unit.h"
#include "Army.h"

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
  Unit& unit;
  const Player* player;
  
public:
  CombatUnit(Unit& unit) : unit(unit), player(unit.getArmy()->getOwner()), moves(unit.getProperty(Property::MOVEMENT)) { }
  
  void resetMoves() { moves = unit.getProperty(Property::MOVEMENT); }
  
  void setPosition(u16 x, u16 y) { this-> x = x; this->y = y; }
  void setPosition(u16 x, u16 y, u16 facing) { setPosition(x,y); this->facing = facing; }
  
  friend bool operator<(const CombatUnit &c1, const CombatUnit &c2);
  
  u16 x, y;
  u16 facing;
  u16 moves;
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
  
public:
  const cast_list& getSpells() const { return spells; }
  
};

#endif
