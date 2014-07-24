#include "Spells.h"

#include "Localization.h"


//   SpecialSpell(I18 name, SpellRarity rarity, School school, SpellDuration duration, s16 researchCost, s16 manaCost, s16 manaCostDelta, Target Target) :

namespace spellimpl
{
  static SpecialSpell CORRUPTION = SpecialSpell(I18::SPELL_CORRUPTION, RARITY_COMMON, CHAOS, UNDEFINED, 100, 40, 0, Target::MAP_TILE);
  static SpecialSpell CHANGE_TERRAIN = SpecialSpell(I18::SPELL_CHANGE_TERRAIN, RARITY_UNCOMMON, NATURE, UNDEFINED, 500, 50, 0, Target::MAP_TILE);
  static SpecialSpell RAISE_VOLCANO = SpecialSpell(I18::SPELL_RAISE_VOLCANO, RARITY_UNCOMMON, CHAOS, UNDEFINED, 680, 200, 0, Target::MAP_TILE);

}



const Spell* Spells::CORRUPTION = &spellimpl::CORRUPTION;
const Spell* Spells::CHANGE_TERRAIN = &spellimpl::CHANGE_TERRAIN;
const Spell* Spells::RAISE_VOLCANO = &spellimpl::RAISE_VOLCANO;