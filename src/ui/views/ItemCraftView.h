//
//  ItemCraftView.h
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _ITEM_CRAFT_VIEW_H_
#define _ITEM_CRAFT_VIEW_H_

#include "ui/View.h"
#include "game/units/Items.h"

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
  
  Item::TypeID currentType;
  s8 currentItemGfx;
  std::string itemName;
  
  School school;
  
  void updateItemName();
  
  RadioButtonGroup<Item::TypeID>* itemType;
  
  static constexpr const Item::TypeID ITEM_TYPES[] = {
    Item::TypeID::SWORD, Item::TypeID::MACE, Item::TypeID::AXE, Item::TypeID::BOW, Item::TypeID::STAFF,
    Item::TypeID::WAND, Item::TypeID::MISC, Item::TypeID::SHIELD, Item::TypeID::CHAIN, Item::TypeID::PLATE
  };
  
public:
  ItemCraftView(ViewManager* gvm);
  
  void activate() override
  {
    currentType = Item::TypeID::SWORD;
    currentItemGfx = 0;
    updateItemName();
  }
  
  void deactivate() override { }
};

#endif
