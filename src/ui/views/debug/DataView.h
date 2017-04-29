//
//  ResearchView.h
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#pragma once

#include "View.h"
#include "DrawQueue.h"
#include "DrawQueueBasic.h"

class DataView : public View
{
private:
  enum class Mode
  {
    UNITS
  };
  
  int page;
  Mode mode;
  
  DrawQueue<BasicDrawEntry> entries;
  std::vector<std::string> keys;
  
  void draw() override;
  void drawPost() override { }
  
  void hover(int index);
  
public:
  DataView(ViewManager* gvm);
  
  void switchMode(Mode mode);
  
  void activate() override;
  void deactivate() override;
};
