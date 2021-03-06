#include "SkillSet.h"

#include "Unit.h"
#include "Player.h"
#include "Army.h"

#include <numeric>

using namespace std;

SkillSet::SkillSet(const Unit* unit) : unit(unit), nativeSkills(unit->spec->skills) { }

size_t SkillSet::size() const {
  size_t count = nativeSkills.size() + spells.size();
  
  if (unit)
  {
    const Hero* hero = unit->asHero();
    if (hero)
      count += hero->items().powers().size();
    
    if (unit->getArmy())
      count += unit->getArmy()->getOwner()->globalSkillSpellsCount(unit);
  }
  
  return count;
}

const Skill* SkillSet::get(size_t index) const
{
  size_t globalCount = unit && unit->getArmy() ? unit->getArmy()->getOwner()->globalSkillSpellsCount(unit) : 0,
         itemPowerCount = 0;
  
  const Hero* hero = unit ? unit->asHero() : nullptr;
  
  if (hero)
  {
    itemPowerCount = hero->items().powers().size();
  }
  
  if (index < itemPowerCount)
    return hero->items().powers()[index];
  else
    index -= itemPowerCount;

  if (index < spells.size())
    return spells[index].spell()->skill;
  else
    index -= spells.size();
  
  if (index < nativeSkills.size())
    return nativeSkills[index];
  else
    index -= nativeSkills.size();
  
  if (index < globalCount)
    return unit->getArmy()->getOwner()->nthGlobalSkillSpell(index, unit)->skill;
  else
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
  return accumulate(spells.begin(), spells.end(), 0,
                    [](value_t value, const auto& cast) { return value + cast.spell()->mana.upkeep; }
  );
}

value_t SkillSet::bonusForProperty(Property property) const
{
  unit_effect_list effects;

  //TODO: rewrite interely, this only works with additive property bonuses, doesnt't sort them by priority and such

  /* add all effects from skill units to final list */
  for (const Skill* skill : *this)
    effects += skill->getEffects();

  /* flatten list and remove overidden skills */
  effects = effects.actuals(unit).flatten();
  /* keep bonuses only */
  effects.filter<UnitPropertyBonus>(property);
  
  // add bonuses from specific ArmyBonus effect
  if (unit->getArmy())
  {
    for (const auto u : *unit->getArmy())
    {
      for (const Skill* skill : *u->skills())
      {        
        unit_effect_list seffects = skill->getEffects();
        seffects.filter<ArmyPropertyBonus>(property);
        effects += seffects;
      }
    }
  }

  effects = effects.actuals(unit);
  
  value_t bonus = 0;

  for (const auto* effect : effects)
    bonus = effect->as<ModifierEffect<UnitEffect, UnitModifierValue, Property>>()->transformValue(property, bonus, unit);

  return bonus;
}

value_t SkillSet::bonusForPlayerAttribute(WizardAttribute attribute) const
{
  unit_effect_list effects;
  for (const Skill* skill : *this)
    effects += skill->getEffects();

  effects.filter([](const UnitEffect* e) { return e->type == UnitEffectType::WIZARD_BONUS; });
  effects.sort();

  return effects.reduceAsModifier<WizardAttributeModifier>(attribute, unit);
}

bool SkillSet::hasSpell(const Spell* spell) const {
  return std::find(spells.begin(), spells.end(), spell) != spells.end();
}

bool SkillSet::hasSkill(const Skill* skill) const
{
  return std::find(this->begin(), this->end(), skill) != this->end();
}

bool SkillSet::has(const std::function<bool(const UnitEffect*)>& predicate) const
{
  return std::any_of(this->begin(), this->end(), [&predicate] (const Skill* skill) {
    return std::any_of(skill->getEffects().dbegin(), skill->getEffects().dend(), [&predicate] (const UnitEffect* effect) {
      return predicate(effect);
    });
  });
}

bool SkillSet::has(MovementType type) const {
  bool hasType = false, hasPreventType = false;
  
  forEachEffect([&hasType, &hasPreventType, type] (const UnitEffect* effect) {
    hasType |= effect->type == UnitEffectType::MOVEMENT && effect->as<MovementEffect>()->subType() == type;
    hasPreventType |= effect->type == UnitEffectType::DISALLOW_MOVEMENT && effect->as<MovementDisallowEffect>()->subType() == type;
  });
  
  return hasType && !hasPreventType;
}

bool SkillSet::hasSimpleEffect(SimpleEffect::Type type) const
{
  return skills::hasSimpleEffect(*this, type);
}

bool SkillSet::isImmuneTo(const EffectGroup* group) const
{
  bool hasImmunity = false;
  forEachEffect([group, &hasImmunity] (const UnitEffect* effect) {
    hasImmunity |= effect->type == UnitEffectType::IMMUNITY && effect->as<ImmunityToGroupEffect>()->group() == group;
  });
  return hasImmunity;
}

void SkillSet::forEachEffect(std::function<void(const UnitEffect*)> lambda) const
{
  std::for_each(begin(), end(), [&lambda] (const Skill* skill) {
    std::for_each(skill->getEffects().dbegin(), skill->getEffects().dend(), [&lambda] (const UnitEffect* effect) {
      lambda(effect);
    });
  });
}

School SkillSet::glowEffect() const
{
  auto it = std::max_element(spells.begin(), spells.end(), 
                             [](const auto& c1, const auto& c2) { return c1.spell()->mana.manaCost < c2.spell()->mana.manaCost; });
  
  return it != spells.end() ? it->spell()->school : School::NO_SCHOOL;
}
