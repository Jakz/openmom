//
//  ItemCraftView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "ItemCraftView.h"

#include "Font.h"
#include "Buttons.h"
#include "Texture.h"
#include "Gfx.h"
#include "ColorMap.h"

#include "Util.h"

constexpr const Item::TypeID ItemCraftView::ITEM_TYPES[];

ItemCraftView::ItemCraftView(ViewManager* gvm) : View(gvm), school(NATURE), currentType(Item::TypeID::SWORD), currentItemGfx(0)
{
  buttons.resize(BUTTON_COUNT);
  
  buttons[PREV_GFX] = BistateButton::build("Prev Gfx", 4, 24, TextureID::ITEM_CRAFT_ARROWS, 0)->setAction([this](){
    --currentItemGfx;
    if (currentItemGfx < 0)
      currentItemGfx = Item::typeForItem(currentType)->count();
    updateItemName();
  });
  
  buttons[NEXT_GFX] = BistateButton::build("Next Gfx", 17, 24, TextureID::ITEM_CRAFT_ARROWS, 1)->setAction([this](){
    ++currentItemGfx;
    currentItemGfx %= Item::typeForItem(currentType)->count();
    updateItemName();
  });
  
  itemType = new RadioButtonGroup<Item::TypeID>();
  itemType->setAction([this](RadioButton<Item::TypeID>* b){
    currentType = b->getData();
    currentItemGfx = 0;
    school = static_cast<School>(Util::randomIntInclusive(1, 5)); // TODO: why?
    updateItemName();
  });
  
  for (int i = 0; i < sizeof(ITEM_TYPES)/sizeof(ITEM_TYPES[0]); ++i)
  {
    RadioButton<Item::TypeID>* button = RadioButton<Item::TypeID>::build("type", ITEM_TYPES[i], itemType, 156 + 33*(i%5), 3 + 15*(i/5), TextureID::ITEM_CRAFT_BUTTONS, i, 0, 2);
    buttons.push_back(button);
    itemType->add(button);
  }
  
  itemType->set(0);
}

void ItemCraftView::updateItemName()
{
  
}

void ItemCraftView::draw()
{
  Gfx::draw(TextureID::ITEM_CRAFT_BACKDROP, 0, 0);
  Gfx::draw(TextureID::ITEMS, Item::typeForItem(currentType)->startOffset + currentItemGfx, 7, 6);
  Gfx::drawGlow(TextureID::ITEMS, 7, 6, Item::typeForItem(currentType)->startOffset + currentItemGfx, school);
  
  Fonts::drawString(itemName, FontFaces::Small::GRAY_ITEM_CRAFT, 29, 12, ALIGN_LEFT);

}