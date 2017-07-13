//
//  ResearchView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "MapEditorView.h"

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

#include "Viewport.h"
#include "GfxData.h"
#include "Localization.h"

#include "LBX.h"

#include "ViewManager.h"

constexpr s32 TILE_WIDTH = 20, TILE_HEIGHT = 18;
constexpr s32 MARGIN = 1;
constexpr s32 OX = 1, OY = 5;
constexpr size_t VIEWPORT_WIDTH = 12, VIEWPORT_HEIGHT = 10;

const static std::array<Brush, 2> brushes = {
{
  {
    Brush::Type::OCEAN,
    LSI(TERRAIN, 0),
    [](Tile* tile)
    {
      tile->type = TILE_WATER;
      DirJoin land = tile->computeMask([](const Tile* tile) { return tile && tile->isSolidLand(); });
      if (land != DirJoin::NONE)
        tile->type = TILE_SHORE;
    }
  },
  {
    Brush::Type::GRASSLANDS,
    LSI(TERRAIN, 1),
    [](Tile* tile)
    {
      tile->type = TILE_GRASS;
    }
  }
} };


MapEditorView::MapEditorView(ViewManager* gvm) : ViewWithQueue(gvm)
{
  lbx::Repository::loadLBXTerrain();
  
  s16 y = 5;
  for (const Brush& brush : brushes)
  {
    lbx::Repository::loadLBXSpriteTerrainData(brush.info);
    addButton(Button::buildSimple("", 260, y, brush.info));
    y += 20;

  }
  
}

void MapEditorView::activate()
{
  world = new World(nullptr,60,40);
  world->fill(TileType::TILE_GRASS, Plane::ARCANUS);
  world->fill(TileType::TILE_GRASS, Plane::MYRRAN);
  world->calcSubTiles();
  
  hover = Point::INVALID;
  brush = brushes.begin();
  offset = Point::ZERO;
  plane = Plane::ARCANUS;
}

void MapEditorView::draw()
{
  for (size_t x = 0; x < VIEWPORT_WIDTH; ++x)
  {
    for (size_t y = 0; y < VIEWPORT_HEIGHT; ++y)
    {
      Viewport::drawTile(world->get(offset.x + x, offset.y + y, plane), OX + x*(TILE_WIDTH+MARGIN), OY + y*(TILE_HEIGHT+MARGIN), plane);
    }
  }
  
  if (hover.isValid())
    Gfx::rect(OX + hover.x*(TILE_WIDTH+MARGIN)-1, OY + hover.y*(TILE_HEIGHT+MARGIN)-1, TILE_WIDTH+MARGIN, TILE_HEIGHT+MARGIN, {255,0,0});
}

void MapEditorView::setup()
{

}

Point MapEditorView::hoveredTile(Point pt)
{
  u16 tx = (pt.x - OX) / (TILE_WIDTH+MARGIN);
  u16 ty = (pt.y - OY) / (TILE_HEIGHT+MARGIN);
  
  if (tx >= 0 && tx < VIEWPORT_WIDTH && ty >= 0 && ty < VIEWPORT_HEIGHT)
    return Point(tx, ty);
  else
    return Point::INVALID;
}

bool MapEditorView::mouseReleased(u16 x, u16 y, MouseButton b)
{
  Point h = hoveredTile(Point(x,y));
  if (h.isValid())
  {
    Tile* tile = world->get(h.x + offset.x, h.y + offset.y, plane);
    if (tile)
    {
      brush->lambda(tile);
      world->calcSubTile(tile->x(), tile->y(), plane);
      tile->for_each_neighbor([this](Tile* t) { if (t) world->calcSubTile(t->x(), t->y(), plane); });
    }
    
    return true;
  }
  
  return false;
}

bool MapEditorView::mouseMoved(u16 x, u16 y, MouseButton b)
{
  this->hover = hoveredTile(Point(x,y));
  
  return true;
}
