#include "Common.h"

#include "Localization.h"
#include "Gfx.h"

using namespace std;

static const Color schoolColors[] = {
  Gfx::color(0,0,0),
  Gfx::color(255, 0, 0),
  Gfx::color(0, 0, 0),
  Gfx::color(255, 255, 255),
  Gfx::color(0, 255, 0),
  Gfx::color(0, 0, 255)
};

static const Trait traitInfo[] = {
  {TRAIT_ALCHEMY,1,NO_SCHOOL},
  {TRAIT_WARLORD,2,NO_SCHOOL},
  {TRAIT_CHANNELER,2,NO_SCHOOL},
  {TRAIT_ARCHMAGE,1,NO_SCHOOL},
  {TRAIT_ARTIFICER,1,NO_SCHOOL},
  {TRAIT_CONJURER,1,NO_SCHOOL},
  {TRAIT_SAGE_MASTER,1,NO_SCHOOL},
  {TRAIT_MYRRAN,3,NO_SCHOOL},
  {TRAIT_DIVINE_POWER,2,NO_SCHOOL},
  {TRAIT_FAMOUS,2,NO_SCHOOL},
  {TRAIT_RUNEMASTER,1,NO_SCHOOL},
  {TRAIT_CHARISMATIC,1,NO_SCHOOL},
  {TRAIT_CHAOS_MASTERY,1,CHAOS},
  {TRAIT_NATURE_MASTERY,1,NATURE},
  {TRAIT_SORCERY_MASTERY,1,SORCERY},
  {TRAIT_INFERNAL_POWER,1,NO_SCHOOL},
  {TRAIT_MANA_FOCUSING,1,NO_SCHOOL},
  {TRAIT_NODE_MASTERY,1,NO_SCHOOL}
};

static const Wizard wizardInfo[] = {
  {I18_WIZARD_NAME_MERLIN},
  {I18_WIZARD_NAME_RAVEN},
  {I18_WIZARD_NAME_SHAREE},
  {I18_WIZARD_NAME_LO_PAN},
  {I18_WIZARD_NAME_JAFAR},
  {I18_WIZARD_NAME_OBERIC},
  {I18_WIZARD_NAME_RJAK},
  {I18_WIZARD_NAME_SSS_RA},
  {I18_WIZARD_NAME_TAURON},
  {I18_WIZARD_NAME_FREYA},
  {I18_WIZARD_NAME_HORUS},
  {I18_WIZARD_NAME_ARIEL},
  {I18_WIZARD_NAME_TLALOC},
  {I18_WIZARD_NAME_KALI}
};



Color Data::colorForSchool(const School school)
{
  return school < SCHOOL_COUNT ? schoolColors[school] : 0;
}

const Trait& Data::trait(const TraitID trait)
{
  return traitInfo[trait];
}

const Wizard& Data::wizard(const WizardID wizard)
{
  return wizardInfo[wizard];
}

const std::vector<const SpellKind>& Data::spellKinds(bool combat)
{
  static vector<const SpellKind> overland = {KIND_SUMMONING,KIND_SPECIAL,KIND_ENCHANTMENT,KIND_UNIT_SPELL};
  static vector<const SpellKind> combatb = {KIND_SUMMONING,KIND_SPECIAL,KIND_ENCHANTMENT,KIND_UNIT_SPELL,KIND_COMBAT_SPELL};
  return combat ? combatb : overland;
}
