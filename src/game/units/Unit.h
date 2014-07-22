#ifndef _UNIT_H_
#define _UNIT_H_

#include "Common.h"

#include "UnitSpec.h"

enum class Property : u8;

class Army;

class Unit
{
private:
  Army* army;

protected:
  Unit(const UnitSpec& spec) : spec(spec), army(nullptr) { }

public:
  
  void setArmy(Army* army) { this->army = army; }
  Army* getArmy() { return army; }
  
  Productable::Type type() { return spec.productionType(); }
  
  s16 getProperty(Property property) const { return 0; } // TODO
  
  void resetMoves() { } // TODO
  s16 availableMoves() const { return 0; } // TODO
  void useMoves(s16 moves) { } // TODO
  
  void turnBegin() { } // TODO
  
  bool hasSkillEffect(const SkillEffect* effect) const { return false; } // TODO
  bool hasSpellSkill(const SkillBase skill) const { return false; } // TODO
  
  const UnitSpec& spec;
};

class Hero : public Unit
{
public:
  Hero(const HeroSpec& spec) : Unit(spec) { }
};

#endif
