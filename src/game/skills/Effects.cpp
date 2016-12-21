#include "Effects.h"

#include "Unit.h"

const s16 UnitLevelBonus::getValue(const Unit* unit) const
{
  return static_cast<u16>(std::floor((unit->level->index()+1)*multiplier)); // TODO: is +1 intended behavior? According to OSG it is.
}


bool ArmyBonus::applicableOn(const Unit* unit) const
{
  return target == Type::WHOLE_ARMY || (target == Type::NORMAL_UNITS && unit->type() == Productable::Type::UNIT);
}

const s16 ArmyBonus::getValue(const Unit* unit) const {
  return applicableOn(unit) ? value : 0;
}

const s16 ArmyLevelBonus::getValue(const Unit* unit) const
{
  return applicableOn(unit) ? static_cast<u16>(std::floor((unit->level->index()+1)*multiplier)) : 0;
  // TODO: is +1 intended behavior? According to OSG it is.
}

const s16 FilterUnitBonus::getValue(const Unit* unit) const {
  return unit->school() == school ? value : 0;
}



namespace movement_effect_impl
{
  const MovementEffect FORESTWALK = MovementEffect(SimpleEffect::Type::FORESTWALK, true);
  const MovementEffect FLYING = MovementEffect(SimpleEffect::Type::FLYING, false);
  const MovementEffect UNDERGROUND = MovementEffect(SimpleEffect::Type::UNDERGROUND, false);
  const MovementEffect MOUNTAINWALK = MovementEffect(SimpleEffect::Type::MOUNTAINWALK, true);
  const MovementEffect NON_CORPOREAL = MovementEffect(SimpleEffect::Type::NON_CORPOREAL, false);
  const MovementEffect PATH_FINDER = MovementEffect(SimpleEffect::Type::PATH_FINDER, true);
  const MovementEffect PLANAR_TRAVEL = MovementEffect(SimpleEffect::Type::PLANAR_TRAVEL, false);
  const MovementEffect TELEPORT = MovementEffect(SimpleEffect::Type::TELEPORT, false);
  const MovementEffect SWIMMING = MovementEffect(SimpleEffect::Type::SWIMMING, false);
  const MovementEffect WINDWALK = MovementEffect(SimpleEffect::Type::WINDWALK, true);
  const MovementEffect SAILING = MovementEffect(SimpleEffect::Type::SAILING, true);
  const MovementEffect DESERTWALK = MovementEffect(SimpleEffect::Type::DESERTWALK, true);
  const MovementEffect SWAMPWALK = MovementEffect(SimpleEffect::Type::SWAMPWALK, true);
}


const MovementEffect* Effects::FORESTWALK = &movement_effect_impl::FORESTWALK;
const MovementEffect* Effects::FLYING = &movement_effect_impl::FLYING;
const MovementEffect* Effects::UNDERGROUND = &movement_effect_impl::UNDERGROUND;
const MovementEffect* Effects::MOUNTAINWALK = &movement_effect_impl::MOUNTAINWALK;
const MovementEffect* Effects::NON_CORPOREAL = &movement_effect_impl::NON_CORPOREAL;
const MovementEffect* Effects::PATH_FINDER = &movement_effect_impl::PATH_FINDER;
const MovementEffect* Effects::PLANAR_TRAVEL = &movement_effect_impl::PLANAR_TRAVEL;
const MovementEffect* Effects::TELEPORT = &movement_effect_impl::TELEPORT;
const MovementEffect* Effects::SWIMMING = &movement_effect_impl::SWIMMING;
const MovementEffect* Effects::WINDWALK = &movement_effect_impl::WINDWALK;
const MovementEffect* Effects::SAILING = &movement_effect_impl::SAILING;
const MovementEffect* Effects::DESERTWALK = &movement_effect_impl::DESERTWALK;
const MovementEffect* Effects::SWAMPWALK = &movement_effect_impl::SWAMPWALK;




