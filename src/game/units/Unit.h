#ifndef _UNIT_H_
#define _UNIT_H_

#include "Common.h"

#include "UnitSpec.h"
#include "SkillSet.h"

enum class Property : u8;

class Army;
class SkillSet;

class Unit
{
private:
  Army* army;

protected:
  Unit(const UnitSpec& spec) : spec(spec), army(nullptr), skills(*this) { }

public:
  const SkillSet skills;
  const UnitSpec& spec;
  
  void setArmy(Army* army) { this->army = army; }
  Army* getArmy() const { return army; }
  
  Productable::Type type() const { return spec.productionType(); }
  
  s16 getProperty(Property property) const { return 0; } // TODO
  
  void resetMoves() { } // TODO
  s16 availableMoves() const { return 0; } // TODO
  void useMoves(s16 moves) { } // TODO
  
  void turnBegin() { } // TODO
  
  const School school() const { return CHAOS; } // TODO
  

};

class Hero : public Unit
{
public:
  Hero(const HeroSpec& spec) : Unit(spec) { }
};

#endif
