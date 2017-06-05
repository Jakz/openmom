//
//  ArmiesView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "ArmiesView.h"

#include "Game.h"
#include "Army.h"
#include "LocalPlayer.h"

#include "Gfx.h"
#include "Texture.h"
#include "Buttons.h"
#include "Font.h"
#include "Unit.h"
#include "UnitDraw.h"
#include "Viewport.h"

#include "ViewManager.h"

using namespace std;

ArmiesView::ArmiesView(ViewManager* gvm) : View(gvm), offset(0), unit(nullptr), army(nullptr)
{
  buttons.resize(BUTTON_COUNT);
  
  buttons[ITEMS] = Button::buildBistate("Items", 273, 163, LSI(ARMYLIST, 3))->setAction([gvm](){ gvm->switchOverview(VIEW_ARMIES_ITEMS); });
  buttons[OK] = Button::buildBistate("Ok", 273, 182, LSI(ARMYLIST, 4))->setAction([gvm](){ gvm->switchView(VIEW_MAIN); });
  // TODO: arrows buttons
  
  auto lambdaScrollDown = [this](){ scrollDown(); };
  auto lambdaScrollUp = [this](){ scrollUp(); };

  
  buttons[PREV1] = Button::buildTristate("Prev1", 60, 26, LSI(ARMYLIST, 1))->setAction(lambdaScrollUp);
  buttons[NEXT1] = Button::buildTristate("Next1", 60, 139, LSI(ARMYLIST, 2))->setAction(lambdaScrollDown);
  buttons[PREV2] = Button::buildTristate("Prev2", 259, 26, LSI(ARMYLIST, 1))->setAction(lambdaScrollUp);
  buttons[NEXT2] = Button::buildTristate("Next1", 250, 139, LSI(ARMYLIST, 2))->setAction(lambdaScrollDown);
}

void ArmiesView::updateScrollButtons()
{
  buttons[PREV1]->activateIf(offset > 0);
  buttons[PREV2]->activateIf(offset > 0);
  buttons[NEXT1]->activateIf(offset + 6 + 1 < player->getArmies().size());
  buttons[NEXT2]->activateIf(offset + 6 + 1 < player->getArmies().size());
}

static const u16 HERO_PORTRAITS[][2] = {{12,5},{12,56},{12,107},{277,5},{277,56},{277,107}};

void ArmiesView::draw()
{
  Gfx::draw(LSI(ARMYLIST, 0), 0, 0);
  
  // draw heroes portraits
  auto heroes = player->getHeroes(); u8 i = 0;
  for (const auto hero : heroes)
  {
    UnitDraw::drawHeroPortrait(hero, HERO_PORTRAITS[i][0], HERO_PORTRAITS[i][1]);
    // TODO: fixare color map, serve pale red apposita
    Fonts::drawString(hero->name(), FontFaces::Tiny::WHITE, 27+265*(i/3), 41+51*(i%3), ALIGN_CENTER);
    ++i;
  }
  
  // draw armies
  u8 armiesPosition[2] = {78,25};
  u8 armiesYdelta = 22;
  
  // draw at most 6 armies and don't go over the armies count of the player
  auto armies = player->getArmies();
  for (int i = 0; i < 6 && i+offset < armies.size(); ++i)
  {
    int ri = offset + i;
    const Army* a = *next(armies.begin(),ri);
    
    // TODO: the dragon go over by 1 pixel, check where is the problem
    
    for (int j = 0; j < a->size(); ++j)
    {
      const Unit* u = a->get(j);
      
      if (a == army && u == unit)
        Gfx::drawClipped(TSI(UNIT_DETAIL_SPECIAL_THINGS,0,0), armiesPosition[0]+18*j, armiesPosition[1]+armiesYdelta*i+1, 40, 0, 19, 15);
      
      // TODO: 'G' letter related to a particular unit status, maybe pathfinding?
      UnitDraw::drawStatic(u, armiesPosition[0]+18*j, armiesPosition[1]+armiesYdelta*i, false, a->isPatrolling());
    }
  }
  
  Fonts::setFace(FontFaces::Small::RED_PALE, 0, 1);
  
  /* fill minimap background */
  Gfx::fillRect(86, 164, 48, 32, Gfx::mainPalette->get(1));
  
  if (army)
  {
    // draw minimap
    // TODO: size: 48, 32 but in original it's 48,33
    // offset 164, 86
    const Position& apos = army->getPosition();
    Viewport::drawMicroMap(player, 86, 164, 48, 32, apos.x, apos.y, apos.plane);
    if (Gfx::fticks%4 == 0)
      Gfx::drawPixel({255,255,255}, 86 + 48/2, 164 + 32/2);
    
    // draw unit name
    // TODO: space dello SmallCompactFont è 1 in meno di quanto è nel gioco originale
    Fonts::drawString(/*unit->name().toUpper()*/"LIZARDMAN SWORDSMEN", 187, 160, ALIGN_CENTER);
    
  }
  
  // draw upkeep
  Fonts::drawString("UPKEEP", 30, 161, ALIGN_CENTER);
  Upkeep upkeep;
  for (auto a : player->getArmies())
    for (auto u : *a)
      upkeep += u->upkeep();
  
  //TODO: real values, as above on unit name
  Fonts::drawString(/*to_string(upkeep.gold)+*/"7", 44, 172, ALIGN_CENTER);
  Fonts::drawString(/*to_string(upkeep.mana+*/"0", 44, 182, ALIGN_CENTER);
  Fonts::drawString(/*to_string(upkeep.food+*/"11", 45, 191, ALIGN_CENTER);
}

bool ArmiesView::mouseMoved(u16 x, u16 y, MouseButton b)
{
  u8 ap[] = {78,24};
  u8 ayd = 22;
  
  // select an army and an unit according to where the mouse is hovering
  s8 whichArmy = -1, whichUnit = -1;
  
  for (u8 i = 0; i < 6; ++i)
    if (y > ap[1]+ayd*i && y <= ap[1]+ayd*i + 16)
      whichArmy = i;
  
  if (x >= ap[0] && x < ap[0] + 18*9)
    whichUnit = (x - ap[0]) / 18;
	
  // if we're in an actual rectangle for which there could be an unit
  // we should select by hovering it if there is an army in the slot and an unit in the position
  if (whichArmy != -1 && whichUnit != -1)
  {
    if (whichArmy + offset < player->getArmies().size())
    {
      const Army* a = *next(player->getArmies().begin(), whichArmy);
      
      if (whichUnit < a->size())
      {
        // SELECT
        army = a;
        unit = a->get(whichUnit);
      }
    }
  }
  
  // hover on hero portrait
  for (int i = 0; i < 6; ++i)
  {
    const u16* bounds = HERO_PORTRAITS[i];
    
    if (x >= bounds[0] && x < bounds[0]+32 && y >= bounds[1] && y < bounds[1]+32)
    {
      auto heroes = player->getHeroes();
      if (heroes.size() > i)
      {
        const Hero* hero = *next(heroes.begin(), i);
        unit = hero;
        army = hero->getArmy();
        break;
      }
    }
  }
  
  return true;
}
