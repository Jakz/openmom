//
//  ArmiesItemsView.h
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _ARMIES_ITEMS_VIEW_H_
#define _ARMIES_ITEMS_VIEW_H_

#include "View.h"

class ViewManager;

class ArmiesItemsView : public View
{
private:
  enum
  {
    ALCHEMY,
    OK,
    
    BUTTON_COUNT
  };
  
  void draw() override;
  void drawPost() override { }
  
  void clickOnHeroItemSlot(index_t heroIndex, index_t slotIndex);
  
public:
  ArmiesItemsView(ViewManager* gvm);
  
  void activate() override { }
  void deactivate() override { }
};

#endif
