//
//  CartographerView.h
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#pragma once

#include "common/Common.h"
#include "View.h"

namespace items
{
  class Item;
}

class MerchantView : public ViewWithQueue
{
private:
  enum button_id
  {
    BUY,
    REJECT
  };
  
  enum class Mode
  {
    ITEM
  };
  
  void draw() override;
  void drawPost() override { }
  
  void setup();
  
  Mode mode;

public:
  MerchantView(ViewManager* gvm);
  
  void activate() override;
  void deactivate() override { }
  
  void openWithOffer(const items::Item* item, u16 price);
  
  bool mouseReleased(u16 x, u16 y, MouseButton b) override;
};
