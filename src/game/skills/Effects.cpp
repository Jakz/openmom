#include "Effects.h"

#include "Unit.h"

template<typename ReturnType, typename T, typename F>
ReturnType Modifier<ReturnType, T, F>::transformValue(ReturnType previous, const T* owner) const
{
  switch (mode)
  {
    case Mode::ADDITIVE:
      return previous + (type == Type::FLOATING ? static_cast<ReturnType>(multiplier) : value);
    case Mode::ADDITIVE_PARAMETRIC:
      return previous + static_cast<ReturnType>(std::floor(F()(owner)*multiplier));
    case Mode::MULTIPLICATIVE:
      return static_cast<ReturnType>(std::floor(previous * multiplier));
    case Mode::FIXED:
      return value;
    default:
      assert(false);
      return 0;
  }
}

value_t UnitModifierLevelGetter::operator()(const Unit* unit) const { return unit->experienceMultiplier(); }

template class Modifier<value_t, Unit, UnitModifierLevelGetter>;

template class PropertyModifierEffect<WizardAttribute, Effect::Type::WIZARD_BONUS>;
template class PropertyModifierEffect<Property, Effect::Type::UNIT_BONUS>;
template class PropertyModifierEffect<Property, Effect::Type::ARMY_BONUS>;


effect_list effect_list::actuals(const Unit* unit) const
{
  std::unordered_multimap<const EffectGroup*, const Effect*> byGroup;

  using pair_t = const decltype(byGroup)::value_type;
  const auto sorterByMagnitude = [unit](const pair_t& e1, const pair_t& e2) { return e1.second->compare(unit, e2.second) == Order::LESSER; };
  static const auto sorterByPriority = [](const pair_t& e1, const pair_t& e2) { return e1.second->groupParam() < e2.second->groupParam(); };

  /* group effects by group */
  std::transform(data.begin(), data.end(), std::inserter(byGroup, byGroup.begin()), [] (const Effect* effect) { return std::make_pair(effect->group(), effect); });
  
  effect_list actuals;
  
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
effect_list effect_list::flatten()
{
  std::vector<const Effect*> data;
  std::copy(dbegin(), dend(), std::back_inserter(data));

  return data;
}
