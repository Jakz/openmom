//
//  Buttons.h
//  OpenMoM
//
//  Created by Jack on 7/19/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _BUTTONS_H_
#define _BUTTONS_H_

#include "Common.h"
#include "Texture.h"

#include <string>
#include <functional>
#include <SDL2/SDL.h>

enum FontFace : u16;

typedef std::function<void()> Action;

class Clickable
{
  protected:
    const u16 x, y, w, h;
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
  
    inline void setAction(Action action) { this->action = action; }
    inline Action getAction() { return action; }
  
    void draw();
};

class Button : public Clickable
{
  protected:
    const std::string name;
    bool pressed, visible;
    SpriteInfo normalCoords;
  
  public:
    Button(const std::string name, u16 x, u16 y, SpriteInfo info) :
      Clickable(x,y,Texture::get(info.texture).span(info.x),Texture::get(info.texture).h), name(name), pressed(false), visible(true), normalCoords(info)
    {
    
    }
  
    virtual void draw() = 0;
  
    inline void execute() { action(); }
  
    inline void showIf(bool condition) { visible = condition;}
    inline void hide() { visible = false; }
    inline void show() { visible = true; }
    inline bool isVisible() { return visible;}
  
    bool isActive() override { return active && visible; }

    inline void press() { pressed = true;}
    inline void release() { pressed = false; }
  
    inline void click() {
      //TODO log clicked
      execute();
    }
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

  public:
    BistateButton(const std::string name, u16 x, u16 y, SpriteInfo normal, SpriteInfo pressed) : Button(name, x, y, normal), pressedCoords(pressed) { }
    void draw() override;
};

class TristateButton : public BistateButton
{
  protected:
    SpriteInfo inactiveCoords;
    
  public:
    TristateButton(const std::string name, u16 x, u16 y, SpriteInfo normal, SpriteInfo pressed, SpriteInfo inactive) : BistateButton(name, x, y, normal, pressed), inactiveCoords(inactive) { }
    void draw() override;
};

class LabeledSimpleButton : public OffsetButton
{
  protected:
    std::string label;
    u16 textX, textY;
    FontFace font;
  
  public:
    LabeledSimpleButton(const std::string name, u16 x, u16 y, SpriteInfo info, std::string label, FontFace font) : OffsetButton(name,x,y,info), label(label), font(font) { }
    void setLabel(std::string label) { this->label = label; }
    void draw() override;
};


#endif