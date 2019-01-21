#include "Spells.h"

#include "Localization.h"

using namespace std;

//   SpecialSpell(I18 name, SpellRarity rarity, School school, SpellDuration duration, s16 researchCost, s16 manaCost, s16 manaCostDelta, Target Target) :

namespace spellimpl
{
  /* LIFE - COMMON */
  static UnitSpell ENDURANCE = UnitSpell(I18::SPELL_ENDURANCE, SpellRarity::COMMON, LIFE, 60, 30, -1, 1, Skills::SPELL_ENDURANCE);
  
  
  
  
  
  static SpecialSpell CORRUPTION = SpecialSpell(I18::SPELL_CORRUPTION, SpellRarity::COMMON, CHAOS, SpellDuration::UNDEFINED, 100, 40, 0, Target::MAP_TILE);
  static SpecialSpell CHANGE_TERRAIN = SpecialSpell(I18::SPELL_CHANGE_TERRAIN, SpellRarity::UNCOMMON, NATURE, SpellDuration::UNDEFINED, 500, 50, 0, Target::MAP_TILE);
  static SpecialSpell RAISE_VOLCANO = SpecialSpell(I18::SPELL_RAISE_VOLCANO, SpellRarity::UNCOMMON, CHAOS, SpellDuration::UNDEFINED, 680, 200, 0, Target::MAP_TILE);

}

const vector<SpellKind>& Spells::spellKinds(bool combat)
{
  static const vector<SpellKind> overland = {SpellKind::SUMMONING,SpellKind::SPECIAL,SpellKind::ENCHANTMENT,SpellKind::UNIT_SPELL};
  static const vector<SpellKind> combatb = {SpellKind::SUMMONING,SpellKind::SPECIAL,SpellKind::ENCHANTMENT,SpellKind::UNIT_SPELL,SpellKind::COMBAT_SPELL};
  return combat ? combatb : overland;
}

const Spell* Spells::ENDURANCE = &spellimpl::ENDURANCE;

const Spell* Spells::CORRUPTION = &spellimpl::CORRUPTION;
const Spell* Spells::CHANGE_TERRAIN = &spellimpl::CHANGE_TERRAIN;
const Spell* Spells::RAISE_VOLCANO = &spellimpl::RAISE_VOLCANO;


vector<vector<vector<const Spell*> > > SPELLS =
{
  // arcane
  { {}, {}, {}, {}, },
  // chaos
  { {}, {}, {}, {}, },
  // death
  { {}, {}, {}, {}, },
  // life
  { {}, {}, {}, {}, },
  // nature
  { {}, {}, {}, {}, },
  // sorcery
  { {}, {}, {}, {}, },
};

spell_list& Spells::spellsByRarityAndSchool(SpellRarity rarity, School school)
{
  return SPELLS[(size_t)school][(size_t)rarity];
}

Spell::Spell(I18 name, SpellType type, SpellRarity rarity, SpellKind kind, SpellDuration duration, School school, Target target, const ManaInfo mana) :
name(name), type(type), rarity(rarity), kind(kind), duration(duration), school(school), target(target), mana(mana), help(nullptr)
{
  //TODO: non funziona SPELLS[school][rarity].push_back(this);
}


