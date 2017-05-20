//
//  OutpostView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "OutpostView.h"

#include "Gfx.h"
#include "Texture.h"
#include "Font.h"
#include "Localization.h"
#include "GfxData.h"

#include "ViewManager.h"

#include "City.h"

OutpostView::OutpostView(ViewManager* gvm) : View(gvm)
{
  
}

void OutpostView::draw()
{
  Gfx::draw(LSI(BACKGRND, 32), 30, 50); // bg
  Fonts::drawString(i18n::s(city->race->ident).name, FontFaces::Small::YELLOW, 35, 72, ALIGN_LEFT);
  
  auto& houseSpec = GfxData::raceHouseGfxSpec(city->race->houseType);

  for (int i = 0; i < 10; ++i)
  {
    Gfx::draw(i*100 <= city->getPopulation() ? houseSpec.outpostFull : houseSpec.outpostEmpty, 38+14*i, 81);
  }
  
  //TODO: fix with LBX management
  /*Gfx::drawClipped(TSI(CITY_BACKGROUND,0,0), BX, BY, 100 - 35, 0, 72, 66);
  
  // draw correct far sight according to city features: grasslands, hills, mountain
  if (city->hasPlacement(CITY_BY_MOUNTAIN))
    Gfx::drawClipped(TSI(CITY_LANDSCAPE,0,0), BX, BY, 100 - 35, 19*(2+3*city->getPosition().plane), 72, 19);
  else if (city->hasPlacement(CITY_BY_HILL))
    Gfx::drawClipped(TSI(CITY_LANDSCAPE,0,0), BX, BY, 100 - 35, 19*(1+3*city->getPosition().plane), 72, 19);
  else
    Gfx::drawClipped(TSI(CITY_LANDSCAPE,0,0), BX, BY, 100 - 35, 19*(0+3*city->getPosition().plane), 72, 19);
  */
  
  // black bounding box around city scape
  Gfx::drawLine({0,0,0}, BX, BY, BX, BY+66);
  Gfx::drawLine({0,0,0}, BX+71, BY, BX+71, BY+66);
  Gfx::drawLine({0,0,0}, BX, BY, BX+72, BY);
  Gfx::drawLine({0,0,0}, BX, BY+65, BX+72, BY+65);
  
  Gfx::draw(houseSpec.cityScapeHouse, BX + 30, BY + 30);
  
  // TODO: draw units
}

bool OutpostView::mouseReleased(u16 x, u16 y, MouseButton b)
{
  gvm->closeOverview();
  return true;
}
