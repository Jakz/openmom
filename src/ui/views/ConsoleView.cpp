//
//  ItemCraftView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "ConsoleView.h"

#include "Font.h"
#include "Buttons.h"
#include "Texture.h"
#include "Gfx.h"
#include "GfxData.h"
#include "ColorMap.h"

#include "ViewManager.h"
#include "Util.h"

ConsoleView::ConsoleView(ViewManager* gvm) : View(gvm)
{
  
}

void ConsoleView::executeCommand(std::string cmd)
{
  LOGD("[console] executing '%s'", cmd.c_str());
}

void ConsoleView::draw()
{
  Fonts::drawString(line, FontFaces::Small::WHITE, 10, 10, ALIGN_LEFT);
}

bool ConsoleView::keyPressed(KeyboardCode key, KeyboardKey kkey, KeyboardMod mod)
{
  if (key == SDL_SCANCODE_BACKSPACE && line.length() > 1)
    line.pop_back();
  else
  {
    if (isalpha(kkey) || isdigit(kkey) || isspace(kkey))
      line += kkey;
  }
  
  return true;
}

bool ConsoleView::keyReleased(KeyboardCode key, KeyboardKey kkey, KeyboardMod mod)
{
  if (key == SDL_SCANCODE_RETURN)
  {
    if (line.length() > 1)
    {
      executeCommand(line.substr(1));
      line = ">";
    }
    else
      gvm->closeOverview();
  }
  
  return true;
}

void ConsoleView::activate()
{
  line = ">";
}
