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

action_t View::buildSwitchViewAction(View* view, ViewID newView)
{
  return [&](){gvm->switchView(newView);};
}

action_t View::buildSwitchOverviewAction(View* view, ViewID newView)
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
  for (const auto& c : areas)
    if (c && c->isActive())
      c->draw();
}

bool View::doMouseReleased(u16 x, u16 y, MouseButton b)
{
  bool handled = false;
  
  if (curButton)
  {
    bool inside = curButton->isInside(x,y);
    if (inside && b == BUTTON_LEFT)
    {
      handled = true;
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
  
  for (const auto& c : areas)
  {
    if (c && c->isActive() && c->isInside(x,y) && c->getAction())
    {
      if (c->getAction())
        if (c->getAction()(x,y,b))
        {
          handled = true;
          break;
        }
    }
  }
  
  if (!handled)
    return mouseReleased(x,y,b);
  else
    return true;
}

bool View::doMousePressed(u16 x, u16 y, MouseButton b)
{
  if (!curButton)
  {
    for (const auto bt : buttons)
    {
      if (bt && bt->isActive() && bt->isInside(x,y) && b == BUTTON_LEFT)
      {
        bt->press();
        curButton = bt;
        return true;
      }
    }
  }
  
  return mousePressed(x,y,b);
}

bool View::doMouseDragged(u16 x, u16 y, MouseButton b)
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
      return true;
    }
  }
  
  return mouseDragged(x,y,b);
}

bool View::doMouseMoved(u16 x, u16 y, MouseButton b)
{
  if (currentHoverButton && !currentHoverButton->isInside(x, y))
  {
    currentHoverButton->mouseExited();
    currentHoverButton = nullptr;
  }

  for (const auto bt : buttons)
  {
    if (bt && bt != currentHoverButton && bt->isActive() && bt->isInside(x, y))
    {
      currentHoverButton = bt;
      currentHoverButton->mouseEntered();
      return true;
    }
  }
  
  for (const auto& bt : areas)
  {
    if (bt && bt.get() != currentHoverButton && bt->isActive() && bt->isInside(x, y))
    {
      currentHoverButton = bt.get();
      currentHoverButton->mouseEntered();
      return true;
    }
  }
  
  return mouseMoved(x, y, b);
}

bool View::doMouseWheel(s16 dx, s16 dy, u16 d)
{
  return mouseWheel(dx, dy, d);
}
