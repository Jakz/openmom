//
//  AlchemyView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "AlchemyView.h"

#include "Game.h"
#include "LocalPlayer.h"

#include "Gfx.h"
#include "Texture.h"
#include "Buttons.h"
#include "Font.h"

#include "ViewManager.h"

AlchemyView::AlchemyView(ViewManager* gvm) : View(gvm), inverted(false), percent(0.0f), amount(0)
{
  buttons.resize(BUTTON_COUNT);
  
  buttons[CANCEL] = Button::buildBistate("Cancel", 95, 105, LSI(MAGIC, 53))->setAction([gvm](){ gvm->closeOverview(); });
  buttons[OK] = Button::buildBistate("Ok", 178, 105, LSI(MAGIC, 54))-> setAction([this,gvm](){
    g->alchemy(player, amount, !inverted);
    gvm->closeOverview();
  });
  
  auto invertAction = [this](){
    buttons[SWITCH]->showIf(inverted);
    buttons[SWITCH2]->showIf(!inverted);
    
    amount = static_cast<s32>(percent * (inverted ? player->totalGoldPool() : player->totalManaPool()));
    inverted = !inverted;
  };
  
  buttons[SWITCH] = Button::buildBistate("Switch1", 148, 105, LSI(MAGIC, 55))->setAction(invertAction);
  buttons[SWITCH2] = Button::buildBistate("Switch2", 148, 105, LSI(MAGIC, 56))->setAction(invertAction)->hide();
}

void AlchemyView::draw()
{
  // draw backdrop
  Gfx::drawClipped(TSI(ALCHEMY_BACKDROP,0,0), 77, 66, 0, 0, 166, 67);
  
  // if conversion is inverted power -> gold switch label
  if (inverted)
    Gfx::drawClipped(TSI(ALCHEMY_BACKDROP,0,0), 77, 76, 0, 67, 166, 10);
  
  // draw bar on amount bar
  // TODO: should be animated
  Gfx::drawClipped(TSI(ALCHEMY_BACKDROP,0,0),133, 91,  7, 77,   (s32)(percent*(49))+4, 7);
  // draw symbol on amount bar
  Gfx::drawClipped(TSI(ALCHEMY_BACKDROP,0,0),133+(s32)(percent*(49)),91,  0, 77,  7, 7);
  
  int flx = 95, frx = 197, fy = 90;
  int gamount = 0, mamount = 0;
  
  // TODO: manage ALCHEMIST trait for wizards
  if (!inverted)
  {
    gamount = amount;
    mamount = amount/2;
  }
  else
  {
    gamount = amount/2;
    mamount = amount;
  }
  
  // TODO: font is not exactly the same, and space is removed otherwise it would go over the box
  Fonts::drawString(Fonts::format("%dGP",gamount), FontFaces::Small::YELLOW, flx, fy, ALIGN_LEFT);
  Fonts::drawString(Fonts::format("%dMP",mamount), FontFaces::Small::YELLOW, frx, fy, ALIGN_LEFT);
  
}

void AlchemyView::mouseDragged(u16 x, u16 y, MouseButton b)
{
  s32 sx = 133, sy = 91, w = 57, h = 7;
  s32 dx = x - sx;
  
  if (dx >= 0 && dx < w && y >= sy && y < sy + h)
  {
    if (dx < 3)
      percent = 0;
    else if (dx >= 53)
      percent = 1;
    else
      percent = (dx-3) / (float)(w-7);
    
    if (!inverted)
      amount = (s32)(player->totalGoldPool()*percent);
    else
      amount = (s32)(player->totalManaPool()*percent);
  }
}
