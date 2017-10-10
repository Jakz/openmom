//
//  ItemCraftChargesView.h
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#pragma once

#include "View.h"
#include "Items.h"
#include "Buttons.h"

#include <string>

class Player;

namespace items
{
  class ItemSpellCharge;
}

class ViewManager;

class ItemCraftChargesView : public ViewWithQueue
{
public:
  using callback_t = std::function<void(items::ItemSpellCharge)>;
private:
  using list_entry_t = std::string;
  using selected_entry_t = std::pair<BasicTextEntry*, std::vector<list_entry_t>::const_iterator>;
  
  enum Mode
  {
    SPELL_SELECT,
    CHARGES_SELECT
  };
  
  Mode mode;
  callback_t callback;
  selected_entry_t selected;
  
  std::vector<list_entry_t> spells;
  
  const FontSpriteSheet *spellNameFont, *spellNameSelectedFont;
  
  BasicSpriteEntry* chargesBG;
  BasicTextEntry* chargesCaption;
  

  void draw() override;
  void drawPost() override { }
  
  void populate(const Player* player);
  
  void chargesPicked(u32 amount);
  
public:
  ItemCraftChargesView(ViewManager* gvm);
  
  void activate() override;
  void deactivate() override { }
  
  void activateWithCallback(callback_t callback);
  
  bool mouseReleased(u16 x, u16 y, MouseButton b) override;
};
