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
  spellNameFont = new fonts::MediumBoldFont({69,2,31}, {142, 93, 35});
  spellNameSelectedFont = new fonts::MediumBoldFont({154,0,0}, {73, 56, 35});

  populate(player);
}

void ItemCraftChargesView::draw()
{
  
}

void ItemCraftChargesView::populate(const Player* player)
{
  mode = Mode::SPELL_SELECT;
  
  const SpellBook* book = player->book();
  std::vector<ResearchStatus> combatSpells = book->discoveredSpells(SpellBook::Type::COMBAT);
  
  //TODO: group by school
  spells = { "Earth to Mud", "Giant Strength", "Web", "Stone Skin", "Warp Wood", "Fire Bolt", "Wall of Fire", "Shatter", "Warp Creature", "Fire Elemental",
    "Lightning Bolt", "Fireball", "Magic Vortex", "Warp Reality", "Dispel Magic", "Disenchant Area", "Recall Hero"
  };
  selected = std::make_pair(nullptr, spells.end());
  
  areas.clear();
  buttons.clear();
  clear();
  
  /* draw bg */
  add(new BasicSpriteEntry(LSI(SPELLS, 0), Point(28, 12)));
  
  constexpr u32 BX = 57, BY = 18, DX = 130, DY = 10;
  constexpr u32 ENTRIES_PER_PAGE = 14;
  const static Point CHARGES_BOX_BOUNDS[] = { Point(43, 18), Point(183,118) };
  const static u32 CHARGES_BUTTON_DY = 15;
  const static u32 CHARGES_BUTTON_DX = 3;
  const static char* CHARGES_BUTTON_NAMES[] = { "1x", "2x", "3x", "4x" };
  
  u32 i = 0;
  for (auto it = spells.begin(); it != spells.end(); ++it)
  {
    u32 y = BY + (i%ENTRIES_PER_PAGE)*DY, x = i < ENTRIES_PER_PAGE ? BX : (BX + DX);
    BasicTextEntry* entry = new BasicTextEntry(*it, spellNameFont, Point(x, y), ALIGN_LEFT);
    add(entry);
    Clickable* area = new Clickable(x, y - 1, 100, 10);
    area->setAction([this,entry,it,i,y]() {
      if (mode == Mode::SPELL_SELECT)
      {
        entry->setFace(spellNameSelectedFont);
        selected = std::make_pair(entry, it);
        mode = Mode::CHARGES_SELECT;
        
        Point chargesBoxPosition = Point(
                                         i < ENTRIES_PER_PAGE ? CHARGES_BOX_BOUNDS[1].x : CHARGES_BOX_BOUNDS[0].x,
                                         std::max(std::min(y - 11, (u32)CHARGES_BOX_BOUNDS[1].y), (u32)CHARGES_BOX_BOUNDS[0].y)
                                         );
        
        chargesBG->setPosition(chargesBoxPosition);
        chargesBG->setVisible(true);
        
        chargesCaption->setPosition(chargesBoxPosition + Point(48, 4));
        chargesCaption->setVisible(true);
        
        /* add four charges buttons which will invoke chargesPicked */
        for (u32 c = 0; c < 4; ++c)
          addButton(Button::buildBistate(CHARGES_BUTTON_NAMES[c], chargesBoxPosition.x + CHARGES_BUTTON_DX + 23*c, chargesBoxPosition.y + CHARGES_BUTTON_DY, LSI(SPELLSCR, 38 + c)))->setAction([this,c](){
            chargesPicked(c);
          });
      }
    });
    addArea(area);
    
    ++i;
  }
  
  Clickable* exit = new Clickable(186, 169, 20, 33);
  exit->setAction([this]() { gvm->closeOverview(); });
  addArea(exit);
  
  chargesBG = new BasicSpriteEntry(LSI(SPELLSCR, 37), Point::ZERO);
  chargesBG->setVisible(false);
  add(chargesBG);
  
  //TODO: localize
  chargesCaption = new BasicTextEntry("Charges", spellNameFont, Point::ZERO, ALIGN_CENTER);
  chargesCaption->setVisible(false);
  add(chargesCaption);
  
  //TODO: localize + font is wrong
  add(new BasicTextEntry("Choose a spell to embed in this item", FontFaces::Serif::GOLD_SHADOW, Point(WIDTH/2 - 1, 1), ALIGN_CENTER));
}

void ItemCraftChargesView::chargesPicked(u32 amount)
{
  //TODO: place real spell
  callback(items::ItemSpellCharge(nullptr, amount));
  gvm->closeOverview();
}

bool ItemCraftChargesView::mouseReleased(u16 x, u16 y, MouseButton b)
{
  if (mode == Mode::CHARGES_SELECT && b == MouseButton::BUTTON_RIGHT)
  {
    selected.first->setFace(spellNameFont);
    selected = std::make_pair(nullptr, spells.end());
    mode = Mode::SPELL_SELECT;
    
    chargesBG->setVisible(false);
    chargesCaption->setVisible(false);
    buttons.clear();
    
    return true;
  }
  
  return false;
}

