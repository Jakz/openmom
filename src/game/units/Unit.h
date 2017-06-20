#ifndef _UNIT_H_
#define _UNIT_H_

#include "Common.h"

#include "Items.h"
#include "Level.h"
#include "UnitSpec.h"
#include "SkillSet.h"
#include "Data.h"

enum class Property : u8;

class Army;
class SkillSet;
class Damage;

using hit_points = std::vector<s16>;
using unit_figure_flag = std::vector<bool>;

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
  
  void applyDamage(s16 dmg);
  void applySameDamageToEachFigure(s16 dmg);
  void applyDifferentDamageToEachFigure(const hit_points& dmgs);
  void killFigures(const unit_figure_flag& indices);
};

class Unit : public Propertable
{
public:
  const UnitSpec* const spec;
  
private:
  Army* army;

protected:
  Unit(const UnitSpec* spec) : spec(spec), army(nullptr), _skills(*this), _health(*this), selected(true),
  experience(0, spec->type == UnitType::RACIAL ? &Data::experienceLevelsForUnits() : (spec->type == UnitType::HERO ? &Data::experienceLevelsForHeroes() : nullptr))
  {
    resetMoves();
    _health.healAll();
  }
  
  SkillSet _skills;
  HitPoints _health;

  
  bool selected;
  
  experience_level experience;
  s16 availableMoves;

public:
  
  void setArmy(Army* army) { this->army = army; }
  Army* getArmy() const { return army; }
  
  //TODO: manage bonus levels with retorts and such through getBonusProperty
  const Level* getExperienceLevel() const { return experience.level(); }
  s32 getExperience() const { return experience.xp(); }
  
  Productable::Type type() const { return spec->productionType(); }
  
  s16 getBaseProperty(Property property) const override;
  s16 getBonusProperty(Property property) const override;
  
  void resetMoves() { availableMoves = getProperty(Property::MOVEMENT)*2; }
  s16 getAvailableMoves() const { return availableMoves; }
  void useMoves(s16 moves) { availableMoves -= moves; }
  
  bool isSelected() const { return selected;}
  void select() { selected = true; }
  void unselect() { selected = false; }
  
  Upkeep upkeep() const { return spec->upkeep + spellUpkeep(); }
  Upkeep spellUpkeep() const { return Upkeep(0, _skills.spellsUpkeep(), 0); }
  void removeSpell(const Spell* spell);
  
  void turnBegin() {
    experience.increaseExperience(getProperty(Property::XP));
  }
  
  // TODO: more elaborate check according to chaos channels and things, possibly trhough a PropertyEffect
  bool isFantastic() const { return spec->type == UnitType::FANTASTIC; }
  const School school() const { return School::CHAOS; } // TODO
  
  SkillSet* skills() { return &_skills; }
  const SkillSet* skills() const { return &_skills; }

  HitPoints* health() { return &_health; }
  const HitPoints* health() const { return &_health; }
  
  virtual const std::string name() const { return spec->productionName(); }
  School glow() const { return _skills.glowEffect(); }
};

class Hero : public Unit
{
protected:
  std::array<items::Item*, 3> items;
  
public:
  Hero(const HeroSpec* spec) : Unit(spec), items({nullptr}) { }
  
  // const std::string name() const; TODO: name management
  const std::string title() const;
  
  const items::Item* itemAt(u16 index) const { return items[index]; }
  const HeroSpec* getSpec() const { return static_cast<const HeroSpec*>(spec); }
};

class RaceUnit : public Unit
{
public:
  RaceUnit(const RaceUnitSpec* spec) : Unit(spec) { } //TODO: starting level may change (according to buildings e wizard traits)
  
  const std::string name() const override;
};

class FantasticUnit : public Unit
{
public:
  FantasticUnit(const SummonSpec* spec) : Unit(spec) { }
};

#endif
