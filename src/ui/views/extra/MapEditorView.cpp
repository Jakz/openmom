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

#include "Minimap.h"
#include "World.h"
#include "WorldGenerator.h"

#include "Army.h"

#include "Viewport.h"
#include "GfxData.h"
#include "Localization.h"

#include "LBX.h"

#include "ViewManager.h"

constexpr s32 TILE_WIDTH = 20, TILE_HEIGHT = 18;
constexpr s32 MARGIN = 1;
constexpr s32 OX = 1, OY = 1;
constexpr size_t VIEWPORT_WIDTH = 12, VIEWPORT_HEIGHT = 10;

constexpr s32 MAP_WIDTH = 60, MAP_HEIGHT = 40;

const static std::array<Brush, 13> brushes = {
{
  {
    Brush::Type::OCEAN,
    LSI(TERRAIN, 0),
    [](Tile* tile)
    {
      tile->type = TileType::OCEAN;
      DirJoin land = tile->computeMask([](const Tile* tile) { return tile && tile->isSolidLand(); });
      if (land != DirJoin::NONE)
        tile->type = TileType::SHORE;
    }
  },
  {
    Brush::Type::GRASSLANDS,
    LSI(TERRAIN, 1),
    [](Tile* tile)
    {
      tile->type = TileType::GRASS;
    }
  },
  {
    Brush::Type::DESERT,
    LSI(TERRAIN, 0xA5),
    [](Tile* tile)
    {
      tile->type = TileType::DESERT;
    }
  },
  {
    Brush::Type::TUNDRA,
    LSI(TERRAIN, 0xA7),
    [](Tile* tile)
    {
      tile->type = TileType::TUNDRA;
    }
  },
  {
    Brush::Type::HILLS,
    LSI(TERRAIN, 0xAB),
    [](Tile* tile)
    {
      tile->type = TileType::HILL;
    }
  },
  {
    Brush::Type::MOUNTAINS,
    LSI(TERRAIN, 0xA4),
    [](Tile* tile)
    {
      tile->type = TileType::MOUNTAIN;
    }
  },
  {
    Brush::Type::FOREST,
    LSI(TERRAIN, 0xA3),
    [](Tile* tile)
    {
      tile->type = TileType::FOREST;
    }
  },
  {
    Brush::Type::VOLCANO,
    LSI(TERRAIN, 0xB3),
    [](Tile* tile)
    {
      tile->type = TileType::VOLCANO;
    }
  },
  {
    Brush::Type::SWAMP,
    LSI(TERRAIN, 0xA6),
    [](Tile* tile)
    {
      tile->type = TileType::SWAMP;
    }
  },
  {
    Brush::Type::RIVER,
    LSI(TERRAIN, 0xED),
    [](Tile* tile)
    {
      tile->type = TileType::RIVER;
    }
  },
  {
    Brush::Type::SORCERY_NODE,
    LSI(TERRAIN, 0xA8),
    [](Tile* tile)
    {

    }
  },
  {
    Brush::Type::NATURE_NODE,
    LSI(TERRAIN, 0xA9),
    [](Tile* tile)
    {

    }
  },
  {
    Brush::Type::CHAOS_NODE,
    LSI(TERRAIN, 0xAA),
    [](Tile* tile)
    {

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
  world->fill(TileType::GRASS, Plane::ARCANUS);
  world->fill(TileType::GRASS, Plane::MYRRAN);
  world->calcSubTiles();
  
  minimap = new MiniMap(world);
  minimap->discover(Rect(0,0,60,40), Plane::ARCANUS);
  minimap->discover(Rect(0,0,60,40), Plane::MYRRAN);
  
  //WorldGenerator generator(world);
  //generator.atlasGenerate();
  
  hover = Point::INVALID;
  brush = brushes.begin();
  offset = Point::ZERO;
  plane = Plane::ARCANUS;
  
  mode = Mode::TERRAIN;
}

void MapEditorView::deactivate()
{
  // TODO: release resources
}

Point MapEditorView::positionForBrush(size_t i)
{
  static const Point PALETTE_POSITION = Point(256,40+3);
  constexpr s32 PALETTE_DY = 20, PALETTE_DX = 22;
  constexpr s32 BRUSH_PER_COLUMN = 7;
  
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
  
  Gfx::draw(minimap->get(plane), 256, OY);
  
  if (offset.x < MAP_WIDTH - VIEWPORT_WIDTH)
    Gfx::rect((256 + offset.x - 1), (OY + offset.y - 1), VIEWPORT_WIDTH + 1, VIEWPORT_HEIGHT + 1, {255,0,0});
  else
  {
    Gfx::rect((256 + offset.x - 1), (OY + offset.y - 1), MAP_WIDTH - offset.x + 1, VIEWPORT_HEIGHT + 1, {255,0,0});
    Gfx::rect((256 - 1), (OY + offset.y - 1), VIEWPORT_WIDTH - (MAP_WIDTH - offset.x) + 1, VIEWPORT_HEIGHT + 1, {255,0,0});
  }

  
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
    
    minimap->discover(Position(tile->x(), tile->y(), plane));
    tile->for_each_neighbor([this](const Tile* neighbor) {
      if (neighbor)
        minimap->discover(neighbor->position);
    });
  }
}

bool MapEditorView::mousePressed(u16 x, u16 y, MouseButton b)
{
  Point h = hoveredTile(Point(x,y));
  
  if (h.isValid())
  {
    if (b == MouseButton::BUTTON_LEFT)
      clickOnTile(Point(h.x + offset.x, h.y + offset.y));
    else if (b == MouseButton::BUTTON_RIGHT)
    {
      offset = offset + h - Point(VIEWPORT_WIDTH/2, VIEWPORT_HEIGHT/2);
      offset.y = std::max((s16)0, offset.y);
      offset.y = std::min((s16)(world->h - VIEWPORT_HEIGHT), offset.y);
      
      offset.x %= MAP_WIDTH;
      if (offset.x < 0) offset.x += MAP_WIDTH;
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
