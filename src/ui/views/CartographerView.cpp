//
//  ResearchView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "CartographerView.h"

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

CartographerView::CartographerView(ViewManager* gvm) : View(gvm)
{
  addArea(new Clickable(264,140,36,30))->setAction([this]() { this->gvm->switchView(VIEW_MAIN); });
  
  addArea(new Clickable(280,180,39,19))->setAction([this]() { /* TODO */ });
}

void CartographerView::activate()
{
  this->plane = Plane::ARCANUS;
}

void CartographerView::draw()
{
  Gfx::draw(LSI(RELOAD, 2), 0, 0);
  
  
  
  Fonts::drawString("Arcanus Plane", FontFaces::Huge::BROWN_CARTOGRAPHER, Point(WIDTH/2 - 1, 11), ALIGN_CENTER);
}
