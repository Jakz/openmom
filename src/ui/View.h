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
  virtual void drawPre() { }
  
  void setPlayer(LocalPlayer* player) { this->player = player; }
  
  template<typename T> T* addArea(T* area) { areas.push_back(std::unique_ptr<Clickable>(area)); return area; }
  //Clickable* addArea(Clickable* clickable) { areas.push_back(std::unique_ptr<Clickable>(clickable)); return clickable; }
  Button* addButton(Button* button) { buttons.push_back(button); return button; }
  
  ViewManager *gvm;
  
public:
  View(ViewManager *gvm) : gvm(gvm), curButton(nullptr), currentHoverButton(nullptr) { }
  
  Button* buttonAt(u16 index) { return buttons[index]; }
  const std::unique_ptr<Clickable>& areaAt(u16 index) { return areas[index]; }
  
  void doActivate(LocalPlayer* player);
  virtual void doDeactivate() { deactivate(); }
  
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
  
  bool doMouseWheel(s16 dx, s16 dy, u16 d);
  
  bool doKeyPressed(KeyboardCode key, KeyboardKey kkey, KeyboardMod mod) { return keyPressed(key,kkey,mod); }
  bool doKeyReleased(KeyboardCode key, KeyboardKey kkey, KeyboardMod mod) { return keyReleased(key,kkey,mod); }
  bool doTextInput(sdl_text_input data) { return textInput(data); }

  LocalPlayer* currentPlayer() { return player; }
  Game* game() { return g; }
  
  friend class ViewManager;
  
  action_t buildSwitchViewAction(View* view, ViewID newView);
  action_t buildSwitchOverviewAction(View* view, ViewID newView);
  
};

#include "ui/parts/DrawQueueBasic.h"

using Sprite = BasicSpriteEntry;
using TextLabel = BasicTextEntry;

class ViewWithQueue : public View
{
  using DrawQueue = BasicDrawQueue;
  
protected:
  DrawQueue queue;
  
  void clear() { queue.clear(); }
  void add(DrawQueue::type* entity) { queue.add(entity); }
  
  void sprite(SpriteInfo info, Point position) { add(new Sprite(info, position)); }
  void label(const std::string& text, const FontSpriteSheet* font, Point position, TextAlign align) { add(new TextLabel(text, font, position, align)); }
  void rect(const Rect& rect, Color color) { add(new BasicRectEntry(rect, color)); }
  
public:
  ViewWithQueue(ViewManager* gvm) : View(gvm) { }
  
  void doDraw() override
  {
    queue.draw();
    draw();
    drawButtons();
  }
  
  void doDeactivate() override
  {
    queue.clear();
    View::doDeactivate();
  }
};

#endif
