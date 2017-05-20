//
//  OutpostView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "LoadView.h"

#include "Gfx.h"
#include "Texture.h"
#include "Font.h"
#include "Localization.h"
#include "GfxData.h"
#include "Buttons.h"
#include "LBX.h"

#include "ViewManager.h"

enum lbx_entries
{
  background = LBXI(LOAD, 0),
  
};

LoadView::LoadView(ViewManager* gvm) : View(gvm)
{
  const auto* palette = lbx::Repository::spriteFor(background)->palette;
  
  buttons.resize(bts::COUNT);
  
  buttons[LOAD] = Button::buildOffsetted("load", 83, 171, LSI(LOAD,1), LSI(LOAD,5));
  buttons[LOAD]->setPalette(palette);
  
  buttons[QUIT] = Button::buildOffsetted("quit", 43, 171, LSI(LOAD,2), LSI(LOAD,6));
  buttons[QUIT]->setPalette(palette);
  
  buttons[SAVE] = Button::buildOffsetted("save", 122, 171, LSI(LOAD,3), LSI(LOAD,7));
  buttons[SAVE]->setPalette(palette);
  
  buttons[OK] = Button::buildOffsetted("ok", 231, 171, LSI(LOAD,4), LSI(LOAD,8));
  buttons[OK]->setPalette(palette);
  buttons[OK]->setAction([this](){ this->gvm->switchView(VIEW_MAIN); });
  
  buttons[SETTINGS] = Button::buildOffsetted("settings", 172, 171, LSI(LOAD,12));
  buttons[SETTINGS]->setPalette(palette);
  buttons[SETTINGS]->setAction([this](){ this->gvm->switchView(VIEW_OPTIONS); });

}

void LoadView::draw()
{
  Gfx::draw(background, 0, 0);
  Gfx::fillRect(43, 171, 117, 13, Gfx::mainPaletteOpaque->get(0));
  Gfx::fillRect(172, 171, 99, 13, Gfx::mainPaletteOpaque->get(0));

}

bool LoadView::mouseReleased(u16 x, u16 y, MouseButton b)
{
  return true;
}
