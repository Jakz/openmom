#include "SpellEffectAnim.h"

#include "Viewport.h"
#include "GfxData.h"
#include "Font.h"

#include "Unit.h"

#include "LBX.h"

using namespace std;
using namespace anims;

#pragma mark SpellEffect

SpellEffect::SpellEffect(LocalPlayer* player, SpriteInfo effectId, Position tile) : DiscreteAnimation(TICKS_PER_FRAME*effectId.count()), effect(effectId), frame(0)
{
  u16 w = effectId.sw(), h = effectId.sh();
  
  coords = Viewport::screenCoordsForTile(player, tile);
  coords.x = coords.x - w/2 + Viewport::tileWidth/2;
  coords.y = coords.y - h/2 + Viewport::tileHeight/2;
  
  if (!coords.isValid())
    force = true;
  else
  {
    force = false;
  }
}

SpellEffect::SpellEffect(LocalPlayer* player, SpriteInfo effectId, int tx, int ty)
: DiscreteAnimation(TICKS_PER_FRAME*effectId.count()), effect(effectId), frame(0)
{
  coords = ScreenCoord(tx-15, ty-25);
  force = false;
}

void SpellEffect::step()
{
  if (!force)
    Gfx::drawAnimated(effect, coords.x, coords.y, 0, TICKS_PER_FRAME);
}

#pragma mark SummonEffect

SummonAnimation::SummonAnimation(WizardID wizard, const SummonSpec* spec)
: ContinuousEndlessAnimation(5000), spec(spec), wizard(GfxData::wizardGfxSpec(wizard).summonPose), creature(GfxData::unitGfxSpec(spec).summonFigure),
  palette(lbx::Repository::spriteFor(GfxData::schoolGfxSpec(spec->as<SummonSpec>()->school).summonPalette)->getPalette())
{

}

void SummonAnimation::step()
{
  /* TODO: find alternative way to manage sprite data with palette */
  lbx::LBXSpriteDataWithPalette summonBg(lbx::Repository::spriteFor(LSI(SPELLSCR,9)), palette);
  lbx::LBXSpriteDataWithPalette summonFlame(lbx::Repository::spriteFor(LSI(SPELLSCR,11)), palette);
  lbx::LBXSpriteDataWithPalette summonFlame2(lbx::Repository::spriteFor(LSI(SPELLSCR,10)), palette);
  
  ScreenCoord base = ScreenCoord(30, 42);
  u16 creatureTopY = 18;
  u16 creatureBottomY = 100;

  float percent = position();
  u16 creatureY = creatureBottomY - (creatureBottomY - creatureTopY)*percent;
  
  Gfx::draw(&summonBg, base.x, base.y);
  
  u16 frame = (Gfx::fticks % 15);
  
  Gfx::drawClipped(wizard, base.x + 8, base.y + 3 + 5, 0, 5, wizard.sw(), wizard.sh()-5);
  Gfx::draw(&summonFlame2, base.x + 55, base.y + 0+97-43, frame);
  Gfx::drawClipped(creature, base.x + 76, base.y + creatureY, 0, 0, creature.sw(), std::min((int)creatureBottomY - creatureY, (int)creature.sh()));
  Gfx::draw(&summonFlame, base.x + 65, base.y + 26+97-43, frame);
  
  //TODO: palette is not the same, single pixels different
  Fonts::drawString(Fonts::format("%s Summoned", spec->productionName().c_str()), FontFaces::Serif::GOLD_SHADOW, base.x + 90, base.y + 116, ALIGN_CENTER);
}

void SummonAnimation::mouseReleased(u16 x, u16 y, MouseButton b) { finish(); }
