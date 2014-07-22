#include "CombatSpells.h"

#include "Spells.h"
#include "Army.h"
#include "Unit.h"


s16 CombatEnchModifier::apply(Combat* combat, const SpellCast& cast, CombatUnit* unit, Property property)
{
  if (cast.spell.target == Target::ENEMY_ARMY && cast.player != unit->unit->getArmy()->getOwner())
    return doApply(unit, property);
  else if (cast.spell.target == Target::FRIENDLY_ARMY && cast.player == unit->unit->getArmy()->getOwner())
    return doApply(unit, property);
  else if (cast.spell.target == Target::BOTH_ARMIES)
    return doApply(unit, property);
  
  return 0;
}