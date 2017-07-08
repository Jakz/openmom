//
//  ResearchView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "CartographerView.h"

#include "Gfx.h"
#include "Texture.h"
#include "Buttons.h"
#include "Font.h"

#include "BookView.h"
#include "Spells.h"
#include "LocalPlayer.h"
#include "Game.h"
#include "World.h"

#include "Army.h"

#include "GfxData.h"
#include "Localization.h"

#include "ViewManager.h"

CartographerView::CartographerView(ViewManager* gvm) : ViewWithQueue(gvm)
{
  addArea(new Clickable(264,140,36,30))->setAction([this]() { this->gvm->switchView(VIEW_MAIN); });
  
  addArea(new Clickable(280,180,39,19))->setAction([this]()
  {
    clear();
    plane = plane == Plane::ARCANUS ? Plane::MYRRAN : Plane::ARCANUS;
    setup();
  });
}

void CartographerView::activate()
{
  this->plane = Plane::ARCANUS;
  setup();
}

void CartographerView::drawCity(const City* city)
{
  const auto position = city->getPosition();
  const auto point = positionForTile(position.x, position.y);
  
  const Color SHADOW = {8,4,4};
  const auto& gfx = GfxData::playerGfxSpec(city->getOwner()->color);
  
  Gfx::fillRect(point.x+1, point.y+1, 3, 3, SHADOW);
  Gfx::fillRect(point.x, point.y, 3, 3, gfx.cartographer.color);
}

void CartographerView::draw()
{
  const World* world = player->game()->world;
  
  const Size mapSize = Size(world->width(), world->height());
  const Size viewportSize = Size(60,40);
  const Point offset = Point(0,0);

  assert(mapSize == viewportSize);
  /* TODO: this view for now works only with 60x40 worlds, 
    scrollbars will be needed in case */

  for (u16 x = 0; x < viewportSize.w; ++x)
    for (u16 y = 0; y < viewportSize.h; ++y)
    {
      const Point position = offset.delta(x, y);
      const Position mapPosition = Position(position, plane);
      const Tile* tile = world->get(mapPosition);
      
      /* if tile is discovered */
      if (player->fog()->get(mapPosition))
      {
        if (tile->city)
          drawCity(tile->city);
        else if (tile->army)
        {
          const auto point = positionForTile(position.x, position.y);

          Gfx::fillRect(point.x+2, point.y+1, 1, 1, GfxData::playerGfxSpec(tile->army->getOwner()->color).cartographer.color);
        }
      }
    }
}

void CartographerView::drawLegend(u32 row, PlayerColor color, const std::string& name)
{
  Point position = Point(260, 80 + row*9);
  const auto& gfx = GfxData::playerGfxSpec(color);

  sprite(gfx.cartographer.flag, position);
  label(name, FontFaces::Small::BROWN, position.delta(8, 1), ALIGN_LEFT); // TODO: correct font colors
}

void CartographerView::setup()
{
  sprite(LSI(RELOAD, 2), {0, 0});
  label(plane == Plane::ARCANUS ? "Arcanus Plane" : "Myrror Plane", FontFaces::Huge::BROWN_CARTOGRAPHER, Point(WIDTH/2 - 1, 11), ALIGN_CENTER);
  
  drawLegend(0, PlayerColor::YELLOW, "Dracon");
  drawLegend(1, PlayerColor::GREEN, "Raven");
  drawLegend(2, PlayerColor::BLUE, "Kali");
  drawLegend(3, PlayerColor::RED, "Sharee");
  drawLegend(4, PlayerColor::PURPLE, "Freya");
  drawLegend(5, PlayerColor::NEUTRAL, "Neutral");

}
