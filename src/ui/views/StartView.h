//
//  ResearchView.h
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _START_VIEW_H_
#define _START_VIEW_H_

#include "View.h"

class StartView : public View
{
private:
  void draw() override;
  void drawPost() override { }
  
  FontSpriteSheet* mainFont;
  
  enum
  {
    BUTTON_CONTINUE = 0,
    BUTTON_HALL_OF_FAME,
    BUTTON_EXIT_TO_DOS,
    BUTTON_NEW_GAME,
    BUTTON_LOAD_GAME,
    
    BUTTON_COUNT
  };
  
public:
  StartView(ViewManager* gvm);
  
  void activate() override;
  void deactivate() override { }
};

#endif
