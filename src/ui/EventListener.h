//
//  MouseListener.h
//  OpenMoM
//
//  Created by Jack on 7/19/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _EVENT_LISTENER_H_
#define _EVENT_LISTENER_H_

#include "common/Common.h"

class MouseEventListener
{
public:
  virtual bool mousePressed(u16 x, u16 y, MouseButton b) { return true; };
  virtual bool mouseClicked(u16 x, u16 y, MouseButton b) { return true; };
  virtual bool mouseReleased(u16 x, u16 y, MouseButton b) { return true; };
  virtual bool mouseMoved(u16 x, u16 y, MouseButton b) { return true; };
  virtual bool mouseDragged(u16 x, u16 y, MouseButton b) { return true; };
  virtual bool mouseWheel(s16 dx, s16 dy, u16 d) { return true; }
};

class KeyEventListener
{
public:
  virtual bool textInput(sdl_text_input data) { return true; }
  virtual bool keyPressed(KeyboardCode key, KeyboardKey kkey, KeyboardMod mod) { return true; };
  virtual bool keyReleased(KeyboardCode key, KeyboardKey kkey, KeyboardMod mod) { return true; };
};

class EventListener : public MouseEventListener, public KeyEventListener
{
  

};

#endif
