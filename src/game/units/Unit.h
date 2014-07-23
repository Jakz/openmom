#ifndef _UNIT_H_
#define _UNIT_H_

#include "Common.h"

#include "Items.h"
#include "Level.h"
#include "UnitSpec.h"
#include "SkillSet.h"

enum class Property : u8;

class Army;
class SkillSet;

namespace combat {
  class Damage;
}

typedef std::vector<s16> hit_points;

class HitPoints
{
protected:
  const Unit& unit;
  hit_points data;
public:
  HitPoints(const Unit& unit) : unit(unit)
  {
    
  }
  
  s16 aliveCount() const { return data.size(); }
  bool isAlive() const { return !data.empty(); }
  s16 hitsOfFigure(u16 index) const { return data[index]; }
  s16 hitsOfLeadFigure() const { return data[0]; }
  
  float percentHealth() const;
  
  void healAll();
  
  void applyDamage(const combat::Damage& damage);
  void applyDamage(s16 dmg);
  void applySameDamageToEachFigure(s16 dmg);
  void applyDifferentDamageToEachFigure(const hit_points& dmgs);
  void killFigures(const std::vector<bool>& indices);
};

class Unit
{
private:
  Army* army;

protected:
  Unit(const UnitSpec& spec, const Level* level) : spec(spec), army(nullptr), skills_(*this), health_(*this), xp(0), selected(true), level(level)
  {
    resetMoves();
    health_.healAll();
  }
  
  SkillSet skills_;
  HitPoints health_;

  
  bool selected;
  
  s16 xp;
  s16 availableMoves;

public:
  const UnitSpec& spec;
  
  void setArmy(Army* army) { this->army = army; }
  Army* getArmy() const { return army; }
  
  Productable::Type type() const { return spec.productionType(); }
  
  s16 getProperty(Property property) const { return getBaseProperty(property) + getBonusProperty(property); };
  s16 getBaseProperty(Property property) const;
  s16 getBonusProperty(Property property) const;
  
  void resetMoves() { availableMoves = getProperty(Property::MOVEMENT)*2; }
  s16 getAvailableMoves() const { return availableMoves; }
  void useMoves(s16 moves) { availableMoves -= moves; }
  
  bool isSelected() { return selected;}
  void select() { selected = true; }
  void unselect() { selected = false; }
  
  Upkeep upkeep() const { Upkeep u = spec.upkeep; u.add(speelUpkeep()); return u; }
  Upkeep speelUpkeep() const { return Upkeep(0, skills_.spellsUpkeep(), 0); }
  void removeSpell(const Spell* spell) { skills_.remove(spell); } // TODO: army.owner.game.playerMechanics.updateGlobalGains(army.owner);
  
  void turnBegin() {
    xp += getProperty(Property::XP);
    if (level && level->hasLeveled(xp))
      level = level->next;
  }
  
  const Level* level;
  const School school() const { return CHAOS; } // TODO
  
  const SkillSet* skills() const { return &skills_; }
  const HitPoints* health() const { return &health_; }
  
  virtual const std::string name() const { return spec.productionName(); }
  School glow() { return skills_.glowEffect(); }
};

class Hero : public Unit
{
protected:
  Item* items[Item::MAX_SLOTS];
  
public:
  Hero(const HeroSpec& spec) : Unit(spec, &HeroLevel::HERO) { }
  
  // const std::string name() const; TODO: name management
  const std::string title() const;
  
  const Item* itemAt(u16 index) const { return items[index]; }
};

class RaceUnit : public Unit
{
public:
  RaceUnit(const RaceUnitSpec& spec) : Unit(spec, &UnitLevel::RECRUIT) { } //TODO: starting level may change (according to buildings e wizard traits
};

class FantasticUnit : public Unit
{
public:
  FantasticUnit(const SummonSpec& spec) : Unit(spec, nullptr) { }
};

#endif
