//
//  Buttons.h
//  OpenMoM
//
//  Created by Jack on 7/19/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _BUTTONS_H_
#define _BUTTONS_H_

#include "SDL.h"
#include "common/Common.h"
#include "Texture.h"

#include <string>
#include <functional>
#include <vector>
#include <memory>

class FontSpriteSheet;

using clickable_action_t = std::function<bool(coord_t, coord_t, MouseButton)>;

class Clickable
{
protected:
  coord_t x, y, w, h;
  clickable_action_t action;
  action_t onEnter;
  action_t onExit;
  
  bool active;
  bool hover;
  
public:
  Clickable(u16 x, u16 y, u16 w, u16 h) :
    x(x), y(y), w(w), h(h), active(true), hover(false),
    onEnter([](){}), onExit([](){})
  { }
  
  void setXY(coord_t x, coord_t y) { this->x = x; this->y = y; }
  
  //TODO: removed <= in favor of < for rect check, this was necessary because otherwise ClickableGrid ad a dummy 1 pixel wide cell over the bounds
  bool isInside(coord_t x, coord_t y) const { return x >= this->x && x < this->x+w && y >= this->y && y < this->y+h; }
  virtual bool isActive() const { return active; }
  inline void activate() { active = true; }
  inline void deactivate() { active = false; }
  inline void activateIf(bool condition) { active = condition; }
  virtual void setPosition(u16 x, u16 y) { this->x = x; this->y = y; }
  
  Clickable* setAction(clickable_action_t action) { this->action = action; return this; }
  Clickable* setAction(action_t action)
  {
    this->action = [action](coord_t, coord_t, MouseButton b) {
      if (b == MouseButton::BUTTON_LEFT)
      {
        action();
        return true;
      }
      return false;
    };
    return this;
  }

  inline const clickable_action_t& getAction() const { return action; }
  
  void setOnEnterAction(action_t action) { this->onEnter = action; }
  void setOnExitAction(action_t action) { this->onExit = action; }
  
  void mouseEntered()
  {
    hover = true;
    onEnter();
  }
  
  void mouseExited()
  {
    hover = false;
    onExit();
  }

  virtual void draw() const;
};

class ClickableGrid : public Clickable
{
public:
  struct Cell { coord_t x, y; };

  using action_t = std::function<bool(coord_t, coord_t, MouseButton)>;
  
private:
  coord_t cw, ch;
  coord_t mx, my;
  size_t rows, cols;
  action_t cellAction;
  
public:
  ClickableGrid() : ClickableGrid(0,0,1,1,0,0) { }
  ClickableGrid(coord_t x, coord_t y, coord_t cw, coord_t ch, size_t rows, size_t cols, coord_t mx, coord_t my) :
    ClickableGrid(x, y, cw, ch, rows, cols) { setMargin(mx, my); }
  ClickableGrid(coord_t x, coord_t y, coord_t cw, coord_t ch, size_t rows, size_t cols) :
    Clickable(x, y, cw*cols, ch*rows),
    cw(cw), ch(ch), mx(0), my(0), rows(rows), cols(cols), cellAction([](coord_t, coord_t, MouseButton) { return false; })
  { 
    Clickable::setAction([this](coord_t x, coord_t y, MouseButton bt) {
      const auto cell = getCell(x, y);
      if (cell.x >= 0)
        return cellAction(cell.x, cell.y, bt);
      
      return false;
    });
  }

  void setMargin(coord_t x, coord_t y)
  {
    mx = x; my = y;
    w = (cw+mx)*cols; h = (ch+my)*rows;
  }
  
  ClickableGrid* setCellAction(action_t action) { this->cellAction = action; return this; }
  ClickableGrid* setCellAction(std::function<void(coord_t,coord_t)> action)
  {
    setCellAction([action](coord_t x, coord_t y, MouseButton bt) {
      if (bt == MouseButton::BUTTON_LEFT)
      {
        action(x, y);
        return true;
      }
      
      return false;
    });
    
    return this;
  }

  
  void forEachCell(std::function<void(coord_t,coord_t,coord_t,coord_t)> lambda) const
  {
    for (int j = 0; j < cols; ++j)
      for (int i = 0; i < rows; ++i)
        lambda(x + j*(cw+mx), y + i*(ch+my), cw, ch);
  }
  
  inline Cell getCell(coord_t x, coord_t y) const
  {
    coord_t dx = x - this->x, dy = y - this->y;
    coord_t cx = dx / (this->cw + mx);
    coord_t cy = dy / (this->ch + my);
    
    if (dx % (this->cw + mx) < cw && dy % (this->ch + my) < ch)
      return { cx, cy };
    else
      return { -1, -1 };
  }

  void draw() const override;
};

template<typename T>
class clickable_list
{
private:
  std::vector<std::unique_ptr<T>> areas;
  
public:
  clickable_list() { }
  clickable_list(size_t r) { areas.resize(r); }
  
  size_t size() const { return areas.size(); }
  void emplace_back(T* area) { areas.emplace_back(area); }
  void clear() { areas.clear(); }
  
  void draw() const { for (const auto& area : areas) area->draw(); }
  bool handleEvent(u16 x, u16 y, MouseButton b)
  {
    for (const auto& area : areas)
      if (area->isActive() && area->isInside(x,y))
      {
        return area->getAction()(x,y,b);
      }
    
    return false;
  }
  
  using iterator = typename decltype(areas)::iterator;
  using const_iterator = typename decltype(areas)::const_iterator;
  
  const_iterator begin() const { return areas.begin(); }
  const_iterator end() const { return areas.end(); }
};

struct ButtonGfx
{
  const Palette* palette;
  optional<SpriteInfo> normal;
  optional<SpriteInfo> pressed;
  optional<SpriteInfo> inactive;
  optional<SpriteInfo> hover;
  bool shouldOffsetNormal;
  
  void draw(u16 x, u16 y, bool isActive, bool isPressed, bool isHover) const;
  
  ButtonGfx() : palette(nullptr) { }
  ButtonGfx(SpriteInfo normal, bool shouldOffsetNormal = false) : normal(normal), palette(nullptr), shouldOffsetNormal(shouldOffsetNormal) { }
  ButtonGfx(SpriteInfo normal, SpriteInfo pressed) : normal(normal), pressed(pressed), palette(nullptr) { }
  ButtonGfx(SpriteInfo normal, SpriteInfo pressed, SpriteInfo inactive) : normal(normal), pressed(pressed), inactive(inactive), palette(nullptr) { }
};

struct TextInfo
{
  std::string label;
  const FontSpriteSheet* font;
  Point position;
  
  TextInfo() = default;
  TextInfo(const std::string& label, const FontSpriteSheet* font) : label(label), font(font) { }
  TextInfo(const std::string& label, Point position) : label(label), font(nullptr), position(position) { }
  TextInfo(const std::string& label, const FontSpriteSheet* font, Point position) : label(label), font(font), position(position) { }

};

class Button : public Clickable
{
protected:
  const std::string name;
  bool pressed, visible;
  
  ButtonGfx gfx;
  optional<TextInfo> labelGfx;
  uintptr_t data;
  
public:
  Button(const std::string& name, u16 x, u16 y, u16 w, u16 h) : Clickable(x, y, w, h), name(name), pressed(false), visible(true) { }
  Button(const std::string& name, u16 x, u16 y, SpriteInfo normal, bool shouldOffsetNormal) : Clickable(x, y, normal.sw(), normal.sh()), name(name), pressed(false), visible(true), gfx(normal, shouldOffsetNormal) { }
  Button(const std::string& name, u16 x, u16 y, SpriteInfo normal, SpriteInfo pressed) : Clickable(x, y, normal.sw(), normal.sh()), name(name), pressed(false), visible(true), gfx(normal, pressed) { }
  Button(const std::string& name, u16 x, u16 y, SpriteInfo normal, SpriteInfo pressed, SpriteInfo inactive) : Clickable(x, y, normal.sw(), normal.sh()), name(name), pressed(false), visible(true), gfx(normal, pressed, inactive) { }

  Button* setAction(action_t action) { Clickable::setAction(action); return this; }
  
  ButtonGfx& graphics() { return gfx; }

  virtual void setTextInfo(const TextInfo& info);
  virtual void setLabel(const std::string& string);
  
  void draw() const override;
  void setPosition(u16 x, u16 y) override;
  
  void setPalette(const Palette* palette) { gfx.palette = palette; }
  
  virtual void click() { getAction()(0, 0, MouseButton::BUTTON_LEFT); }
  
  inline void showIf(bool condition) { visible = condition;}
  inline Button* hide() { visible = false; return this; }
  inline Button* show() { visible = true; return this; }
  inline bool isVisible() const { return visible;}
  
  bool isActive() const override { return active && visible; }

  template<typename T> T* as() { return static_cast<T*>(this); }

  inline void press() { pressed = true;}
  inline void release() { pressed = false; }
  
  template<typename T, typename std::enable_if<std::is_integral<T>::value && sizeof(T) <= sizeof(uintptr_t), int>::type = 0> inline void setData(T data) { this->data = data; }
  template<typename T, typename std::enable_if<std::is_integral<T>::value && sizeof(T) <= sizeof(uintptr_t), int>::type = 0> inline void getData() const { return static_cast<T>(data); }
  
  static Button* buildSimple(const std::string& name, u16 x, u16 y, SpriteInfo normal) { return new Button(name, x, y, normal, false); }
  
  static Button* buildPressedOnly(const std::string& name, u16 x, u16 y, SpriteInfo pressed) {
    auto* bt = new Button(name, x, y, pressed.sw(), pressed.sh());
    bt->gfx.pressed = pressed;
    return bt;
  }
  
  static Button* buildOffsetted(const std::string& name, u16 x, u16 y, SpriteInfo normal) { return new Button(name, x, y, normal, true); }
  static Button* buildOffsetted(const std::string& name, u16 x, u16 y, SpriteInfo normal, SpriteInfo inactive) { auto* bt = new Button(name, x, y, normal, true); bt->gfx.inactive = inactive; return bt; }
  
  static Button* buildBistate(const std::string& name, u16 x, u16 y, SpriteInfo normal, SpriteInfo pressed) { return new Button(name, x, y, normal, pressed); }
  static Button* buildBistate(const std::string& name, u16 x, u16 y, SpriteInfo normal) { return new Button(name, x, y, normal, normal.frame(1)); }
  static Button* buildBistate(const std::string& name, u16 x, u16 y, SpriteInfo normal, const std::string& label, const FontSpriteSheet* font) {
    auto* bt = new Button(name, x, y, normal, normal.frame(1));
    bt->setTextInfo(TextInfo(label, font));
    return bt;
  }

  
  static Button* buildTristate(const std::string& name, u16 x, u16 y, SpriteInfo normal, SpriteInfo pressed, SpriteInfo inactive) { return new Button(name, x, y, normal, pressed, inactive); }
  static Button* buildTristate(const std::string& name, u16 x, u16 y, SpriteInfo normal, SpriteInfo inactive) { return new Button(name, x, y, normal, normal.frame(1), inactive); }
  static Button* buildTristate(const std::string& name, u16 x, u16 y, SpriteInfo normal) { return new Button(name, x, y, normal, normal.frame(1), normal); }
  
};

class TwoPartButton : public Button
{
public:
  struct Clip { u16 start, length; };

private:
  ButtonGfx gfx2;
  Clip clip1, clip2;
  
public:
  TwoPartButton(const std::string& name, u16 x, u16 y, SpriteInfo normal1, SpriteInfo normal2, SpriteInfo pressed1, SpriteInfo pressed2, Clip clip1, Clip clip2) :
    Button(name, x, y, normal1, pressed1), gfx2(normal2, pressed2), clip1(clip1), clip2(clip2) { }
  
  void draw() const override;
};

template<typename T>
class RadioButtonGroup
{
  using value_type = T*;
  
private:
  bool allowsNoSelection;
  std::vector<T*> buttons;
  T* current;
  std::function<void(T*)> lambda;
  std::function<bool(T*)> canSelectPredicate;
public:
  RadioButtonGroup(bool allowsNoSelection = false)
    : current(nullptr), allowsNoSelection(allowsNoSelection), canSelectPredicate([](T*){return true;}) { }
  
  T* getCurrent() const { return current; }
  void set(u16 index) { current = buttons[index]; }
  void click(T* button)
  {
    if (current != button)
    {
      if (button && !canSelectPredicate(button))
        return;
      
      current = button;
      if (lambda)
        lambda(current);
    }
    else if (allowsNoSelection)
    {
      current = nullptr;
    }
    
    for (const auto& button : buttons)
      button->setToggled(button == current);
  }
  
  void add(T* button)
  {
    buttons.push_back(button);
  }
  
  void setCanSelectPredicate(std::function<bool(T*)> predicate) { this->canSelectPredicate = predicate; }
  void setAction(std::function<void(T*)> lambda) { this->lambda = lambda; }
};

template<typename T>
class RadioButton : public Button
{
  using group_t = RadioButtonGroup<RadioButton<T>>;
private:
  group_t* group;
  
  ButtonGfx toggledGfx;
  bool toggled;
  
  T data;
  
  RadioButton(const std::string name, T data, RadioButtonGroup<RadioButton<T>>* group, u16 x, u16 y, SpriteInfo normal, SpriteInfo pressed, SpriteInfo normalToggled, SpriteInfo pressedToggled)
  : Button(name, x, y, normal, pressed), data(data), toggledGfx(normalToggled, pressedToggled), toggled(false), group(group) { }
  
public:
  void draw() const override;
  
  const T getData() const { return data; }
  
  void click() override { group->click(this); }
  void setToggled(bool t) { this->toggled = t; }
  
  static RadioButton<T>* build(const std::string name, T data, RadioButtonGroup<RadioButton<T>>* group, u16 x, u16 y, SpriteInfo untoggled, SpriteInfo toggled)
  {
    return new RadioButton(name, data, group, x, y, untoggled, untoggled.frame(1), toggled, toggled.frame(1));
  }
};

template<typename T>
class RadioClickable : public Clickable
{
  using group_t = RadioButtonGroup<RadioClickable<T>>;

protected:
  group_t* group;
  bool toggled;
  T data;

public:
  RadioClickable(T data, group_t* group, u16 x, u16 y, u16 w, u16 h) : Clickable(x, y, w, h), group(group), toggled(false), data(data) {
    setAction([this](){ this->group->click(this); });
  }
  
  const group_t* getGroup() const { return group; }
  void setToggled(bool t) { this->toggled = t; }
};

#endif
