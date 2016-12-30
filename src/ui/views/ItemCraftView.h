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
#include "Buttons.h"

#include <string>

class ViewManager;

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
  
  u16 propertyCostLimit;
  
  School school;
  
  void updateItemName();
  
  std::unique_ptr<RadioButtonGroup<RadioButton<items::Item::TypeID>>> itemType;
  
  static const items::Item::TypeID ITEM_TYPES[];
  
  void updateClickableAreas();
  
  using affix_clickable_t = RadioClickable<size_t>;
  using affix_radio_group_t = RadioButtonGroup<affix_clickable_t>;

  class ClickableAffix : public affix_clickable_t
  {
    std::string left;
    std::string right;
    
    const FontSpriteSheet* font();
    const FontSpriteSheet* fontSelected();
  public:
    ClickableAffix(affix_radio_group_t* group, size_t index, std::string left, std::string right, u16 x, u16 y, u16 w, u16 h);
    void draw() override;
  };
  
  class ClickableGroup
  {
    std::vector<const ClickableAffix*> affixes;
  };
  
  clickable_list<ClickableAffix> clickables;
  std::vector<std::unique_ptr<affix_radio_group_t>> groups;

  size_t selectedPropertiesCount();

public:
  ItemCraftView(ViewManager* gvm);
  
  void activate() override
  {
    currentType = items::Item::TypeID::SWORD;
    currentItemGfx = 0;
    updateItemName();
    updateClickableAreas();
  }
  
  void deactivate() override { }
  
  void mouseReleased(u16 x, u16 y, MouseButton b) override;
};

#endif
