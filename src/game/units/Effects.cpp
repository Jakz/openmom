#include "Effects.h"

#include "Unit.h"

const s16 ArmyBonus::getValue(Unit* unit) const {
  return (target == Type::WHOLE_ARMY || (target == Type::NORMAL_UNITS && unit->type() == Productable::Type::UNIT)) ? value : 0;
}




const MovementEffect Effects::FORESTWALK = MovementEffect(MovementEffectID::FORESTWALK, true);
const MovementEffect Effects::FLYING = MovementEffect(MovementEffectID::FLYING, false);
const MovementEffect Effects::UNDERGROUND = MovementEffect(MovementEffectID::UNDERGROUND, false);
const MovementEffect Effects::MOUNTAINWALK = MovementEffect(MovementEffectID::MOUNTAINWALK, true);
const MovementEffect Effects::NON_CORPOREAL = MovementEffect(MovementEffectID::NON_CORPOREAL, false);
const MovementEffect Effects::PATH_FINDER = MovementEffect(MovementEffectID::PATH_FINDER, true);
const MovementEffect Effects::PLANAR_TRAVEL = MovementEffect(MovementEffectID::PLANAR_TRAVEL, false);
const MovementEffect Effects::TELEPORT = MovementEffect(MovementEffectID::TELEPORT, false);
const MovementEffect Effects::SWIMMING = MovementEffect(MovementEffectID::SWIMMING, false);
const MovementEffect Effects::WINDWALK = MovementEffect(MovementEffectID::WINDWALK, true);
const MovementEffect Effects::SAILING = MovementEffect(MovementEffectID::SAILING, true);
const MovementEffect Effects::DESERTWALK = MovementEffect(MovementEffectID::DESERTWALK, true);
const MovementEffect Effects::SWAMPWALK = MovementEffect(MovementEffectID::SWAMPWALK, true);
