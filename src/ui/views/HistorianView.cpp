//
//  ResearchView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "HistorianView.h"

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

HistorianView::HistorianView(ViewManager* gvm) : ViewWithQueue(gvm)
{

}

void HistorianView::activate()
{
  setup();
}

void HistorianView::draw()
{
  
}

void HistorianView::setup()
{
  sprite(LSI(RELOAD, 0), {0, 0});
}

bool HistorianView::mouseReleased(u16 x, u16 y, MouseButton b)
{
  if (b == MouseButton::BUTTON_LEFT)
    gvm->switchView(VIEW_MAIN);
  
  return true;
}
