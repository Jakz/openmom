//
//  View.cpp
//  OpenMoM
//
//  Created by Jack on 7/19/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "View.h"

#include "Buttons.h"
#include "LocalPlayer.h"

#include "ViewManager.h"

Action View::buildSwitchViewAction(View* view, ViewID newView)
{
  return [&](){gvm->switchView(newView);};
}

Action View::buildSwitchOverviewAction(View* view, ViewID newView)
{
  return [&](){gvm->switchOverview(newView);};
}

void View::doActivate(LocalPlayer* player)
{
  this->player = player;
  this->g = player->game();
  activate();
}

void View::drawButtons()
{
  for (const auto b : buttons)
    if (b && b->isVisible())
      b->draw();
  for (const auto c : areas)
    if (c && c->isActive())
      c->draw();
}

void View::doMouseReleased(u16 x, u16 y, MouseButton b)
{
  if (curButton)
  {
    bool inside = curButton->isInside(x,y);
    if (inside && b == BUTTON_LEFT)
    {
      curButton->click();
      curButton->release();
      curButton = nullptr;
    }
    else if (!inside)
    {
      curButton->release();
      curButton = nullptr;
    }
  }
  
  for (const auto c : areas)
  {
    if (c && c->isActive() && c->isCorrectButton(b) && c->isInside(x,y))
      c->getAction()();
  }
  
  mouseReleased(x,y,b);
}

void View::doMousePressed(u16 x, u16 y, MouseButton b)
{
  if (!curButton)
  {
    for (const auto bt : buttons)
    {
      if (bt && bt->isActive() && bt->isInside(x,y) && b == BUTTON_LEFT)
      {
        bt->press();
        curButton = bt;
        return;
      }
    }
  }
  
  mousePressed(x,y,b);
}

void View::doMouseDragged(u16 x, u16 y, MouseButton b)
{
  if (curButton)
  {
    bool inside = curButton->isInside(x,y);
    if (!inside && b == BUTTON_LEFT)
    {
      curButton->release();
      curButton = nullptr;
    }
  }
  
  for (const auto bt : buttons)
  {
    if (bt && bt->isActive() && bt->isInside(x,y) && b == BUTTON_LEFT)
    {
      bt->press();
      curButton = bt;
      return;
    }
  }
  
  mouseDragged(x,y,b);
}