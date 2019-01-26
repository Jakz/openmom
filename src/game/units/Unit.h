#ifndef _UNIT_H_
#define _UNIT_H_

#include "common/Common.h"

#include "Items.h"
#include "Level.h"
#include "UnitSpec.h"
#include "SkillSet.h"
#include "Data.h"

enum class Property : u8;

class Army;
class SkillSet;
class Damage;

using unit_figure_flag = std::vector<bool>;

class HitPoints : public unit_figure_value
{
protected:
  const Unit* unit;
public:
  HitPoints(size_t count, value_t hits, value_t leadHits) : unit_figure_value(count), unit(nullptr)
  {
    assign(count, hits);
    operator[](0) = leadHits;
  }
  
  HitPoints(size_t count, value_t hits) : HitPoints(count, hits, hits) { }
  HitPoints(const Unit& unit);
  
  count_t aliveCount() const { return size(); }
  bool isAlive() const { return !empty(); }
  value_t hitsOfFigure(size_t index) const { return operator[](index); }
  value_t hitsOfLeadFigure() const { return operator[](0); }

  float percentHealth() const;
  
  void healAll();
  
  void applyDamage(value_t dmg);
  void applySameDamageToEachFigure(value_t dmg);
  void applyDifferentDamageToEachFigure(const unit_figure_value& dmgs);
  void killFigures(const unit_figure_flag& indices);
};

class Unit : public Propertable
{
public:
  const UnitSpec* const spec;
  
private:
  Army* army;

protected:
  Unit(const UnitSpec* spec) : spec(spec), army(nullptr), _skills(this), _health(*this), selected(true),
  experience(0, spec->type == UnitType::RACIAL ? &Data::experienceLevelsForUnits() : (spec->type == UnitType::HERO ? &Data::experienceLevelsForHeroes() : nullptr))
  {
    //TODO: commnted because t relies on ItemSlots which are initialized after in Hero class so it's UB
    //resetMoves();
    //_health.healAll();
  }
  
  SkillSet _skills;
  HitPoints _health;

  
  bool selected;
  
  experience_level experience;
  value_t availableMoves;

public:
  
  void setArmy(Army* army) { this->army = army; }
  Army* getArmy() const { return army; }
  
  //TODO: manage bonus levels with retorts and such through getBonusProperty
  const Level* getExperienceLevel() const { return experience.level(); }
  experience_t getExperience() const { return experience.xp(); }
  virtual value_t experienceMultiplier() const { return experience.level()->ordinal(); }
  
  Productable::Type type() const { return spec->productionType(); }
  
  prop_value getBaseProperty(Property property) const override;
  prop_value getBonusProperty(Property property) const override;
  
  void resetMoves() { availableMoves = getProperty(Property::MOVEMENT)*2; }
  value_t getAvailableMoves() const { return availableMoves; }
  void useMoves(value_t moves) { availableMoves -= moves; }
  
  bool isSelected() const { return selected;}
  void select() { selected = true; }
  void unselect() { selected = false; }
  
  Upkeep upkeep() const { return Upkeep(getProperty(Property::GOLD_UPKEEP), getProperty(Property::MANA_UPKEEP), getProperty(Property::FOOD_UPKEEP)) + spellUpkeep(); }
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
  
  Hero* asHero();
  const Hero* asHero() const;
};

class Hero : public Unit
{
protected:
  items::ItemSlots<3> _items;
  
public:
  Hero(const HeroSpec* spec) : Unit(spec), _items() { }

  prop_value getBonusProperty(Property property) const override;
  
  const std::string& firstName() const;
  const std::string name() const;
  
  decltype(_items)& items() { return _items; }
  const decltype(_items)& items() const { return _items; }
  
  void placeItem(size_t index, items::Item* item) { _items.set(index, item); }
  const items::Item* itemAt(size_t index) const { return _items[index]; }
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
  
  value_t experienceMultiplier() const override { return 1; }
};

inline Hero* Unit::asHero() { return type() == Productable::Type::HERO ? static_cast<Hero*>(this) : nullptr; }
inline const Hero* Unit::asHero() const { return type() == Productable::Type::HERO ? static_cast<const Hero*>(this) : nullptr; }


#endif
