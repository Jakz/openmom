//
//  ArmiesItemsView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "ArmiesItemsView.h"

#include "ViewManager.h"

#include "LocalPlayer.h"
#include "Unit.h"
#include "Items.h"

#include "Buttons.h"
#include "Gfx.h"
#include "Texture.h"

#include "UnitDraw.h"

using namespace std;

enum lbx_indices
{
  hero_gold_portrait = LBXI(ARMYLIST, 6)
};

ArmiesItemsView::ArmiesItemsView(ViewManager* gvm) : View(gvm)
{
  buttons.resize(BUTTON_COUNT);
  
  buttons[ALCHEMY] = NormalButton::buildBistate("Alchemy", 236, 157, LSI(ARMYLIST, 7))->setAction([gvm](){gvm->switchOverview(VIEW_ALCHEMY);});
  buttons[OK] = NormalButton::buildBistate("Ok", 236, 177, LSI(ARMYLIST, 8))->setAction([gvm](){gvm->closeOverview();});
}

void ArmiesItemsView::draw()
{
  Gfx::draw(LSI(ARMYLIST, 5), 17, 2);
  
  // draw heroes portraits
  u16 heroPositions[][2] = {{32,15},{32,61},{32,107},{167,15},{167,61},{167,107}};
  
  auto heroes = player->getHeroes();
  
  for (int i = 0; i < heroes.size(); ++i)
  {
    const Hero* hero = *next(heroes.begin(), i);
    
    // TODO: unit names and items
    
    // TODO: real colors
    Fonts::drawString(hero->name(), FontFaces::Small::YELLOW, heroPositions[i][0]+35, heroPositions[i][1]-2, ALIGN_LEFT);
    
    Gfx::draw(hero_gold_portrait, heroPositions[i][0]-1, heroPositions[i][1]-1);
    UnitDraw::drawHeroPortrait(hero, heroPositions[i][0], heroPositions[i][1]);
    
    for (int j = 0; j < Item::MAX_SLOTS; ++j)
    {
      const Item* item = hero->itemAt(j);
      
      u8 indices[] = {0,4,5};
      // TODO: real slots
      if (!item)
      {
        Gfx::draw(TextureID::UNIT_HERO_ITEM_BACKGROUND, 0, indices[j], heroPositions[i][0]+40+29*j, heroPositions[i][1]+16);
      }
    }
    
  }
}
