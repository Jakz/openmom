//
//  ItemCraftChargesView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "ItemCraftChargesView.h"

#include "Font.h"
#include "Buttons.h"
#include "Texture.h"
#include "Gfx.h"
#include "GfxData.h"
#include "ColorMap.h"
#include "Messages.h"
#include "Player.h"
#include "LocalPlayer.h"
#include "ViewManager.h"

#include "ui/parts/DrawQueueBasic.h"

#include "common/Util.h"

using namespace items;

ItemCraftChargesView::ItemCraftChargesView(ViewManager* gvm) : ViewWithQueue(gvm), callback([](items::ItemSpellCharge) { })
{
  
}

void ItemCraftChargesView::activateWithCallback(callback_t callback)
{
  this->callback = callback;
  this->gvm->switchOverview(VIEW_ITEM_CRAFT_CHARGES);
}

void ItemCraftChargesView::activate()
{
  spellNameFont = FontFaces::MediumBold::BROWN_START;// new fonts::MediumBoldFont({69,2,31}, {192, 143, 35});
  spellNameSelectedFont = new fonts::MediumBoldFont({69,2,31}, {192, 143, 35});

  populate(player);
}

void ItemCraftChargesView::draw()
{
  
}

void ItemCraftChargesView::populate(const Player* player)
{
  const SpellBook* book = player->book();
  std::vector<ResearchStatus> combatSpells = book->discoveredSpells(SpellBook::Type::COMBAT);
  
  //TODO: group by school
  
  
  std::vector<std::string> spells = { "Earth to Mud", "Giant Strength", "Web", "Stone Skin" };
  
  clear();
  /* draw bg */
  add(new BasicSpriteEntry(LSI(SPELLS, 0), Point(28, 12)));
  
  int y = 10;
  for (const std::string& spell : spells)
  {
    add(new BasicTextEntry(spell, rand()%2 == 0 ? spellNameFont : spellNameFont, Point(30,y), ALIGN_LEFT));
    y += 11;
  }
}

bool ItemCraftChargesView::mouseReleased(u16 x, u16 y, MouseButton b)
{
  return true;
}

