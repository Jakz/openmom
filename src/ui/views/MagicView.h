//
//  MagicView.h
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _MAGIC_VIEW_H_
#define _MAGIC_VIEW_H_

#include "View.h"

class ViewManager;

class MagicView : public View
{
private:
  enum
  {
    ALCHEMY,
    OK,
    
    BUTTON_COUNT
  };
  
  struct WandHoverInfo
  {
    s8 index;
    float percent;
    bool top;
  };
  
  struct ManaDest
  {
    float v;
    bool locked;
    u8 index;
  };
  
  ManaDest dests[3] = {{0.0f,false,0},{0.0f,false,1},{0.0f,false,2}};
  static constexpr size_t DESTS = 3;
  
  void draw() override;
  void drawPost() override { }

  WandHoverInfo hoveredWand(u16 x, u16 y);
  
public:
  MagicView(ViewManager* gvm);
  
  void activate() override;
  void deactivate() override { }
  
  void mouseReleased(u16 x, u16 y, MouseButton b) override;
  void mouseDragged(u16 x, u16 y, MouseButton b) override;
  
  void updateValues(u16 dest);
  void setPercent(u16 dest, float v);
};

#endif
