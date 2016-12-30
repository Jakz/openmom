//
//  MirrorView.h
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _MIRROR_VIEW_H_
#define _MIRROR_VIEW_H_

#include "View.h"

class ViewManager;

class MirrorView : public View
{
private:
  enum
  {
    OPTIONS_OK,
    OPTIONS_CANCEL,
    OPTIONS_DIFFICULTY,
    OPTIONS_OPPONENTS,
    OPTIONS_LAND_SIZE,
    OPTIONS_MAGIC,
    
    BUTTON_COUNT
  };
  
  void draw() override;
  void drawPost() override { }
  
public:
  MirrorView(ViewManager* gvm);
  
  void mousePressed(u16 x, u16 y, MouseButton b) override;
  void keyPressed(KeyboardCode key, KeyboardKey kkey, KeyboardMod mod) override;
  
  void activate() override { }
  void deactivate() override { }
};

#endif
