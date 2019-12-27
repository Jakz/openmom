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

#include "ui/parts/Dialogs.h"

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
    MERCHANT,
    DETAIL
  };
  
  void draw() override;
  void drawPost() override { }
  
  void setup();
  
  const items::Item* item;
  dialogs::ItemDetailDialog itemDetails;
  Mode mode;

public:
  MerchantView(ViewManager* gvm);
  
  void activate() override;
  void deactivate() override { }
  
  void openWithOffer(const items::Item* item, u16 price);
  void openWithDetail(const items::Item* item);
  
  bool mouseReleased(u16 x, u16 y, MouseButton b) override;
};
