//
//  MirrorView.h
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _HELP_VIEW_H_
#define _HELP_VIEW_H_

#include "View.h"

#include "Help.h"

class ViewManager;

class HelpView : public View
{
private:
  help::Entry entry;
  
  void draw() override;
  void drawPost() override { }
  
public:
  HelpView(ViewManager* gvm);
  
  void mousePressed(u16 x, u16 y, MouseButton b) override;
  void keyPressed(KeyboardCode key, KeyboardKey kkey, KeyboardMod mod) override;
  
  void activate() override;
  void deactivate() override { }
  
  void setEntry(help::Entry entry) { this->entry = entry; }
};

#endif
