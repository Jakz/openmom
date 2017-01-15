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
  
  buttons[LOAD] = Button::buildOffsetted("load", 50, 50, LSI(LOAD,1));
  buttons[LOAD]->setPalette(palette);
  
}

void LoadView::draw()
{
  Gfx::draw(background, 0, 0);
}

void LoadView::mouseReleased(u16 x, u16 y, MouseButton b)
{
  
}
