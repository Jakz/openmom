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
  
  struct ItemLocation
  {
    enum class Type { HERO, VAULT, NONE } type;
    Hero* hero;
    size_t index;
    
    ItemLocation() : type(Type::NONE) { }
    ItemLocation(Hero* hero, size_t index) : type(Type::HERO), hero(hero), index(index) { }
    ItemLocation(size_t index) : type(Type::VAULT), hero(nullptr), index(index) { }
  };
  
  void draw() override;
  void drawPost() override;
  
  void clickOnItemSlot(ItemLocation location, MouseButton bt);
  
  ItemLocation heldLocation;
  const items::Item* heldItem;
  
  coord_t mouseX, mouseY;
  
public:
  ArmiesItemsView(ViewManager* gvm);
  
  void activate() override;
  void deactivate() override { }
  
  bool mouseMoved(u16 x, u16 y, MouseButton bt) override
  {
    mouseX = x;
    mouseY = y;
    return true;
  }
};

#endif
