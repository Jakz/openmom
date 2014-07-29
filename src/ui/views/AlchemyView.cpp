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
  
  buttons[CANCEL] = TristateButton::build("Cancel", 95, 105, TextureID::ALCHEMY_BUTTONS, 1)->setAction([gvm](){ gvm->closeOverview(); });
  buttons[OK] = TristateButton::build("Ok", 178, 105, TextureID::ALCHEMY_BUTTONS, 0)-> setAction([this,gvm](){
    g->alchemy(player, amount, !inverted);
    gvm->closeOverview();
  });
  
  auto invertAction = [this](){
    buttons[SWITCH]->showIf(inverted);
    buttons[SWITCH2]->showIf(!inverted);
    
    amount = static_cast<s32>(percent * (inverted ? player->totalGoldPool() : player->totalManaPool()));
    inverted = !inverted;
  };
  
  buttons[SWITCH]->setAction(invertAction);
  buttons[SWITCH2]->setAction(invertAction)->hide();
}

void AlchemyView::draw()
{
  // draw backdrop
  Gfx::drawClipped(TextureID::ALCHEMY_BACKDROP, 77, 66, 0, 0, 166, 67);
  
  // if conversion is inverted power -> gold switch label
  if (inverted)
    Gfx::drawClipped(TextureID::ALCHEMY_BACKDROP, 77, 76, 0, 67, 166, 10);
  
  // draw bar on amount bar
  // TODO: should be animated
  Gfx::drawClipped(TextureID::ALCHEMY_BACKDROP,133, 91,  7, 77,   (s32)(percent*(49))+4, 7);
  // draw symbol on amount bar
  Gfx::drawClipped(TextureID::ALCHEMY_BACKDROP,133+(s32)(percent*(49)),91,  0, 77,  7, 7);
  
  int flx = 96, frx = 198, fy = 91;
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
  Fonts::drawString(Fonts::format("%dGP",gamount), FontFace::YELLOW_SMALL, flx, fy, ALIGN_LEFT);
  Fonts::drawString(Fonts::format("%dMP",mamount), FontFace::YELLOW_SMALL, frx, fy, ALIGN_LEFT);
  
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