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
#include "GfxData.h"
#include "ColorMap.h"
#include "Messages.h"
#include "Player.h"
#include "LocalPlayer.h"

#include "Util.h"

using namespace items;

const TypeID ItemCraftView::ITEM_TYPES[] = {
  TypeID::SWORD, TypeID::MACE, TypeID::AXE, TypeID::BOW, TypeID::STAFF,
  TypeID::WAND, TypeID::MISC, TypeID::SHIELD, TypeID::CHAIN, TypeID::PLATE
};

sprite_ref TYPE_BUTTONS[][2] = {
  { LBXI(SPELLSCR, 14), LBXI(SPELLSCR, 25) },
  { LBXI(SPELLSCR, 15), LBXI(SPELLSCR, 26) },
  { LBXI(SPELLSCR, 16), LBXI(SPELLSCR, 27) },
  { LBXI(SPELLSCR, 17), LBXI(SPELLSCR, 28) },
  { LBXI(SPELLSCR, 18), LBXI(SPELLSCR, 29) },
  { LBXI(SPELLSCR, 19), LBXI(SPELLSCR, 30) },
  { LBXI(SPELLSCR, 20), LBXI(SPELLSCR, 31) },
  { LBXI(SPELLSCR, 21), LBXI(SPELLSCR, 32) },
  { LBXI(SPELLSCR, 22), LBXI(SPELLSCR, 33) },
  { LBXI(SPELLSCR, 23), LBXI(SPELLSCR, 34) }
};

const FontSpriteSheet* ItemCraftView::ClickableAffix::font() { return toggled ? FontFaces::MediumBold::GOLD_ITEM_CRAFT : FontFaces::MediumBold::BROWN_ITEM_CRAFT; }

ItemCraftView::ClickableAffix::ClickableAffix(affix_radio_group_t* group, size_t index, std::string left, std::string right, u16 x, u16 y, u16 w, u16 h)
: affix_clickable_t(index, group, x, y, w, 10), left(left), right(right) { }

void ItemCraftView::ClickableAffix::draw()
{
  Fonts::drawString(left.c_str(), font(), x, y, ALIGN_RIGHT);
  Fonts::drawString(right.c_str(), font(), x+2, y, ALIGN_LEFT);
  
  Clickable::draw();
}

ItemCraftView::ItemCraftView(ViewManager* gvm) : View(gvm), school(NATURE), currentType(TypeID::SWORD), currentItemGfx(0), propertyCostLimit(200)
{
  buttons.resize(BUTTON_COUNT);
  
  buttons[PREV_GFX] = Button::buildBistate("Prev Gfx", 4, 24, LSI(SPELLSCR, 35))->setAction([this](){
    --currentItemGfx;
    if (currentItemGfx < 0)
      currentItemGfx = Item::typeForItem(currentType)->count();
    updateItemName();
  });
  
  buttons[NEXT_GFX] = Button::buildBistate("Next Gfx", 17, 24, LSI(SPELLSCR, 36))->setAction([this](){
    ++currentItemGfx;
    currentItemGfx %= Item::typeForItem(currentType)->count();
    updateItemName();
  });
  
  itemType = decltype(itemType)(new RadioButtonGroup<RadioButton<TypeID>>());
  itemType->setAction([this](RadioButton<TypeID>* b){
    currentType = b->getData();
    currentItemGfx = 0;
    school = static_cast<School>(Util::randomIntInclusive(1, 5)); // TODO: why?
    updateClickableAreas();
    updateItemName();
  });
  
  for (size_t i = 0; i < sizeof(ITEM_TYPES)/sizeof(ITEM_TYPES[0]); ++i)
  {
    RadioButton<TypeID>* button = RadioButton<TypeID>::build("type", ITEM_TYPES[i], itemType.get(), 156 + 33*(i%5), 3 + 15*(i/5), TYPE_BUTTONS[i][0], TYPE_BUTTONS[i][1]);
    buttons.push_back(button);
    itemType->add(button);
  }
  
  itemType->set(0);
}

size_t ItemCraftView::selectedPropertiesCount()
{
  size_t count = std::count_if(groups.begin(), groups.end(), [](const decltype(groups)::value_type& group) { return group->getCurrent() != nullptr; });
  return count;
}


void ItemCraftView::updateClickableAreas()
{
  clickables.clear();
  groups.clear();
  
  /* get all affixes for item type */
  const auto affixes = items::Affixes::forType(currentType);
  
  constexpr int BASE_Y = 40;
  constexpr int BASE_X = 21;
  constexpr int MAX_H = 196;
  constexpr int LINE_HEIGHT = 11;
  constexpr int GROUP_SPACING = 3;
  
  int by = BASE_Y;
  int bx = BASE_X;
  for (const auto& affix : affixes.properties)
  {
    /* since property cost limit may be specified (which is 200 for enchant item spell we need
     to keep only affixes which have lower cost
     */
    size_t effectiveRange = affix.sizeForCost(propertyCostLimit);
    auto mediumFace = FontFaces::MediumBold::BROWN_ITEM_CRAFT;

    if (by + effectiveRange*LINE_HEIGHT > MAX_H)
    {
      bx += 91;
      by = BASE_Y;
    }
    
    affix_radio_group_t* group = new affix_radio_group_t(true);
    group->setCanSelectPredicate((std::function<bool(RadioClickable<size_t>*)>)[this, group] (RadioClickable<size_t>* button) {
      size_t count = selectedPropertiesCount();
      
      if (button->getGroup()->getCurrent() == nullptr)
        ++count;
      
      bool valid = count <= 4;
      
      if (!valid)
      {
        msgs::Error* error = new msgs::Error("Only four powers may be enchanted into an item"); //TODO: localize, graphics is not correctly centered
        this->player->send(error);
      }
      
      return valid;
    });
    groups.emplace_back(group);
    
    for (size_t i = 0; i < effectiveRange; ++i)
    {
      std::string left = Fonts::format("%+d", affix.valueAt(i));
      std::string right = Fonts::format("%s", affix.name().c_str());
      u16 width = Fonts::stringWidth(mediumFace, right) /*+ Fonts::stringWidth(mediumFace, left)*/ + 2;
      
      ClickableAffix* area = new ClickableAffix(group, i, left, right, bx, by, width, LINE_HEIGHT);
      group->add(area);
      clickables.emplace_back(area);

      by += LINE_HEIGHT;
    }
    by += GROUP_SPACING;
  }

}

void ItemCraftView::updateItemName()
{
  
}

void ItemCraftView::draw()
{
  Gfx::draw(LSI(SPELLSCR, 13), 0, 0);
  
  const auto itemGfx = GfxData::itemGfxSpec(currentType, currentItemGfx);
  
  Gfx::draw(itemGfx, 7, 6);
  Gfx::drawGlow(itemGfx, 7, 6, school);
  
  Fonts::drawString(itemName, FontFaces::Small::GRAY_ITEM_CRAFT, 29, 12, ALIGN_LEFT);
  
  clickables.draw();
}

void ItemCraftView::mouseReleased(u16 x, u16 y, MouseButton b)
{
  clickables.handleEvent(x, y, b);
}
