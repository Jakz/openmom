//
//  View.h
//  OpenMoM
//
//  Created by Jack on 7/19/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _VIEW_H_
#define _VIEW_H_

#include "EventListener.h"

#include <functional>
#include <vector>

typedef std::function<void()> Action;

class Button;
class Clickable;
class LocalPlayer;
class Game;
class ViewManager;

class View : public EventListener
{
  protected:
    std::vector<Button*> buttons;
    std::vector<Clickable*> areas;
  
    Button* curButton;
  
    LocalPlayer *player;
    Game *g;
  
    virtual void activate() = 0;
    virtual void deactivate() = 0;
  
    virtual void draw() = 0;
    virtual void drawPost() = 0;

    ViewManager *gvm;
  
  public:
    View(ViewManager *gvm) : gvm(gvm), curButton(nullptr) { }
  
    Button* buttonAt(u16 index) { return buttons[index]; }
    Clickable* areaAt(u16 index) { return areas[index]; }
  
    void doActivate(LocalPlayer* player);
    void doDeactivate() { deactivate(); }
  
    void doDraw()
    {
      draw();
      drawButtons();
    }
  
    void drawButtons();
  
    void doMouseReleased(u16 x, u16 y, MouseButton b);
    void doMousePressed(u16 x, u16 y, MouseButton b);
    void doMouseDragged(u16 x, u16 y, MouseButton b);
  
    void doMouseMoved(u16 x, u16 y, MouseButton b) { mouseMoved(x,y,b); }
    void doMouseClicked(u16 x, u16 y, MouseButton b) { mouseClicked(x,y,b); }
  
    void doKeyPressed(KeyboardKey key, KeyboardMod mod) { keyPressed(key,mod); }
    void doKeyReleased(KeyboardKey key, KeyboardMod mod) { keyReleased(key,mod); }
  
  friend class ViewManager;
  
  Action buildSwitchViewAction(View* view, ViewID newView);
  Action buildSwitchOverviewAction(View* view, ViewID newView);

};

#endif
