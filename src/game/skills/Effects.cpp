#include "Effects.h"

#include "Unit.h"

s16 UnitLevelBonus::getValue(const Unit* unit) const
{
  return static_cast<u16>(std::floor((unit->getExperienceLevel()->index()+1)*multiplier)); // TODO: is +1 intended behavior? According to OSG it is.
}


bool ArmyBonus::applicableOn(const Unit* unit) const
{
  return target == Type::WHOLE_ARMY || (target == Type::NORMAL_UNITS && unit->type() == Productable::Type::UNIT);
}

s16 ArmyBonus::getValue(const Unit* unit) const {
  return applicableOn(unit) ? value : 0;
}
s16 ArmyLevelBonus::getValue(const Unit* unit) const
{
  return applicableOn(unit) ? static_cast<u16>(std::floor((unit->getExperienceLevel()->index()+1)*multiplier)) : 0;
  // TODO: is +1 intended behavior? According to OSG it is.
}

FilterUnitBonus::FilterUnitBonus(Property property, s16 value, School school) : UnitBonus(property, value),
  filter([school](const Unit* unit) { return unit->school() == school; }) { }

s16 FilterUnitBonus::getValue(const Unit* unit) const
{
  return filter(unit) ? value : 0;
}




effect_list effect_list::actuals(const Unit* unit) const
{
  std::unordered_multimap<const SkillEffectGroup*, const SkillEffect*> byGroup;

  using pair_t = const decltype(byGroup)::value_type;
  auto sorter = [unit](const pair_t& e1, const pair_t& e2) { return e1.second->compare(unit, e2.second) == SkillEffect::Order::LESSER; };

  std::transform(data.begin(), data.end(), std::inserter(byGroup, byGroup.begin()), [] (const SkillEffect* effect) { return std::make_pair(effect->group(), effect); });
  
  effect_list actuals;
  
  for (auto it = byGroup.begin(); it != byGroup.end(); /**/)
  {
    const SkillEffectGroup* group = it->first;
    
    auto pair = byGroup.equal_range(group);
    

    if (!group || group->mode()== SkillEffectGroup::Mode::KEEP_ALL)
      std::transform(pair.first, pair.second, std::back_inserter(actuals), [] (const pair_t& entry) { return entry.second; });
    else
    {
      //TODO: write tests in Data YAML loading for preconditions about these modes
      switch (group->mode())
      {
        case SkillEffectGroup::Mode::UNIQUE:
        {
          /* just push one, they should be all equal for the same group */
          actuals.push_back(pair.first->second);
          break;
        }
          
        case SkillEffectGroup::Mode::KEEP_GREATER:
        {
          auto max = std::max_element(pair.first, pair.second, sorter);
          actuals.push_back(max->second);
          break;
        }
          
        case SkillEffectGroup::Mode::KEEP_LESSER:
        {
          auto min = std::min_element(pair.first, pair.second, sorter);
          actuals.push_back(min->second);
          break;
        }
          
        case SkillEffectGroup::Mode::KEEP_ALL:
        {
          /* already handled in general case outside switch */
          break;
        }
      }
    }
    
    it = pair.second;
  }
  
  return actuals;
}
