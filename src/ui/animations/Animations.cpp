//
//  Animations.cpp
//  OpenMoM
//
//  Created by Jack on 7/19/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "Animations.h"

#include "Gfx.h"
#include "Viewport.h"
#include "UnitDraw.h"
#include "LocalPlayer.h"

using namespace std;
using namespace anims;

const Color anims::Blink::SCHOOL_COLORS[] = {{0,0,0}, {255,0,0}, {0,0,0}, {255,255,255}, {0,255,0}, {0,0,255}};


void Blink::step()
{
  u8 alpha = 0;
  float pos = position();
  

  
  alpha = (u8) (pos < 0.5f ? pos*2*maxAlpha : (1.0f - ((pos - 0.5f)*2))*maxAlpha);
  
  printf("%f POS %d\n", pos, alpha);
  
  Gfx::alphaBlend(rect, (alpha << 24) | (color & 0x00FFFFFF));
}



UnitMovement::UnitMovement(LocalPlayer* player, const Army* army, const decltype(moves)& moves) : ContinuousAnimation(200), player(player), army(army), moves(moves)
{
  Point s = Viewport::screenCoordsForTile(player, moves.front());
  Point d = Viewport::screenCoordsForTile(player, *::next(moves.begin()));
  
  tx = d.x - s.x;
  ty = d.y - s.y;
  sx = s.x;
  sy = s.y;
  
  player->setDrawSelectedArmy(false);
}

void UnitMovement::step()
{
  float d = position();
  UnitDraw::drawStatic(army, sx + d*tx, sy + d*ty, true);
}

bool UnitMovement::hasFinished()
{
  bool h = ContinuousAnimation::hasFinished();
  
  if (h)
  {
    moves.pop_front();
    
    if (moves.size() < 2)
      player->setDrawSelectedArmy(true);
    else
      player->push(new UnitMovement(player,army,moves));
  }
  
  return h;
}


#include "ui/views/CombatView.h"

constexpr float PROJECTILE_SPEED = 0.01f;

CombatProjectile::CombatProjectile(coord_t from, coord_t to, SpriteInfo effect, size_t count) : ContinuousAnimation(0),
from(from), to(to), effect(effect), count(count)
{
  start = CombatView::coordsForTile(from.x, from.y);
  end = CombatView::coordsForTile(to.x, to.y);

  
  float distance = Util::distance(start.x, end.x, start.y, end.y);
  setDuration(distance * PROJECTILE_SPEED);
  //TODO: not nice to call setDuration but computations are long
}

void CombatProjectile::step()
{
  float percent = position();
  auto gfx = effect.relative(spriteDeltaForFacing(facing));
  
  //TODO TODO
  /* last 10% of the time is used to show frame on target tile, this requires different approach to animation */
}
