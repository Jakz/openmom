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
  static const std::vector<std::string> mainEntries = {
    "Surveyor",
    "Cartographer"
  };
  
  static class MainDelegate : public dialogs::InfoMenu::Delegate
  {
    virtual int buttonCount() override { return static_cast<int>(mainEntries.size()); }
    virtual void buttonClicked(int index) override { }
    virtual void drawButton(int i, bool hoveref, const Rect& bounds) override
    {
      Fonts::drawString(mainEntries[i], FontFaces::Serif::BLACK_INFO_MENU, bounds.origin.x+3, bounds.origin.y+1, ALIGN_LEFT);
      Fonts::drawString(Fonts::format("(F%d)", i+1), FontFaces::Serif::BLACK_INFO_MENU, bounds.origin.x + bounds.size.w - 5, bounds.origin.y+1, ALIGN_RIGHT);

    }
    
  } mainDelegate;
  
  menus.push_back(dialogs::InfoMenu(&mainDelegate, 117));
  menus.back().setPosition(Point(87,22));
}

void InfoMenuView::draw()
{
  for (const auto& menu : menus)
    menu.draw();
}

bool InfoMenuView::mouseMoved(u16 x, u16 y, MouseButton b)
{
  if (!menus.empty())
  {
    dialogs::InfoMenu& menu = menus.back();
    int hovered = menu.hoveredButton(Point(x,y));
    menu.setHovered(hovered);
  }
  
  return true;
}
