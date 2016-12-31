#include "Unit.h"

#include "Combat.h"
#include "Localization.h"
#include "Player.h"
#include "Game.h"

#include <numeric>

using namespace std;

float HitPoints::percentHealth() const
{
  return accumulate(data.begin(), data.end(), 0);
}

void HitPoints::healAll()
{
  data.assign(unit.getProperty(Property::FIGURES), unit.getProperty(Property::HIT_POINTS));
}

void HitPoints::applyDamage(s16 dmg)
{
  while (dmg > 0 && isAlive())
  {
    s16 v = std::min(dmg, data[0]);
    
    data[0] -= v;
    dmg -= v;
    
    if (data[0] <= 0)
      data.erase(data.begin());
    
    if (aliveCount() == 0)
      return;
  }
}

void HitPoints::applySameDamageToEachFigure(s16 dmg)
{
  for_each(data.begin(), data.end(), [&](s16& hp){ hp -= dmg; });
  remove_if(data.begin(), data.end(), [](s16& hp) { return hp <= 0; });
}

void HitPoints::applyDifferentDamageToEachFigure(const hit_points& dmgs)
{
  for (int i = 0; i < dmgs.size(); ++i)
    data[i] -= dmgs[i];
  
  remove_if(data.begin(), data.end(), [](s16& hp) { return hp <= 0; });
}

void HitPoints::killFigures(const unit_figure_flag& indices)
{
  s16 current = 0;
  remove_if(data.begin(), data.end(), [&](s16& hp) { return indices[current++]; });
}

void Unit::removeSpell(const Spell* spell)
{
  _skills.remove(spell);
  army->getOwner()->game()->playerMechanics.updateGlobalGains(army->getOwner());
}


s16 Unit::getBaseProperty(Property property) const
{
  switch (property) {
    case Property::ALIVE_FIGURES:
      return _health.aliveCount();
    case Property::MOVEMENT_BASE_TYPE:
    {
      if (skills()->hasSkillEffect(Effects::FLYING))
        return static_cast<s16>(MovementBaseType::FLYING);
      else if (skills()->hasSkillEffect(Effects::SWIMMING))
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

s16 Unit::getBonusProperty(Property property) const
{
  int bonus = 0;
  
  if (property == Property::FIGURES || property == Property::MOVEMENT_BASE_TYPE)
    return 0;
  
  if (spec->productionType() != Productable::Type::SUMMON)
  {
    switch (property)
    {
      case Property::MELEE: bonus += level->bonuses[0]; break;
      case Property::RANGED: bonus += level->bonuses[0]; break;
      case Property::SHIELDS: bonus += level->bonuses[1]; break;
      case Property::RESIST: bonus  += level->bonuses[2]; break;
      case Property::HIT_POINTS: bonus += level->bonuses[3]; break;
      default: break;
    }
  }
  
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
      if (cast.spell->type == SpellType::COMBAT_ENCHANT)
      {
        const CombatEnchSpell* ench = cast.asCombatEnchSpell();
        
        if (ench->effect.type == CombatEnchEffect::Type::UNIT_MODIFIER)
        {
          const CombatEnchModifier& modifier = static_cast<const CombatEnchModifier&>(ench->effect);
          modifier.apply(army->getOwner()->getCombat(), cast, this, property);
        }
      }
    }
  }
  
  if (property == Property::TO_HIT || property == Property::TO_DEFEND)
    bonus *= 10;
  
  return bonus;
}


const std::string RaceUnit::name() const {
  const RaceUnitSpec* rspec = static_cast<const RaceUnitSpec*>(spec);
  return i18n::s(rspec->race.ident).unitName + " " + spec->productionName();
}



const std::string Hero::title() const
{
  return "";
}

