//
//  ResearchView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "AstrologerView.h"

#include "Gfx.h"
#include "Texture.h"
#include "Buttons.h"
#include "Font.h"

#include "BookView.h"
#include "Spells.h"
#include "LocalPlayer.h"
#include "Game.h"
#include "World.h"

#include "Army.h"

#include "GfxData.h"
#include "Localization.h"

#include "ViewManager.h"

AstrologerView::AstrologerView(ViewManager* gvm) : ViewWithQueue(gvm)
{

}

void AstrologerView::activate()
{
  setup();
}

void AstrologerView::draw()
{
  
}

void AstrologerView::setup()
{
  SpriteInfo bg = LSI(RELOAD, 1);
  sprite(bg, Point(WIDTH/2 - bg.sw()/2, HEIGHT/2 - bg.sh()/2));
}

bool AstrologerView::mouseReleased(u16 x, u16 y, MouseButton b)
{
  if (b == MouseButton::BUTTON_LEFT)
    gvm->closeOverview();
  
  return true;
}
