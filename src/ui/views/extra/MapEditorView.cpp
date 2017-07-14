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

const static std::array<Brush, 9> brushes = {
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
  },
  {
    Brush::Type::DESERT,
    LSI(TERRAIN, 0xA5),
    [](Tile* tile)
    {
      tile->type = TILE_DESERT;
    }
  },
  {
    Brush::Type::TUNDRA,
    LSI(TERRAIN, 0xA7),
    [](Tile* tile)
    {
      tile->type = TILE_TUNDRA;
    }
  },
  {
    Brush::Type::HILLS,
    LSI(TERRAIN, 0xAB),
    [](Tile* tile)
    {
      tile->type = TILE_HILL;
    }
  },
  {
    Brush::Type::MOUNTAINS,
    LSI(TERRAIN, 0xA4),
    [](Tile* tile)
    {
      tile->type = TILE_MOUNTAIN;
    }
  },
  {
    Brush::Type::FOREST,
    LSI(TERRAIN, 0xA3),
    [](Tile* tile)
    {
      tile->type = TILE_FOREST;
    }
  },
  {
    Brush::Type::VOLCANO,
    LSI(TERRAIN, 0xB3),
    [](Tile* tile)
    {
      tile->type = TILE_VOLCANO;
    }
  },
  {
    Brush::Type::SWAMP,
    LSI(TERRAIN, 0xA6),
    [](Tile* tile)
    {
      tile->type = TILE_SWAMP;
    }
  }
} };


MapEditorView::MapEditorView(ViewManager* gvm) : ViewWithQueue(gvm)
{
  lbx::Repository::loadLBXTerrain();
  
  size_t i = 0;
  for (auto it = brushes.begin(); it != brushes.end(); ++it)
  {
    lbx::Repository::loadLBXSpriteTerrainData(it->info);
    Point position = positionForBrush(i);
    addButton(Button::buildSimple("", position.x, position.y, it->info))->setAction([this, it](){
      this->brush = it;
    });
    
    ++i;
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
  
  mode = Mode::TERRAIN;
}

Point MapEditorView::positionForBrush(size_t i)
{
  static const Point PALETTE_POSITION = Point(260,5);
  constexpr s32 PALETTE_DY = 20, PALETTE_DX = 22;
  constexpr s32 BRUSH_PER_COLUMN = 6;
  
  return Point(PALETTE_POSITION.x + ((i/BRUSH_PER_COLUMN)*PALETTE_DX), PALETTE_POSITION.y + ((i%BRUSH_PER_COLUMN)*PALETTE_DY));
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
  
  size_t brushIndex = std::distance(brushes.begin(), brush);
  Point brushPosition = positionForBrush(brushIndex);

  Gfx::rect(brushPosition.x - 1, brushPosition.y - 1, 21, 19, {255,0,0});
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

void MapEditorView::clickOnTile(Point coords)
{
  Tile* tile = world->get(coords.x, coords.y, plane);
  if (tile)
  {
    brush->lambda(tile);
    world->calcSubTile(tile->x(), tile->y(), plane);
    tile->for_each_neighbor([this](Tile* t) { if (t) world->calcSubTile(t->x(), t->y(), plane); });
  }
}

bool MapEditorView::mousePressed(u16 x, u16 y, MouseButton b)
{
  Point h = hoveredTile(Point(x,y));
  if (h.isValid())
  {
    clickOnTile(Point(h.x + offset.x, h.y + offset.y));
    return true;
  }
  
  return false;
}

bool MapEditorView::mouseMoved(u16 x, u16 y, MouseButton b)
{
  this->hover = hoveredTile(Point(x,y));
  return true;
}

bool MapEditorView::mouseDragged(u16 x, u16 y, MouseButton b)
{
  Point oldHover = hover;
  this->hover = hoveredTile(Point(x,y));
  if (oldHover != hover && hover.isValid())
    clickOnTile(Point(hover.x + offset.x, hover.y + offset.y));
  return true;
}

bool MapEditorView::mouseWheel(s16 dx, s16 dy, u16 d)
{
  switch (mode)
  {
    case Mode::TERRAIN:
    {
      if (dy < 0 && brush < brushes.end()-1)
        ++brush;
      else if (dy > 0 && brush > brushes.begin())
        --brush;
      
      break;
    }
  }

  
  return true;
}
