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

constexpr const Color Blink::SCHOOL_COLORS[];

void Blink::step()
{
  u8 alpha = 0;
  float pos = position();
  

  
  alpha = (u8) (pos < 0.5f ? pos*2*maxAlpha : (1.0f - ((pos - 0.5f)*2))*maxAlpha);
  
  printf("%f POS %d\n", pos, alpha);
  
  Gfx::alphaBlend(rect, (alpha << 24) | (color & 0x00FFFFFF));
}



UnitMovement::UnitMovement(LocalPlayer* player, const Army* army, const list<pathfind::PathTileInfo>& moves) : ContinuousAnimation(100), player(player), army(army), moves(moves)
{
  Position s = Viewport::tileCoords(player, moves.front().x, moves.front().y);
  Position d = Viewport::tileCoords(player, ::next(moves.begin())->x, ::next(moves.begin())->y);
  
  tx = d.x - s.x;
  ty = d.y - s.y;
  sx = s.x;
  sy = s.y;
  
  player->setDrawSelectedArmy(false);
}

void UnitMovement::step()
{
  float d = position();
  UnitDraw::drawStatic(army, sx + d*tx, sy + d*ty);
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
