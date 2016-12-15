#include "SkillSet.h"

#include "Unit.h"
#include "Player.h"
#include "Army.h"

#include <numeric>

using namespace std;

SkillSet::SkillSet(Unit& unit) : unit(unit), nativeSkills(unit.spec.skills) { }

size_t SkillSet::size() const {
  Army* army = unit.getArmy();
  return nativeSkills.size() + additionalSkills.size() + spells.size() + (army ? army->getOwner()->globalSkillSpellsCount(&unit) : 0);
}

const Skill* SkillSet::get(size_t index) const
{
  size_t native = nativeSkills.size(), additional = additionalSkills.size(), armyc = unit.getArmy() ? unit.getArmy()->getOwner()->globalSkillSpellsCount(&unit) : 0;
  
  if (index < native)
    return nativeSkills[index];
  else if (index < native + additional)
    return additionalSkills[index - native];
  else if (index < native + additional + armyc)
    return unit.getArmy()->getOwner()->nthGlobalSkillSpell(index - native - additional, &unit)->skill;
  else
    return (*next(spells.begin(), index - native - additional - armyc)).asUnitSpell()->skill;
  
  return Skills::LARGE_SHIELD;
}

void SkillSet::remove(const Spell* spell)
{
  auto it = spells.begin();
  while (it != spells.end())
    if ((*it++).spell == spell)
      spells.erase(it);
}

s16 SkillSet::spellsUpkeep() const
{
  return accumulate(spells.begin(), spells.end(), 0, [](s16 value, const SpellCast& cast) { return value + cast.spell->mana.upkeep; });
}

s16 SkillSet::bonusForProperty(Property property) const
{
  s16 bonus = 0;
  
  // add bonuses from specific UnitBonus effect
  for (const Skill* skill : *this)
  {
    const effect_list& effects = skill->getEffects();
    
    for (const auto e : effects)
    {
      if (e->type == SkillEffect::Type::UNIT_BONUS)
      {
        const UnitBonus* ub = static_cast<const UnitBonus*>(e);
      
        if (ub->sameProperty(property)) bonus += ub->getValue(&unit);
      }
    }
  }
  
  // add bonuses from specific ArmyBonus effect
  if (unit.getArmy())
  {
    for (const auto u : *unit.getArmy())
    {
      for (const Skill* skill : *u->skills())
      {
        const effect_list& effects = skill->getEffects();
        
        for (const auto e : effects)
        {
          if (e->type == SkillEffect::Type::ARMY_BONUS)
          {
            const UnitBonus* ub = static_cast<const UnitBonus*>(e);
            if (ub->sameProperty(property)) bonus += ub->getValue(&unit);
          }
        }
      }

    }
  }
  
  return bonus;
}

bool SkillSet::hasSpellSkill(SkillBase base) const {
  return std::find_if(spells.begin(), spells.end(), [&](const SpellCast& c) { return c.asUnitSpell()->skill->base == base; }) != spells.end();
}

bool SkillSet::hasSkill(SkillBase base) const
{
  return std::find_if(this->begin(), this->end(), [&](const Skill* c) { return c->base == base; }) != this->end();
}

bool SkillSet::hasSkillEffect(const SkillEffect* effect) const
{
  for (auto skill : *this)
  {
    effect_list& effects = skill->getEffects();
    
    for (const auto e : effects)
      if (e == effect)
        return true;
  }
  
  return false;
}

bool SkillSet::hasSimpleEffect(SimpleEffect::Type type) const
{
  for (auto skill : *this)
  {
    effect_list& effects = skill->getEffects();
    for (const auto e : effects)
      if (e->type == SkillEffect::Type::ABILITY && static_cast<const SimpleEffect*>(e)->effect == type)
        return true;

  }
  
  return false;
}

School SkillSet::glowEffect() const
{
  School school = NO_SCHOOL;
  s16 mana = 0;
  for_each(spells.begin(), spells.end(), [&](const SpellCast& cast) { if (cast.spell->mana.manaCost > mana) { mana = cast.spell->mana.manaCost; school = cast.spell->school; } });
  return school;
}
