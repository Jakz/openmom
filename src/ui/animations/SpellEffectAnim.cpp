#include "SpellEffectAnim.h"

#include "Viewport.h"

using namespace std;
using namespace anims;

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

SpellEffect::SpellEffect(LocalPlayer* player, SpriteInfo effectId, int tx, int ty) : DiscreteAnimation(TICKS_PER_FRAME*effectId.count()), effect(effectId), frame(0)
{
  coords = ScreenCoord(tx-15, ty-25);
  force = false;
}

void SpellEffect::step()
{
  if (!force)
    Gfx::drawAnimated(effect, coords.x, coords.y, 0, TICKS_PER_FRAME);
}
