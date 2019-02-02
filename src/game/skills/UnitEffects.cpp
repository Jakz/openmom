#include "Effects.h"

#include "Unit.h"

value_t UnitModifierLevelGetter::operator()(const Unit* unit) const { return unit->experienceMultiplier(); }

template class Modifier<value_t, Unit, UnitModifierLevelGetter>;
