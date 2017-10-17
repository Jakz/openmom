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

StartView::StartView(ViewManager* gvm) : View(gvm)
{

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
