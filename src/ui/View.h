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
#include "Buttons.h"

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
  std::vector<std::unique_ptr<Clickable>> areas;
  
  Button* curButton;
  Clickable* currentHoverButton;
  
  LocalPlayer *player;
  Game *g;
  
  virtual void activate() = 0;
  virtual void deactivate() = 0;
  
  virtual void draw() = 0;
  virtual void drawPost() = 0;
  
  void setPlayer(LocalPlayer* player) { this->player = player; }
  
  Clickable* addArea(Clickable* clickable) { areas.push_back(std::unique_ptr<Clickable>(clickable)); return clickable; }
  Button* addButton(Button* button) { buttons.push_back(button); return button; }
  
  ViewManager *gvm;
  
public:
  View(ViewManager *gvm) : gvm(gvm), curButton(nullptr), currentHoverButton(nullptr) { }
  
  Button* buttonAt(u16 index) { return buttons[index]; }
  const std::unique_ptr<Clickable>& areaAt(u16 index) { return areas[index]; }
  
  void doActivate(LocalPlayer* player);
  void doDeactivate() { deactivate(); }
  
  virtual void doDraw()
  {
    draw();
    drawButtons();
  }
  
  void drawButtons();
  
  bool doMouseReleased(u16 x, u16 y, MouseButton b);
  bool doMousePressed(u16 x, u16 y, MouseButton b);
  bool doMouseDragged(u16 x, u16 y, MouseButton b);
  
  bool doMouseMoved(u16 x, u16 y, MouseButton b);
  bool doMouseClicked(u16 x, u16 y, MouseButton b) { return mouseClicked(x,y,b); }
  
  bool doKeyPressed(KeyboardCode key, KeyboardKey kkey, KeyboardMod mod) { return keyPressed(key,kkey,mod); }
  bool doKeyReleased(KeyboardCode key, KeyboardKey kkey, KeyboardMod mod) { return keyReleased(key,kkey,mod); }
  
  friend class ViewManager;
  
  Action buildSwitchViewAction(View* view, ViewID newView);
  Action buildSwitchOverviewAction(View* view, ViewID newView);
  
};

#endif
