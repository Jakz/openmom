//
//  ItemCraftView.h
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#pragma once

#include "View.h"

#include <string>
#include <list>

class ViewManager;
class FontSpriteSheet;

class ConsoleView : public View
{
private:
  void draw() override;
  void drawPost() override { }
  
  struct TextEntry
  {
    std::string string;
    const FontSpriteSheet* face;
  };
  
  std::list<TextEntry> buffer;
  std::string line;
  
  void executeCommand(std::string cmd);
  
public:
  ConsoleView(ViewManager* gvm);
  
  void activate() override;
  void deactivate() override { }
  
  bool keyPressed(KeyboardCode key, KeyboardKey kkey, KeyboardMod mod) override;
  bool keyReleased(KeyboardCode key, KeyboardKey kkey, KeyboardMod mod) override;
};
