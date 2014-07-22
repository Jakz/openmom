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





constexpr const MovementEffect* const Effects::MOVEMENT_EFFECTS[];

const MovementEffect Effects::FORESTWALK = MovementEffect(SimpleEffect::Type::FORESTWALK, true);
const MovementEffect Effects::FLYING = MovementEffect(SimpleEffect::Type::FLYING, false);
const MovementEffect Effects::UNDERGROUND = MovementEffect(SimpleEffect::Type::UNDERGROUND, false);
const MovementEffect Effects::MOUNTAINWALK = MovementEffect(SimpleEffect::Type::MOUNTAINWALK, true);
const MovementEffect Effects::NON_CORPOREAL = MovementEffect(SimpleEffect::Type::NON_CORPOREAL, false);
const MovementEffect Effects::PATH_FINDER = MovementEffect(SimpleEffect::Type::PATH_FINDER, true);
const MovementEffect Effects::PLANAR_TRAVEL = MovementEffect(SimpleEffect::Type::PLANAR_TRAVEL, false);
const MovementEffect Effects::TELEPORT = MovementEffect(SimpleEffect::Type::TELEPORT, false);
const MovementEffect Effects::SWIMMING = MovementEffect(SimpleEffect::Type::SWIMMING, false);
const MovementEffect Effects::WINDWALK = MovementEffect(SimpleEffect::Type::WINDWALK, true);
const MovementEffect Effects::SAILING = MovementEffect(SimpleEffect::Type::SAILING, true);
const MovementEffect Effects::DESERTWALK = MovementEffect(SimpleEffect::Type::DESERTWALK, true);
const MovementEffect Effects::SWAMPWALK = MovementEffect(SimpleEffect::Type::SWAMPWALK, true);




