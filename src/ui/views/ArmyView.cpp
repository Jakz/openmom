//
//  ArmyView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "ArmyView.h"

#include "Game.h"
#include "Army.h"
#include "LocalPlayer.h"

#include "Gfx.h"
#include "GfxData.h"
#include "Texture.h"
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
  
  s16 h = 24 + i*19;
  s16 o = (200-(h+5))/2;
  
  const Palette* palette = LSI(UNITVIEW, 28).palette();
  
  Gfx::drawClipped(LSI(UNITVIEW, 28), 41, o, 0, 0, 238, h);
  Gfx::draw(LSI(UNITVIEW, 29), 41, o + h);
  
  const auto& propGfx = GfxData::propGfx();
  const int PROP_BOX_DELTA = 20;
  
  int c = 24;
  for (int j = 0; j < i; ++j)
  {
    const int y = c + o;
    UnitDraw::drawStatic(army->get(j), 49, y, true, false);
    Fonts::drawString(army->get(j)->name(), FontFaces::Medium::TEAL_STROKE, 72, y + 4, ALIGN_LEFT);
    
    Gfx::draw(propGfx[MeleeType::NORMAL].blueBoxed, palette, 182, y + 5);
    Gfx::draw(propGfx[Ranged::NATURE].blueBoxed, palette, 182 + PROP_BOX_DELTA, y + 5);
    Gfx::draw(propGfx[Property::SHIELDS].blueBoxed, palette, 182 + PROP_BOX_DELTA*2, y + 5);
    Gfx::draw(propGfx[Property::HIT_POINTS].blueBoxed, palette, 182 + PROP_BOX_DELTA*3, y + 5);
    Gfx::draw(propGfx[MovementBaseType::WALKING].blueBoxed, palette, 182 + PROP_BOX_DELTA*4, y + 5);

    
    c += 16+3;
  }
  //Texture.drawClipped(texture, x, y, fx, fy, w, h)
}

void ArmyView::open(Army* army, bool acceptSpellTarget)
{
  setArmy(army);
  setAcceptSpellTarget(acceptSpellTarget);
  gvm->switchOverview(VIEW_ARMY);
}

void ArmyView::mouseReleased(u16 x, u16 y, MouseButton b)
{
  if (b == BUTTON_RIGHT)
    gvm->closeOverview();
  else if (b == BUTTON_LEFT && !acceptSpellTarget)
    gvm->closeOverview();
}

void ArmyView::clickOnIth(size_t i)
{
  if (acceptSpellTarget)
  {
    g->castSpell(army->get(i), player, false);
    gvm->closeOverview();
  }
}

void ArmyView::setArmy(Army* army)
{
  this->army = army;
  areas.clear();
  
  s16 i = army->size();
  
  s16 h = 24 + i*19;
  s16 o = (200-(h+5))/2;

  for (size_t j = 0; j < army->size(); ++j)
  {
    auto* area = new Clickable(49, 24 + o + 19*j, 225, 17);
    areas.emplace_back(area);
    area->setAction([this,j](){ clickOnIth(j); });
  }
}
