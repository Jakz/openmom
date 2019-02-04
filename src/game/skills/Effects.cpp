#include "Effects.h"

#include "Unit.h"

template<typename EffectBase>
effect_list<EffectBase> effect_list<EffectBase>::actuals(const typename EffectBase::owner_type* unit) const
{
  std::unordered_multimap<const EffectGroup*, const EffectBase*> byGroup;

  using pair_t = const typename decltype(byGroup)::value_type;
  const auto sorterByMagnitude = [unit](const pair_t& e1, const pair_t& e2) { return e1.second->compare(unit, e2.second) == Order::LESSER; };
  static const auto sorterByPriority = [](const pair_t& e1, const pair_t& e2) { return e1.second->groupParam() < e2.second->groupParam(); };

  /* group effects by group */
  std::transform(data.begin(), data.end(), std::inserter(byGroup, byGroup.begin()), [] (const EffectBase* effect) { return std::make_pair(effect->group(), effect); });
  
  effect_list<EffectBase> actuals;
  
  for (auto it = byGroup.begin(); it != byGroup.end(); /**/)
  {
    const EffectGroup* group = it->first;
    
    auto pair = byGroup.equal_range(group);
    
    if (!group || group->mode()== EffectGroup::Mode::KEEP_ALL)
      std::transform(pair.first, pair.second, std::back_inserter(actuals), [] (const pair_t& entry) { return entry.second; });
    else
    {
      //TODO: write tests in Data YAML loading for preconditions about these modes
      switch (group->mode())
      {
        case EffectGroup::Mode::UNIQUE:
        {
          /* just push one, they should be all equal for the same group */
          actuals.push_back(pair.first->second);
          break;
        }
          
        case EffectGroup::Mode::KEEP_GREATER:
        {
          auto max = std::max_element(pair.first, pair.second, sorterByMagnitude);
          actuals.push_back(max->second);
          break;
        }
          
        case EffectGroup::Mode::KEEP_LESSER:
        {
          auto min = std::min_element(pair.first, pair.second, sorterByMagnitude);
          actuals.push_back(min->second);
          break;
        }

        case EffectGroup::Mode::PRIORITY:
        {
          /* higher has priority */
          auto min = std::min_element(pair.first, pair.second, sorterByPriority);
          actuals.push_back(min->second);
          break;
        }
          
        case EffectGroup::Mode::KEEP_ALL:
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

/* flatten nested effects */
template<typename EffectBase>
effect_list<EffectBase> effect_list<EffectBase>::flatten()
{
  std::vector<const EffectBase*> data;
  std::copy(dbegin(), dend(), std::back_inserter(data));

  return data;
}

template class SpecificModifierEffect<UnitEffect, UnitEffectType::WIZARD_BONUS, UnitModifierValue, WizardAttribute>;
template class SpecificModifierEffect<UnitEffect, UnitEffectType::UNIT_BONUS, UnitModifierValue, Property>;
template class SpecificModifierEffect<UnitEffect, UnitEffectType::ARMY_BONUS, UnitModifierValue, Property>;


template struct effect_list<UnitEffect>;
template struct effect_list_deep_iterator<UnitEffect>;

template struct effect_list<CityEffect>;
template struct effect_list_deep_iterator<CityEffect>;