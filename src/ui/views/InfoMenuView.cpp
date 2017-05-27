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

}

void InfoMenuView::draw()
{
  auto menu = dialogs::InfoMenu({10,10},3,100);
  menu.draw();
}
