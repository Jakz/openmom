//
//  MouseListener.h
//  OpenMoM
//
//  Created by Jack on 7/19/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _EVENT_LISTENER_H_
#define _EVENT_LISTENER_H_

#include "Common.h"

class EventListener
{
  protected:
  virtual void mousePressed(u16 x, u16 y, MouseButton b) { };
  virtual void mouseClicked(u16 x, u16 y, MouseButton b) { };
  virtual void mouseReleased(u16 x, u16 y, MouseButton b) { };
  virtual void mouseMoved(u16 x, u16 y, MouseButton b) { };
  virtual void mouseDragged(u16 x, u16 y, MouseButton b) { };
  
  virtual void keyPressed(KeyboardKey key, KeyboardMod mod) { };
  virtual void keyReleased(KeyboardKey key, KeyboardMod mod) { };
};

#endif
