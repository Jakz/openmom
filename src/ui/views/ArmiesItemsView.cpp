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
#include "ui/GfxData.h"
#include "Texture.h"

#include "UnitDraw.h"

using namespace std;

enum lbx_indices
{
  hero_gold_portrait = LBXI(ARMYLIST, 6),
  hero_items_backdrop = LBXI(ARMYLIST, 5)
};

enum_simple_map<items::Class, SpriteInfo, 6> emptyItemBackground = {
  { items::Class::MELEE, LBXI(ITEMISC, 19) },
  { items::Class::RANGED, LBXI(ITEMISC, 20) },
  { items::Class::MELEE_STAFF, LBXI(ITEMISC, 21) },
  { items::Class::STAFF_WAND, LBXI(ITEMISC, 22) },
  { items::Class::ARMOR, LBXI(ITEMISC, 24 ) },
  { items::Class::MISC, LBXI(ITEMISC, 23 ) }
};

static const Point heroPositions[] = {{32,15},{32,61},{32,107},{167,15},{167,61},{167,107}};

ArmiesItemsView::ArmiesItemsView(ViewManager* gvm) : View(gvm)
{
  buttons.resize(BUTTON_COUNT);
  
  buttons[ALCHEMY] = Button::buildBistate("Alchemy", 236, 157, LSI(ARMYLIST, 7))->setAction([gvm](){gvm->switchOverview(VIEW_ALCHEMY);});
  buttons[OK] = Button::buildBistate("Ok", 236, 177, LSI(ARMYLIST, 8))->setAction([gvm](){gvm->closeOverview();});
}

void ArmiesItemsView::draw()
{
  Gfx::draw(hero_items_backdrop, 17, 2);
  
  // draw heroes portraits
  
  
  auto heroes = player->getHeroes();
  
  for (int i = 0; i < heroes.size(); ++i)
  {
    const Hero* hero = *next(heroes.begin(), i);
    
    // TODO: unit names and items
    
    // TODO: real colors
    Fonts::drawString(hero->name(), FontFaces::Small::YELLOW, heroPositions[i].x+35, heroPositions[i].y-2, ALIGN_LEFT);
    
    Gfx::draw(hero_gold_portrait, heroPositions[i].x-1, heroPositions[i].y-1);
    UnitDraw::drawHeroPortrait(hero, heroPositions[i].x, heroPositions[i].y);
    
    const auto& slots = hero->getSpec()->items;
    for (int j = 0; j < items::Item::MAX_SLOTS; ++j)
    {
      const items::Item* item = hero->itemAt(j);
      const Point pos = Point(heroPositions[i].x+40+29*j, heroPositions[i].y+16);
      
      if (!item)
        Gfx::draw(emptyItemBackground[slots.types[j]], pos);
      else
      {
        //TODO: check positioning
        auto gfx = GfxData::itemGfxSpec(item->type(), item->gfx());
        Gfx::draw(gfx, pos);
        //TODO: enable
        //Gfx::drawGlow(gfx, pos.x+1, pos.y, School::NATURE);
      }
    }
    
  }
}
