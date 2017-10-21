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
