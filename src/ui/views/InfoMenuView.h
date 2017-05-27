//
//  InfoMenuView.h
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#pragma once

#include "View.h"

#include "Dialogs.h"


class InfoMenuView : public View
{
private:
  void draw() override;
  void drawPost() override { }
  
  std::vector<dialogs::InfoMenu> menus;
  
public:
  InfoMenuView(ViewManager* gvm);
  
  void activate() override;
  void deactivate() override { }
};
