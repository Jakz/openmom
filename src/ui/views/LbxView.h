//
//  AlchemyView.h
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#pragma once

#include "View.h"
#include "Common.h"
#include "LBX.h"

#include <map>

namespace lbx
{
  class LBXView : public View
  {
  private:
    s16 selectedLBX;
    s16 selectedContent;
    
    s16 lbxOffset;
    s16 contentOffset;
    
    bool hasNextFile, hasPrevFile;
    bool hasNextContent, hasPrevContent;
    
    std::map<LBXFileID, lbx::string_list> filesForLBX;
    
    void updateContentButtons();
    
    void draw() override;
    void drawPost() override { }
    
  public:
    LBXView(ViewManager* gvm);
    
    void activate() override { }
    void deactivate() override { }
    
    void mouseReleased(u16 x, u16 y, MouseButton b) override;
    void selectLBX();
    void selectGFX();
  };
}
