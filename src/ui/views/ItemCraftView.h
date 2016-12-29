//
//  ItemCraftView.h
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _ITEM_CRAFT_VIEW_H_
#define _ITEM_CRAFT_VIEW_H_

#include "View.h"
#include "Items.h"

#include <string>

class ViewManager;

template<typename T>
class RadioButtonGroup;

class ItemCraftView : public View
{
private:
  enum Button
  {
    PREV_GFX,
    NEXT_GFX,
    
    BUTTON_COUNT
  };
  
  void draw() override;
  void drawPost() override { }
  
  items::Item::TypeID currentType;
  s8 currentItemGfx;
  std::string itemName;
  
  School school;
  
  void updateItemName();
  
  RadioButtonGroup<items::Item::TypeID>* itemType;
  
  static const items::Item::TypeID ITEM_TYPES[];
  
public:
  ItemCraftView(ViewManager* gvm);
  
  void activate() override
  {
    currentType = items::Item::TypeID::SWORD;
    currentItemGfx = 0;
    updateItemName();
  }
  
  void deactivate() override { }
};

#endif
