//
//  ArmiesView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "CitiesView.h"

#include "Game.h"
#include "City.h"
#include "LocalPlayer.h"

#include "Gfx.h"
#include "Texture.h"
#include "Buttons.h"
#include "Font.h"
#include "Unit.h"
#include "UnitDraw.h"
#include "Viewport.h"

#include "ViewManager.h"

using namespace std;

CitiesView::CitiesView(ViewManager* gvm) : View(gvm), offset(0), city(nullptr)
{
  buttons.resize(BUTTON_COUNT);
  
  buttons[ITEMS] = Button::buildBistate("Items", 273, 163, LSI(ARMYLIST, 3))->setAction([gvm](){ gvm->switchOverview(VIEW_ARMIES_ITEMS); });
  buttons[OK] = Button::buildBistate("Ok", 273, 182, LSI(ARMYLIST, 4))->setAction([gvm](){ gvm->switchView(VIEW_MAIN); });
  // TODO: arrows buttons
  
  auto lambdaScrollDown = [this](){ scrollDown(); };
  auto lambdaScrollUp = [this](){ scrollUp(); };

  
  buttons[PREV1] = Button::buildTristate("Prev1", 60, 26, LSI(ARMYLIST, 1))->setAction(lambdaScrollUp);
  buttons[NEXT1] = Button::buildTristate("Next1", 60, 139, LSI(ARMYLIST, 2))->setAction(lambdaScrollDown);
  buttons[PREV2] = Button::buildTristate("Prev2", 259, 26, LSI(ARMYLIST, 1))->setAction(lambdaScrollUp);
  buttons[NEXT2] = Button::buildTristate("Next1", 250, 139, LSI(ARMYLIST, 2))->setAction(lambdaScrollDown);
}

void CitiesView::updateScrollButtons()
{
  buttons[PREV1]->activateIf(offset > 0);
  buttons[PREV2]->activateIf(offset > 0);
  buttons[NEXT1]->activateIf(offset + 6 + 1 < player->getArmies().size());
  buttons[NEXT2]->activateIf(offset + 6 + 1 < player->getArmies().size());
}

void CitiesView::draw()
{
  Gfx::draw(LSI(ARMYLIST, 0), 0, 0);
  
  /* fill minimap background */
  Gfx::fillRect(86, 164, 48, 32, Gfx::mainPalette->get(1));
  
  if (city)
  {
    // draw minimap
    // TODO: size: 48, 32 but in original it's 48,33
    // offset 164, 86
    const Position& apos = city->getPosition();
    Viewport::drawMicroMap(player, 86, 164, 48, 32, apos.x, apos.y, apos.plane);
    if (Gfx::fticks%4 == 0)
      Gfx::drawPixel({255,255,255}, 86 + 48/2, 164 + 32/2);
    
    // draw city? name
    // TODO: space dello SmallCompactFont è 1 in meno di quanto è nel gioco originale
    Fonts::drawString(/*unit->name().toUpper()*/"LIZARDMAN SWORDSMEN", 187, 160, ALIGN_CENTER);
    
  }
}

bool CitiesView::mouseMoved(u16 x, u16 y, MouseButton b)
{
  return true;
}
