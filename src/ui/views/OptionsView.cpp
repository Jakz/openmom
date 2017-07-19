//
//  OutpostView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "OptionsView.h"

#include "Gfx.h"
#include "Texture.h"
#include "Font.h"
#include "Localization.h"
#include "GfxData.h"
#include "LBXRepository.h"
#include "Buttons.h"
#include "ViewManager.h"


OptionsView::OptionsView(ViewManager* gvm) : View(gvm)
{
  const auto* palette = lbx::Repository::spriteFor(LSI(LOAD, 0))->palette;

  buttons.resize(bts::COUNT);
  buttons[OK] = Button::buildOffsetted("ok", 266, 176, LSI(LOAD,4));
  buttons[OK]->setPalette(palette);
  buttons[OK]->setAction([this](){ this->gvm->switchView(VIEW_LOAD); });
}

void OptionsView::draw()
{
  const auto* palette = lbx::Repository::spriteFor(LSI(LOAD, 0))->palette;
  Gfx::draw(LSI(LOAD,11), palette, 0, 0);
}

bool OptionsView::mouseReleased(u16 x, u16 y, MouseButton b)
{
  return true;
}
