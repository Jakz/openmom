//
//  InfoMenuView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "InfoMenuView.h"

#include "Gfx.h"
#include "Texture.h"
#include "Buttons.h"
#include "Font.h"

#include "ViewManager.h"

InfoMenuView::InfoMenuView(ViewManager* gvm) : View(gvm)
{

}

void InfoMenuView::activate()
{
  menus.push_back(dialogs::InfoMenu({10,10},100));
}

void InfoMenuView::draw()
{
  for (const auto& menu : menus)
    menu.draw();
}
