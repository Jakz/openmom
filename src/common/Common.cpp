#include "Common.h"

#include "i18n/Localization.h"
#include "Gfx.h"
#include "Spells.h"

#include "LBX.h"
#include "Texture.h"

using namespace std;

static char buffer[512];
void debugprintf(const char* str, ...)
{
  va_list args;
  va_start (args, str);
  vsnprintf (buffer, 512, str, args);
  printf("%s\n", buffer);
}

SpriteInfo::SpriteInfo(TextureID texture, u16 i) : SpriteInfo(texture, i / Texture::get(texture)->cols, i % Texture::get(texture)->cols)
{
  static_assert(sizeof(SpriteInfo) == sizeof(SpriteInfo::data_type), "");
}

const SpriteSheet* SpriteInfo::sheet() const
{
  return isLBX() ? static_cast<const SpriteSheet*>(lbx::Repository::spriteFor(*this)) : Texture::get(texture());
}

u16 SpriteInfo::count() const
{
  assert(isLBX());
  const lbx::LBXSpriteData* data = static_cast<const lbx::LBXSpriteData*>(sheet());
  return data->count;
}

const Palette* SpriteInfo::palette() const
{
  assert(isLBX());
  const lbx::LBXSpriteData* data = static_cast<const lbx::LBXSpriteData*>(sheet());
  return data->palette;
}

ScreenCoord ScreenCoord::INVALID = ScreenCoord(-1,-1);
ScreenCoord ScreenCoord::ZERO = ScreenCoord(0,0);


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
  {WizardID::MERLIN, I18::WIZARD_NAME_MERLIN},
  {WizardID::RAVEN, I18::WIZARD_NAME_RAVEN},
  {WizardID::SHAREE, I18::WIZARD_NAME_SHAREE},
  {WizardID::LO_PAN, I18::WIZARD_NAME_LO_PAN},
  {WizardID::JAFAR, I18::WIZARD_NAME_JAFAR},
  {WizardID::OBERIC, I18::WIZARD_NAME_OBERIC},
  {WizardID::RJAK, I18::WIZARD_NAME_RJAK},
  {WizardID::SSS_RA, I18::WIZARD_NAME_SSS_RA},
  {WizardID::TAURON, I18::WIZARD_NAME_TAURON},
  {WizardID::FREYA, I18::WIZARD_NAME_FREYA},
  {WizardID::HORUS, I18::WIZARD_NAME_HORUS},
  {WizardID::ARIEL, I18::WIZARD_NAME_ARIEL},
  {WizardID::TLALOC, I18::WIZARD_NAME_TLALOC},
  {WizardID::KALI, I18::WIZARD_NAME_KALI}
};

const Trait& Data::trait(const TraitID trait)
{
  return traitInfo[static_cast<u8>(trait)];
}

const Wizard& Data::wizard(const WizardID wizard)
{
  return wizardInfo[static_cast<size_t>(wizard)];
}

