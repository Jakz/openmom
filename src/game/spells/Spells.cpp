#include "Spells.h"

#include "Localization.h"

using namespace std;

//   SpecialSpell(I18 name, SpellRarity rarity, School school, SpellDuration duration, s16 researchCost, s16 manaCost, s16 manaCostDelta, Target Target) :

namespace spellimpl
{
  /* LIFE - COMMON */
  static UnitSpell BLESS = UnitSpell(I18::SPELL_BLESS, RARITY_COMMON, LIFE, COMBAT_CONTINUOUS, 20, 25, 5, 1, Skills::SPELL_BLESS);
  static UnitSpell ENDURANCE = UnitSpell(I18::SPELL_ENDURANCE, RARITY_COMMON, LIFE, CONTINUOUS, 60, 30, -1, 1, Skills::SPELL_ENDURANCE);
  //TODO static SummonSpell GUARDIAN_SPIRIT = SummonSpell(I18::SPELL_GUARDIAN_SPIRIT, RARITY_COMMON, LIFE, 220, 80, -1, )    no way to obtain constant summon spec
  
  
  
  
  
  static SpecialSpell CORRUPTION = SpecialSpell(I18::SPELL_CORRUPTION, RARITY_COMMON, CHAOS, UNDEFINED, 100, 40, 0, Target::MAP_TILE);
  static SpecialSpell CHANGE_TERRAIN = SpecialSpell(I18::SPELL_CHANGE_TERRAIN, RARITY_UNCOMMON, NATURE, UNDEFINED, 500, 50, 0, Target::MAP_TILE);
  static SpecialSpell RAISE_VOLCANO = SpecialSpell(I18::SPELL_RAISE_VOLCANO, RARITY_UNCOMMON, CHAOS, UNDEFINED, 680, 200, 0, Target::MAP_TILE);

}

const vector<const SpellKind>& Spells::spellKinds(bool combat)
{
  static vector<const SpellKind> overland = {KIND_SUMMONING,KIND_SPECIAL,KIND_ENCHANTMENT,KIND_UNIT_SPELL};
  static vector<const SpellKind> combatb = {KIND_SUMMONING,KIND_SPECIAL,KIND_ENCHANTMENT,KIND_UNIT_SPELL,KIND_COMBAT_SPELL};
  return combat ? combatb : overland;
}

const Spell* Spells::BLESS = &spellimpl::BLESS;
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
  return SPELLS[school][rarity];
}

Spell::Spell(I18 name, SpellType type, SpellRarity rarity, SpellKind kind, SpellDuration duration, School school, Target target, const ManaInfo mana) :
name(name), type(type), rarity(rarity), kind(kind), duration(duration), school(school), target(target), mana(mana)
{
  //TODO: non funziona SPELLS[school][rarity].push_back(this);
}


