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
#include "Common.h"
#include "Texture.h"

#include <string>
#include <functional>
#include <vector>

class FontSpriteSheet;

typedef std::function<void()> Action;

class Clickable
{
protected:
  u16 x, y, w, h;
  const MouseButton button;
  Action action;
  bool active;
  
public:
  Clickable(u16 x, u16 y, u16 w, u16 h, MouseButton b = BUTTON_LEFT) : x(x), y(y), w(w), h(h), button(b), active(true) { }
  
  bool isCorrectButton(MouseButton b) { return b == button; }
  bool isInside(u16 x, u16 y) { return x >= this->x && x <= this->x+w && y >= this->y && y <= this->y+h; }
  virtual bool isActive() { return active; }
  inline void activate() { active = true; }
  inline void deactivate() { active = false; }
  inline void activateIf(bool condition) { active = condition; }
  
  virtual void setPosition(u16 x, u16 y) { this->x = x; this->y = y; }
  
  virtual inline Clickable* setAction(Action action) { this->action = action; return this; }
  inline Action getAction() { return action; }
  
  void draw();
};

struct ButtonGfx
{
  optional<SpriteInfo> normal;
  optional<SpriteInfo> pressed;
  optional<SpriteInfo> inactive;
  
  ButtonGfx() = default;
  ButtonGfx(SpriteInfo normal) : normal(normal) { }
  ButtonGfx(SpriteInfo normal, SpriteInfo pressed) : normal(normal), pressed(pressed) { }
  ButtonGfx(SpriteInfo normal, SpriteInfo pressed, SpriteInfo inactive) : normal(normal), pressed(pressed), inactive(inactive) { }
};

struct TextInfo
{
  std::string label;
  const FontSpriteSheet* font;
  ScreenCoord position;
  
  TextInfo() = default;
  TextInfo(const std::string& label, const FontSpriteSheet* font, ScreenCoord position) : label(label), font(font), position(position) { }
};

class Button : public Clickable
{
protected:
  const std::string name;
  bool pressed, visible;
  SpriteInfo normalCoords;
  
  Button(const std::string& name, u16 x, u16 y, u16 w, u16 h) : Clickable(x, y, w, h), name(name), pressed(false), visible(true) { }
  
  Button(const std::string name, u16 x, u16 y, SpriteInfo info) :
  Clickable(x, y, info.sw(), info.sh()), name(name), pressed(false), visible(true), normalCoords(info)
  {
    
  }
  
public:
  virtual void draw() = 0;
  
  inline void execute() { if (action) action(); }
  
  inline void showIf(bool condition) { visible = condition;}
  inline Button* hide() { visible = false; return this; }
  inline Button* show() { visible = true; return this; }
  inline bool isVisible() { return visible;}
  
  bool isActive() override { return active && visible; }
  
  inline Button* setAction(Action action) override { this->action = action; return this; }
  
  virtual void setLabel(const std::string&) { }
  
  template<typename T> T* as() { return static_cast<T*>(this); }
  
  inline void press() { pressed = true;}
  inline void release() { pressed = false; }
  
  virtual void click() {
    //TODO log clicked
    execute();
  }
};

class NormalButton : public Button
{
protected:
  bool shouldOffsetNormal;
  ButtonGfx gfx;
  optional<TextInfo> labelGfx;
  
public:
  NormalButton(const std::string& name, u16 x, u16 y, u16 w, u16 h) : Button(name, x, y, w, h) { }
  NormalButton(const std::string& name, u16 x, u16 y, SpriteInfo normal, bool shouldOffsetNormal) : Button(name, x, y, normal), gfx(normal), shouldOffsetNormal(shouldOffsetNormal) { }
  NormalButton(const std::string& name, u16 x, u16 y, SpriteInfo normal, SpriteInfo pressed) : Button(name, x, y, normal), gfx(normal, pressed) { }
  NormalButton(const std::string& name, u16 x, u16 y, SpriteInfo normal, SpriteInfo pressed, SpriteInfo inactive) : Button(name, x, y, normal), gfx(normal, pressed, inactive) { }

  void setTextInfo(const TextInfo& info) { labelGfx = info; }
  void setLabel(const std::string& string) override { assert(labelGfx.isPresent()); labelGfx->label = string; }
  
  void draw() override;
  void setPosition(u16 x, u16 y) override;
  
  static Button* buildSimple(const std::string& name, u16 x, u16 y, SpriteInfo normal) { return new NormalButton(name, x, y, normal, false); }
  
  static Button* buildPressedOnly(const std::string& name, u16 x, u16 y, SpriteInfo pressed) {
    auto* bt = new NormalButton(name, x, y, pressed.sw(), pressed.sh());
    bt->gfx.pressed = pressed;
    return bt;
  }
  
  static Button* buildOffsetted(const std::string& name, u16 x, u16 y, SpriteInfo normal) { return new NormalButton(name, x, y, normal, true); }
  
  static Button* buildBistate(const std::string& name, u16 x, u16 y, SpriteInfo normal, SpriteInfo pressed) { return new NormalButton(name, x, y, normal, pressed); }
  static Button* buildBistate(const std::string& name, u16 x, u16 y, SpriteInfo normal) { return new NormalButton(name, x, y, normal, normal.frame(1)); }
  
  static Button* buildTristate(const std::string& name, u16 x, u16 y, SpriteInfo normal, SpriteInfo pressed, SpriteInfo inactive) { return new NormalButton(name, x, y, normal, pressed, inactive); }
  static Button* buildTristate(const std::string& name, u16 x, u16 y, SpriteInfo normal, SpriteInfo inactive) { return new NormalButton(name, x, y, normal, normal.frame(1), inactive); }
  static Button* buildTristate(const std::string& name, u16 x, u16 y, SpriteInfo normal) { return new NormalButton(name, x, y, normal, normal.frame(1), normal); }
  
};

class SimpleButton : public Button
{
public:
  SimpleButton(const std::string name, u16 x, u16 y, SpriteInfo info) : Button(name, x, y, info) { }
  void draw() override;
};

class OffsetButton : public Button
{
public:
  OffsetButton(const std::string name, u16 x, u16 y, SpriteInfo info) : Button(name, x, y, info) { }
  void draw() override;
};

class BistateButton : public Button
{
protected:
  SpriteInfo pressedCoords;
  
  BistateButton(const std::string name, u16 x, u16 y, SpriteInfo normal, SpriteInfo pressed) : Button(name, x, y, normal), pressedCoords(pressed) { }
  
public:
  void draw() override;

  static BistateButton* build(const std::string name, u16 x, u16 y, TextureID texture, u16 c)
  {
    return new BistateButton(name, x, y, SpriteInfo(texture, 0, c), SpriteInfo(texture, 1, c));
  }
  
  static BistateButton* buildLBX(const std::string name, u16 x, u16 y, SpriteInfo info)
  {
    assert(info.isLBX());
    return new BistateButton(name, x, y, info, info.frame(1));
  }
};

class BistateLabeledButton : public BistateButton
{
protected:
  ScreenCoord textPosition;
  std::string label;
  const FontSpriteSheet* font;
  BistateLabeledButton(const std::string name, u16 x, u16 y, SpriteInfo normal, SpriteInfo pressed, std::string label, const FontSpriteSheet* font);
  
public:
  void draw() override;
  
  void setLabel(const std::string& label) override { this->label = label; }
  void setPosition(u16 x, u16 y) override;
  
  static BistateLabeledButton* buildLBX(const std::string name, u16 x, u16 y, SpriteInfo info, std::string label, const FontSpriteSheet* font)
  {
    assert(info.isLBX());
    return new BistateLabeledButton(name, x, y, info, info.frame(1), label, font);
  }
};

class LabeledSimpleButton : public OffsetButton
{
protected:
  std::string label;
  u16 textX, textY;
  const FontSpriteSheet* font;
  
public:
  LabeledSimpleButton(const std::string name, u16 x, u16 y, SpriteInfo info, std::string label, const FontSpriteSheet* font);
  void setLabel(std::string label) { this->label = label; }
  void draw() override;
};

template<typename T>
class RadioButton;

template<typename T>
class RadioButtonGroup
{
private:
  std::vector<RadioButton<T>*> buttons;
  RadioButton<T>* current;
  std::function<void(RadioButton<T>*)> lambda;
public:
  RadioButtonGroup() : current(nullptr) { }
  
  RadioButton<T>* getCurrent() { return current; }
  void set(u16 index) { current = buttons[index]; }
  void click(RadioButton<T>* button)
  {
    if (current != button)
    {
      current = button;
      lambda(current);
    }
  }
  
  void add(RadioButton<T>* button)
  {
    buttons.push_back(button);
  }
  
  void setAction(std::function<void(RadioButton<T>*)> lambda) { this->lambda = lambda; }
};

template<typename T>
class RadioButton : public NormalButton
{
private:
  RadioButtonGroup<T>* group;
  
  ButtonGfx toggledGfx;
  bool toggled;
  
  T data;
  
  RadioButton(const std::string name, T data, RadioButtonGroup<T>* group, u16 x, u16 y, SpriteInfo normal, SpriteInfo pressed, SpriteInfo normalToggled, SpriteInfo pressedToggled)
  : NormalButton(name, x, y, normal, pressed), data(data), toggledGfx(normalToggled, pressedToggled), toggled(false), group(group) { }
  
public:
  void draw() override;
  
  const T getData() const { return data; }
  
  void click() override { group->click(this); }
  
  static RadioButton* build(const std::string name, T data, RadioButtonGroup<T>* group, u16 x, u16 y, SpriteInfo untoggled, SpriteInfo toggled)
  {
    return new RadioButton(name, data, group, x, y, untoggled, untoggled.frame(1), toggled, toggled.frame(1));
  }
};

#include "Items.h" // FIXME: forced for explicit declaration of template method

template class RadioButton<items::Item::TypeID>;

#endif
