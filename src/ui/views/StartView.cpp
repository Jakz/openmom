//
//  ResearchView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "StartView.h"

#include "Gfx.h"
#include "Texture.h"
#include "Buttons.h"
#include "Font.h"

#include "BookView.h"
#include "Spells.h"
#include "LocalPlayer.h"
#include "Game.h"

#include "Localization.h"

#include "ViewManager.h"

#include "gfx/SDLHelper.h" //TODO: for quit

StartView::StartView(ViewManager* gvm) : View(gvm)
{
  buttons.resize(BUTTON_COUNT);
  
  const char* names[] = { "continue", "load game", "new game", "hall of fame", "quit to dos" };
  const int gfxIndices[] = { 1, 5, 4, 2, 3};
  const coord_t x = 123;
  coord_t y = 141;
  
  //TODO: palette should be LSI(MAINSCRN,0) palette
  for (int i = 0; i < BUTTON_COUNT; ++i)
  {
    buttons[i] = Button::buildBistate(names[i], x, y + i*12, LSI(VORTEX, gfxIndices[i]).frame(1), LSI(VORTEX, gfxIndices[i]).frame(0));
    buttons[i]->graphics().palette = LSI(MAINSCRN, 0).palette();
    buttons[i]->graphics().hover = buttons[i]->graphics().pressed;
  }
  
  buttons[BUTTON_EXIT_TO_DOS]->setAction([]() {
    SDL::quit();
  });
}

void StartView::activate()
{
  
}

void StartView::draw()
{
  const SpriteInfo logo = LSI(MAINSCRN, 0);
  const SpriteInfo background = LSI(MAINSCRN, 5);
  
  Gfx::draw(background, logo.palette(), 0, logo.sh());
  Gfx::drawAnimated(logo, 0, 0);
}
