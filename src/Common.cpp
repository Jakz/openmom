#include "Common.h"

#include "Localization.h"
#include "Gfx.h"
#include "Spells.h"

using namespace std;

static const Trait traitInfo[] = {
  {TraitID::ALCHEMY,1,NO_SCHOOL},
  {TraitID::WARLORD,2,NO_SCHOOL},
  {TraitID::CHANNELER,2,NO_SCHOOL},
  {TraitID::ARCHMAGE,1,NO_SCHOOL},
  {TraitID::ARTIFICER,1,NO_SCHOOL},
  {TraitID::CONJURER,1,NO_SCHOOL},
  {TraitID::SAGE_MASTER,1,NO_SCHOOL},
  {TraitID::MYRRAN,3,NO_SCHOOL},
  {TraitID::DIVINE_POWER,2,NO_SCHOOL},
  {TraitID::FAMOUS,2,NO_SCHOOL},
  {TraitID::RUNEMASTER,1,NO_SCHOOL},
  {TraitID::CHARISMATIC,1,NO_SCHOOL},
  {TraitID::CHAOS_MASTERY,1,CHAOS},
  {TraitID::NATURE_MASTERY,1,NATURE},
  {TraitID::SORCERY_MASTERY,1,SORCERY},
  {TraitID::INFERNAL_POWER,1,NO_SCHOOL},
  {TraitID::MANA_FOCUSING,1,NO_SCHOOL},
  {TraitID::NODE_MASTERY,1,NO_SCHOOL}
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

const Trait& Data::trait(const TraitID trait)
{
  return traitInfo[static_cast<u8>(trait)];
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
