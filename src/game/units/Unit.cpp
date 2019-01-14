#include "Unit.h"

#include "Combat.h"
#include "Localization.h"
#include "Player.h"
#include "Game.h"

#include "GfxData.h" // required for name

#include <numeric>

using namespace std;

HitPoints::HitPoints(const Unit& unit) : unit(&unit), unit_figure_value(unit.getFigures())
{
  
}

float HitPoints::percentHealth() const
{
  return sum() / (float) (unit->getProperty(Property::FIGURES) * unit->getProperty(Property::HIT_POINTS) );
}

void HitPoints::healAll()
{
  assign(unit->getProperty(Property::FIGURES), unit->getProperty(Property::HIT_POINTS));
}

void HitPoints::applyDamage(value_t dmg)
{
  while (dmg > 0 && isAlive())
  {
    s16 v = std::min(dmg, operator[](0));
    
    operator[](0) -= v;
    dmg -= v;
    
    if (operator[](0) <= 0)
      erase(0);
    
    if (aliveCount() == 0)
      return;
  }
}

void HitPoints::applySameDamageToEachFigure(value_t dmg)
{
  this->operator-=(dmg);
  auto nend = remove_if(begin(), end(), [](value_t hp) { return hp <= 0; });
  erase(nend, end());
}

void HitPoints::applyDifferentDamageToEachFigure(const unit_figure_value& dmgs)
{
  this->operator-=(dmgs);
  
  auto nend = remove_if(begin(), end(), [](value_t& hp) { return hp <= 0; });
  erase(nend, end());
}

void HitPoints::killFigures(const unit_figure_flag& indices)
{
  assert(indices.size() == size());
  
  s16 current = 0;
  auto nend = remove_if(begin(), end(), [&](value_t) { return indices[current++]; });
  erase(nend, end());
}

void Unit::removeSpell(const Spell* spell)
{
  _skills.remove(spell);
  army->getOwner()->game()->playerMechanics.updateGlobalGains(army->getOwner());
}


prop_value Unit::getBaseProperty(Property property) const
{
  switch (property) {
    case Property::ALIVE_FIGURES:
      return _health.aliveCount();
    case Property::MOVEMENT_BASE_TYPE:
    {
      if (skills()->has(MovementType::FLYING))
        return static_cast<s16>(MovementBaseType::FLYING);
      else if (skills()->has(MovementType::SWIMMING))
        return static_cast<s16>(MovementBaseType::SWIMMING);
      else
        return static_cast<s16>(MovementBaseType::WALKING);
    }
    case Property::XP:
      return 1;
    case Property::RESIST_CHAOS:
    case Property::RESIST_NATURE:
    case Property::RESIST_SORCERY:
    case Property::RESIST_LIFE:
    case Property::RESIST_DEATH:
      return spec->getProperty(Property::RESIST);
    case Property::SHIELDS_RANGED:
    case Property::SHIELDS_CHAOS:
    case Property::SHIELDS_NATURE:
    case Property::SHIELDS_SORCERY:
    case Property::SHIELDS_LIFE:
    case Property::SHIELDS_DEATH:
      return spec->getProperty(Property::SHIELDS);
    default:
      return spec->getProperty(property);
  }
}

prop_value Unit::getBonusProperty(Property property) const
{
  int bonus = 0;
  
  if (property == Property::FIGURES || property == Property::MOVEMENT_BASE_TYPE)
    return 0;
  

  bonus += experience.getBonusProperty(property);
  
  switch (property)
  {
    case Property::SHIELDS_RANGED:
    case Property::SHIELDS_CHAOS:
    case Property::SHIELDS_NATURE:
    case Property::SHIELDS_SORCERY:
    case Property::SHIELDS_LIFE:
    case Property::SHIELDS_DEATH:
      bonus += getBonusProperty(Property::SHIELDS); //TODO: maybe SHIELDS_RANGED should be used
      break;
    case Property::RESIST_CHAOS:
    case Property::RESIST_NATURE:
    case Property::RESIST_SORCERY:
    case Property::RESIST_LIFE:
    case Property::RESIST_DEATH:
      bonus += getBonusProperty(Property::RESIST);
      break;
    default: break;
  }
  
  bonus += _skills.bonusForProperty(property);
  
  // if army is in combat then apply every possible global buff / debuff
  // TODO: use better check to see if it's in combat
  if (army && army->getOwner()->getCombat())
  {
    const cast_list& spells = army->getOwner()->getCombat()->getSpells();
    
    for (auto cast : spells)
    {
      const CombatEnchSpell* ench = cast.asCombatEnchSpell();

      if (ench && ench->effect.type == CombatEnchEffect::Type::UNIT_MODIFIER)
      {        
         const CombatEnchModifier& modifier = static_cast<const CombatEnchModifier&>(ench->effect);
         modifier.apply(army->getOwner()->getCombat(), cast, this, property);
      }
    }
  }
  
  if (property == Property::TO_HIT || property == Property::TO_DEFEND)
    bonus *= 10;
  
  return bonus;
}


const std::string RaceUnit::name() const {
  const RaceUnitSpec* rspec = static_cast<const RaceUnitSpec*>(spec);
  return i18n::s(GfxData::raceGfxSpec(rspec->race).unitName) + " " + spec->productionName();
}



#pragma mark Hero
const std::string Hero::title() const
{
  return "";
}

prop_value Hero::getBonusProperty(Property property) const
{
  value_t bonus = Unit::getBonusProperty(property);
  value_t bonusFromItems = std::accumulate(items.begin(), items.end(), 0, 
                                           [property](value_t v, const items::Item* item) { return item ? v + item->getBonusProperty(property) : v; }
  );

  return bonus + bonusFromItems;
}


