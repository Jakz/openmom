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

void ArmyView::activate()
{
  if (!propPalette)
  {
    const Palette* basePalette = LSI(UNITVIEW, 28).palette();
    // TODO: color at index 82 must become transparent
    propPalette = std::unique_ptr<const Palette>(new DerivedPalette(basePalette, 255, 1, Color(0,0,0,0)));
  }
}

void ArmyView::draw()
{
  s16 i = army->size();
  
  s16 h = 24 + i*19;
  s16 o = (200-(h+5))/2;
  
  const auto& propGfx = GfxData::propGfx();
  
  Gfx::drawClipped(LSI(UNITVIEW, 28), 41, o, 0, 0, 238, h);
  Gfx::draw(LSI(UNITVIEW, 29), 41, o + h);
  
  const int PROP_BOX_DELTA = 20;
  
  int c = 24;
  for (int j = 0; j < i; ++j)
  {
    const int y = c + o;
    const Unit* unit = army->get(j);
    
    UnitDraw::drawStatic(unit, 49, y, true, false);
    Fonts::drawString(unit->name(), FontFaces::Medium::TEAL_STROKE, 72, y + 4, ALIGN_LEFT);
    
    /* TODO: should they be spec values of computed values (with bonuses and such) */
    const MeleeInfo melee = unit->getMeleeInfo();
    const RangedInfo ranged = unit->getRangedInfo();
    const prop_value shields = unit->getProperty(Property::SHIELDS);
    const prop_value hits = unit->getProperty(Property::HIT_POINTS);
    const MovementInfo movement = unit->getMovementInfo();
    
    if (melee.strength > 0)
    {
      const int BASE = 182 + PROP_BOX_DELTA*0;
      Gfx::draw(propGfx[melee.type].blueBoxed, propPalette.get(), BASE, y + 5);
      Fonts::drawString(std::to_string(melee.strength), FontFaces::Small::TEAL_DARK, BASE, y + 6, ALIGN_RIGHT);
    }
    
    /* TODO: manage schools according to option */
    if (ranged.isPresent())
    {
      const int BASE = 182 + PROP_BOX_DELTA*1;
      Gfx::draw(propGfx[ranged.type].blueBoxed, propPalette.get(), BASE, y + 5);
      Fonts::drawString(std::to_string(melee.strength), FontFaces::Small::TEAL_DARK, BASE, y + 6, ALIGN_RIGHT);
    }
    
    {
      const int BASE = 182 + PROP_BOX_DELTA*2;
      Gfx::draw(propGfx[Property::SHIELDS].blueBoxed, propPalette.get(), BASE, y + 5);
      Fonts::drawString(std::to_string(shields), FontFaces::Small::TEAL_DARK, BASE, y + 6, ALIGN_RIGHT);
    }
    
    {
      const int BASE = 182 + PROP_BOX_DELTA*3;
      Gfx::draw(propGfx[Property::HIT_POINTS].blueBoxed, propPalette.get(), BASE, y + 5);
      Fonts::drawString(std::to_string(hits), FontFaces::Small::TEAL_DARK, BASE, y + 6, ALIGN_RIGHT);
    }
    
    {
      const int BASE = 182 + PROP_BOX_DELTA*4;
      Gfx::draw(propGfx[movement.type].blueBoxed, propPalette.get(), BASE, y + 5);
      Fonts::drawString(std::to_string(movement.moves), FontFaces::Small::TEAL_DARK, BASE, y + 6, ALIGN_RIGHT);
    }
    
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
