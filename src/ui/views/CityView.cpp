//
//  CityView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "CityView.h"

#include "City.h"
#include "CityScape.h"
#include "LocalPlayer.h"
#include "Game.h"
#include "Army.h"
#include "World.h"
#include "Tile.h"
#include "CityMechanics.h"
#include "Viewport.h"

#include "Localization.h"

#include "ViewManager.h"

#include "Gfx.h"
#include "Texture.h"
#include "Font.h"
#include "Buttons.h"
#include "Messages.h"
#include "UnitDraw.h"

#include "ProductionView.h"

#include "Lbx.h"


CityView::CityView(ViewManager* gvm) : View(gvm)
{
  buttons.resize(BUTTON_COUNT);
  
  buttons[BUY] = TristateButton::build("Buy", 214, 188, TextureID::CITY_VIEW_BUTTONS, 0);
  buttons[CHANGE] = TristateButton::build("Change", 247, 188, TextureID::CITY_VIEW_BUTTONS, 1);
  buttons[OK] = TristateButton::build("Ok", 286, 188, TextureID::CITY_VIEW_BUTTONS, 2);

  buttons[BUY]->deactivate();
  
  buttons[BUY]->setAction([this](){
    player->send(new msgs::Confirm(Fonts::format("Do you wish to spend %d Gold by purchasing a %s?", city->getProductable()->productionCost(), city->getProductable()->productionName().c_str()), [&]() {
      g->cityMechanics.buyProduction(city);
    }));
  });
  
  buttons[OK]->setAction([gvm](){ gvm->switchView(VIEW_MAIN); });
  buttons[CHANGE]->setAction([gvm,this]{ gvm->productionView()->setCty(city); gvm->switchOverview(VIEW_PRODUCTION); });
  
  for (int i = 0; i < 6; ++i)
    areas.push_back((new Clickable(139, 51 + 7*i, 68, 7))->setAction([this,i](){
      const cast_list& spells = city->getSpells();
      
      
      if (i < spells.size())
      {
        const SpellCast& cast = *next(spells.begin(), i);
        if (cast.player == player)
        {
          player->send(new msgs::Confirm(Fonts::format("Do you wish to turn off ^s%s^^?", i18n::s(cast.spell->name).c_str()), [&]() {
            city->removeSpell(cast);
          }));
        }
      }
    }));
}

void CityView::setCity(City *city)
{
  this->city = city;
  
  buttons[BUY]->activateIf(g->cityMechanics.isProductionBuyable(city));

  if (!CityLayout::contains(city))
    CityLayout::createLayout(city);
  else /*if (city->revalidateLayout)*/
    CityLayout::updateLayout(city);
}

s16 CityView::drawCityProp(s16 value, s16 row, s16 sx, s16 sy)
{
  for (s16 i = value; i > 0; --i)
    if (i >= 10)
    {
      i -= 9;
      Gfx::draw(TextureID::CITY_PRODUCTION, row, 2, sx, sy);
      sx += 15;
    }
    else
    {
      Gfx::draw(TextureID::CITY_PRODUCTION, row, 0, sx, sy);
      sx += 6;
    }
  
  return sx;
}

void CityView::draw()
{
  CityLayout::draw(city, player);
  
  /* draw minimap */
  Viewport::drawCityViewport(player, g->world, city->getPosition());
  
  Gfx::draw(lbx::Repository::spriteFor(LBXSpriteInfo(LBXID::BACKGRND, 6)), 0, 0);
  
  
  //Gfx::draw(TextureID::CITY_VIEW_BACKDROP, 0, 0);
  
  int popX = 4, reqFarmers = city->reservedPopulation;
  for (int i = 0; i < (city->population / 1000); ++i)
  {
    if (i < reqFarmers)
      Gfx::draw(TextureID::CITY_RACES_LABOUR, 1, static_cast<u16>(city->race.ident), popX, 25);
    else if (i < reqFarmers + city->farmers)
      Gfx::draw(TextureID::CITY_RACES_LABOUR, 1, static_cast<u16>(city->race.ident), popX, 25);
    else
      Gfx::draw(TextureID::CITY_RACES_LABOUR, 0, static_cast<u16>(city->race.ident), popX, 25);
    
    if (i == reqFarmers - 1)
      popX += 4;
    
    popX += 9;
  }
  
  int sx = 6;
  int sy = 52;
  
  //TODO: spacing between icons still incorrect
  
  sx += drawCityProp(city->necessaryFood, 0, sx, sy) + 5;
  drawCityProp(city->food, 0, sx, sy);
  sx = 6;
  sy += 8;
  drawCityProp(city->work, 1, sx, sy);
  sx = 6;
  sy += 8;
  sx += drawCityProp(city->upkeep.gold, 2, sx, sy) + 5;
  drawCityProp(city->gold, 2, sx, sy);
  sx = 6;
  sy += 8;
  drawCityProp(city->mana, 3, sx, sy);
  sx = 6;
  sy += 8;
  drawCityProp(city->knowledge, 4, sx, sy);
  
  Army* army = g->world->get(city->getPosition())->army;
  
  if (army)
  {
    for (int i = 0; i < army->size(); ++i)
    {
      const Unit* u = army->get(i);
      //if (!army.isPatrol())
      UnitDraw::drawStatic(u, 215+20*(i%5), 102+17*(i/5), true, false);
    }
  }
  
  /* draw city info: race and population */
  Fonts::drawString(i18n::s(city->race.name), FontFaces::Small::YELLOW, 5, 18, ALIGN_LEFT);
  //TODO: localize digits
  Fonts::drawString(Fonts::format("Population: %s (+%d)", Fonts::groupDigits(city->population).c_str(), city->growthRate), FontFaces::Small::YELLOW, 209, 18, ALIGN_RIGHT);
  
  const Productable* production = city->getProductable();
  
  /* draw current production */
  if (production == Building::HOUSING || production == Building::TRADE_GOODS)
  {
    Gfx::draw(TextureID::CITY_TRADE_HOUSING_BACKDROP, 260, 149);
    CityLayout::drawBuildingCentered(static_cast<const Building*>(production), 217, 179);
    
    /* TODO: localize and center align */
    if (production == Building::HOUSING)
      Fonts::drawStringBounded("Increases population growth rate.", FontFaces::Tiny::WHITE, 286, 158, 50, ALIGN_CENTER);
    else if (production == Building::TRADE_GOODS)
      Fonts::drawStringBounded("Convert production to gold.", FontFaces::Tiny::WHITE, 286, 158, 40, ALIGN_CENTER);
    
  }
  else
  {
    if (production->productionType() == Productable::Type::BUILDING)
    {
      CityLayout::drawBuildingCentered(static_cast<const Building*>(city->getProductable()), 217, 177);
    }
    else if (production->productionType() == Productable::Type::UNIT)
    {
      UnitDraw::drawUnitIso(static_cast<const UnitSpec*>(production), 218, 144, nullptr);
    }
    
    /* compute and draw missing turns to complete production */
    /* TODO: localize */
    int prodToDo = (production->productionCost() - city->productionPool);
    int turns = prodToDo == 0 ? 1 : (prodToDo/city->work + (prodToDo%city->work == 0 ? 0 : 1));
    Fonts::drawString(Fonts::format("%d Turn%s", turns, (turns == 1 ? "" : "s")), FontFaces::Small::YELLOW, 316, 139, ALIGN_RIGHT);
    
    /* draw production coins */
    int max = 10;
    for (int i = 0; i < std::min(production->productionCost()/10, 40); ++i)
    {
      /* TODO: fixare il draw parziale delle monete */
      //TextureID::drawClippedToRow(TextureID::CITY_PRODUCTION, 5, 0, 262+i%max*5, 151+i/max*7, 2);
      if (i*10 < city->productionPool)
        Gfx::draw(TextureID::CITY_PRODUCTION, 5, 1, 262+i%max*5, 151+i/max*7);
      else
        Gfx::draw(TextureID::CITY_PRODUCTION, 5, 0, 262+i%max*5, 151+i/max*7);
    }
  }
  
  /* TODO: in realtà il font sarebbe obreggiato dall'alto / gestire wrapping nomi lunghi (ship wrights guild etc) */
  if (Fonts::stringWidth(FontFaces::Small::WHITE, production->productionName()) < 40)
    Fonts::drawString(production->productionName(), FontFaces::Small::WHITE, 237, 178, ALIGN_CENTER);
  else
    Fonts::drawString(production->productionName(), FontFaces::Tiny::WHITE, 235, 177, ALIGN_CENTER);
  
  
  // print city enchantments
  int i = 0;
  for (const SpellCast& cast : city->getSpells())
  {
    const FontSpriteSheet* face = Fonts::fontForColor(cast.player->color);
    Fonts::drawString(i18n::s(cast.spell->name), face, 138, 50+7*i, ALIGN_LEFT);
    ++i;
  }
  
  /*for (int i = 0; i < 5; ++i)
   {
   Fonts::drawString("Dark Rituals", Fonts::fontForColor(Color.values()[i]), 138, 50+7*i, ALIGN_LEFT);
   }*/
}
