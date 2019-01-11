#include "SkillSet.h"

#include "Unit.h"
#include "Player.h"
#include "Army.h"

#include <numeric>

using namespace std;

SkillSet::SkillSet(Unit& unit) : unit(unit), nativeSkills(unit.spec->skills) { }

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
  
  return nullptr;
}

void SkillSet::remove(const Spell* spell)
{
  auto it = std::find(spells.begin(), spells.end(), spell);
  if (it != spells.end())
    spells.erase(it);
}

value_t SkillSet::spellsUpkeep() const
{
  return accumulate(spells.begin(), spells.end(), 0, [](value_t value, const SpellCast& cast) { return value + cast.spell->mana.upkeep; });
}

prop_value SkillSet::bonusForProperty(Property property) const
{
  prop_value bonus = 0;

  effect_list effects;
  
  //TODO: rewrite interely, this only works with additive property bonuses, doesnt't sort them by priority and such

  // add bonuses from specific UnitBonus effect
  for (const Skill* skill : *this)
  {    
    for (const auto e : skill->getEffects())
    {
      const PropertyBonus* ub = e->as<UnitBonus>();
      if (ub && ub->sameProperty(property))
        effects.push_back(ub);
    }
  }
  
  // add bonuses from specific ArmyBonus effect
  if (unit.getArmy())
  {
    for (const auto u : *unit.getArmy())
    {
      for (const Skill* skill : *u->skills())
      {        
        for (const auto e : skill->getEffects())
        {
          const PropertyBonus* ub = e->as<ArmyBonus>();
          if (ub && ub->sameProperty(property))
            effects.push_back(ub);
        }
      }
    }
  }

  effects = effects.actuals(&unit);
  
  for (const auto* effect : effects)
    bonus += effect->as<PropertyBonus>()->getValue(&unit);

  return bonus;
}

bool SkillSet::hasSpell(const Spell* spell) const {
  return std::find_if(spells.begin(), spells.end(), [&](const SpellCast& c) { return c.spell == spell; }) != spells.end();
}

bool SkillSet::hasSkill(const Skill* skill) const
{
  return std::find_if(this->begin(), this->end(), [skill](const Skill* it) { return it == skill; }) != this->end();
}

bool SkillSet::has(const std::function<bool(const SkillEffect*)>& predicate) const
{
  return std::any_of(this->begin(), this->end(), [&predicate] (const Skill* skill) {
    return std::any_of(skill->getEffects().begin(), skill->getEffects().end(), [&predicate] (const SkillEffect* effect) {
      return predicate(effect);
    });
  });
}

bool SkillSet::has(MovementType type) const {
  return has([type](const SkillEffect* effect) {
    return effect->type == SkillEffect::Type::MOVEMENT && effect->as<MovementEffect>()->subType() == type;
  });
}

bool SkillSet::hasSkillEffect(const SkillEffect* effect) const
{
  for (auto skill : *this)
  {
    const effect_list& effects = skill->getEffects();
    
    for (const auto e : effects)
      if (e == effect)
        return true;
  }
  
  return false;
}

bool SkillSet::hasSimpleEffect(SimpleEffect::Type type) const
{
  return skills::hasSimpleEffect(*this, type);
}

School SkillSet::glowEffect() const
{
  School school = NO_SCHOOL;
  s16 mana = 0;
  for_each(spells.begin(), spells.end(), [&](const SpellCast& cast) { if (cast.spell->mana.manaCost > mana) { mana = cast.spell->mana.manaCost; school = cast.spell->school; } });
  return school;
}
