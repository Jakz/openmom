//
//  ArmyView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "ArmyView.h"

#include "game/Game.h"
#include "game/units/Army.h"
#include "LocalPlayer.h"

#include "gfx/Gfx.h"
#include "gfx/Texture.h"
#include "Buttons.h"
#include "Font.h"
#include "UnitDraw.h"

#include "ViewManager.h"

ArmyView::ArmyView(ViewManager* gvm) : View(gvm), acceptSpellTarget(false), army(nullptr)
{
  
}

void ArmyView::draw()
{
  s16 i = army->size();
  
  s16 h = 25 + 8 + i*18 + (i-1)*3;
  s16 o = (200-h)/2;
  
  //Texture.draw(Texture.UNIT_DETAIL_TARGET, 41, 200 - h / 2);
  
  Gfx::drawClipped(TextureID::UNIT_DETAIL_TARGET, 41, o, 0, 0, 238, h - 8);
  Gfx::drawClipped(TextureID::UNIT_DETAIL_TARGET, 41, o + h - 10, 0, 195, 238, 5);
  
  int c = 24;
  for (int j = 0; j < i; ++j)
  {
    UnitDraw::drawStatic(army->get(j), 49, c + o, true, false);
    c += 16+3;
  }
  //Texture.drawClipped(texture, x, y, fx, fy, w, h)
}

void ArmyView::mouseReleased(u16 x, u16 y, MouseButton b)
{
  s16 h = 25 + 8 + army->size()*18 + (army->size()-1)*3;
  s16 o = (200-h)/2;
  
  if (acceptSpellTarget && b == BUTTON_LEFT)
  {
    if (x >= 49 && x < 49 + 238 && y >= 24 + o && y < 24 + army->size()*19 + o)// && (y-24)%19 < 16)
    {
      // TODO: move viewport so that unit is at least -1 from edges
      g->castSpell(army->get((y-24-o)/19), player, false);
      gvm->closeOverview();
    }
  }
  else
    gvm->closeOverview();
}
