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
  
public:
  StartView(ViewManager* gvm);
  
  void activate() override;
  void deactivate() override { }
};

#endif
