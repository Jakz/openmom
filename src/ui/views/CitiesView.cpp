//
//  ArmiesView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "CitiesView.h"

#include "Game.h"
#include "City.h"
#include "LocalPlayer.h"

#include "Gfx.h"
#include "Texture.h"
#include "Buttons.h"
#include "Font.h"
#include "Unit.h"
#include "UnitDraw.h"
#include "Viewport.h"
#include "GfxData.h"

#include "ViewManager.h"

using namespace std;

constexpr u32 PAGE_SIZE = 9;

CitiesView::CitiesView(ViewManager* gvm) : View(gvm), offset(0), city(nullptr)
{
  buttons.resize(BUTTON_COUNT);
  
  buttons[OK] = Button::buildBistate("Ok", 239, 182, LSI(RELOAD, 22))->setAction([gvm](){ gvm->switchView(VIEW_MAIN); });
  // TODO: arrows buttons
  
  auto lambdaScrollDown = [this](){ scrollDown(); };
  auto lambdaScrollUp = [this](){ scrollUp(); };

  
  buttons[PREV1] = Button::buildTristate("Prev1", 20, 26, LSI(ARMYLIST, 1))->setAction(lambdaScrollUp);
  buttons[NEXT1] = Button::buildTristate("Next1", 20, 139, LSI(ARMYLIST, 2))->setAction(lambdaScrollDown);
  buttons[PREV2] = Button::buildTristate("Prev2", 259, 26, LSI(ARMYLIST, 1))->setAction(lambdaScrollUp);
  buttons[NEXT2] = Button::buildTristate("Next1", 250, 139, LSI(ARMYLIST, 2))->setAction(lambdaScrollDown);
}

void CitiesView::updateScrollButtons()
{
  buttons[PREV1]->activateIf(offset > 0);
  buttons[PREV2]->activateIf(offset > 0);
  buttons[NEXT1]->activateIf(offset + PAGE_SIZE + 1 < cities.size());
  buttons[NEXT2]->activateIf(offset + PAGE_SIZE + 1 < cities.size());
}

void CitiesView::activate()
{
  /* let's create a copy of the list of cities, maybe we can introduce custom sorting later */
  std::copy(player->getCities().begin(), player->getCities().end(), std::back_inserter(cities));
  
  if (!cities.empty())
    city = *cities.begin();
}

void CitiesView::deactivate()
{
  offset = 0;
  cities.clear();
  city = nullptr;
}

void CitiesView::draw()
{
  Gfx::draw(LSI(RELOAD, 21), 0, 0);

  std::string title = Fonts::format("The Cities Of %s", player->name.c_str()); //TODO: localize and check if player name or wizard name
  Fonts::drawString(title, FontFaces::Serif::GOLD, WIDTH/2 - 1, 2, ALIGN_CENTER); //TODO: correct font (with double shadow)
  
  Fonts::setFace(FontFaces::Small::RED_PALE, 0, 1);
  
  static const char* headers[] = { "Name", "Race", "Pop", "Gold", "Prd", "Producing", "Time" };
  static const s16 headerX[] = { 0, 56, 104, 122, 144, 165, 240 };
  static const Point headerBase = Point(31, 16);
  
  for (size_t i = 0; i < sizeof(headers)/sizeof(headers[0]); ++i)
    Fonts::drawString(headers[i], headerBase.x + headerX[i], headerBase.y, ALIGN_LEFT);
  
  
  //static const s16 headerColumns [] = {
  
  for (u32 i = 0; i < PAGE_SIZE; ++i)
  {
    const u32 index = PAGE_SIZE*offset + i;
    const City* city = nullptr;
    
    if (index < cities.size())
      city = cities[index];
    
    city = cities.front();
    
    if (city)
    {
      constexpr s16 delta = 14;
      const Point base = Point(30, 26 + i*delta);
      
      const std::string& cityName = city->getName();
      const std::string& raceName = i18n::s(GfxData::raceGfxSpec(city->race).unitName);
      const std::string population = std::to_string(5/*city->getPopulation()/1000*/);
      const std::string gold = std::to_string(8);
      const std::string production = std::to_string(9);
      const Productable* productable = city->getProductable();
      const std::string productionTurns = std::to_string(g->cityMechanics.turnsRequiredForProduction(city));

      
      Fonts::drawString(cityName, base.x, base.y, ALIGN_LEFT);
      Fonts::drawString(raceName, base.x + 56, base.y, ALIGN_LEFT);
      Fonts::drawString(population, base.x + 119, base.y, ALIGN_RIGHT);
      Fonts::drawString(gold, base.x + 139, base.y, ALIGN_RIGHT);
      Fonts::drawString(production, base.x + 159, base.y, ALIGN_RIGHT);

      if (productable)
      {
        Fonts::drawString(productable->productionName(), base.x + 165, base.y, ALIGN_LEFT);
        Fonts::drawString(productionTurns, base.x + 258, base.y, ALIGN_RIGHT);

      }
    }
  }
  
  

  if (city)
  {
    // draw minimap
    const Position& apos = city->getPosition();
    Viewport::drawMicroMap(player, 42, 162, 49, 33, apos.x, apos.y, apos.plane, Gfx::mainPalette->get(1));
    if (Gfx::fticks%4 == 0)
      Gfx::drawPixel({255,255,255}, 42 + 49/2, 162 + 33/2);
    
    // draw city? name
    Fonts::drawString(/*unit->name().toUpper()*/"LIZARDMAN SWORDSMEN", 187, 160, ALIGN_CENTER);
    
  }
}

bool CitiesView::mouseMoved(u16 x, u16 y, MouseButton b)
{
  return true;
}
