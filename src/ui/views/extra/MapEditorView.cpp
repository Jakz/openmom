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

#include "MiniMap.h"
#include "World.h"
#include "WorldGenerator.h"

#include "Army.h"

#include "Viewport.h"
#include "GfxData.h"
#include "Localization.h"

#include "LBXRepository.h"

#include "ViewManager.h"

constexpr s32 BASE_TILE_WIDTH = 20, BASE_TILE_HEIGHT = 18;
constexpr s32 MARGIN = 1;
constexpr s32 OX = 1, OY = 1;
constexpr size_t BASE_VIEWPORT_WIDTH = 10, BASE_VIEWPORT_HEIGHT = 10;

constexpr s32 MAP_WIDTH = 60, MAP_HEIGHT = 40;

const static std::array<Brush, 13> brushes = {
{
  {
    LSI(TERRAIN, 0),
    [](Tile* tile)
    {
      if (tile->type != TileType::OCEAN && tile->type != TileType::SHORE)
      {
        tile->type = TileType::OCEAN;
        DirJoin land = tile->computeMask([](const Tile* tile) { return tile && tile->isSolidLand(); });
        if (land != DirJoin::NONE)
          tile->type = TileType::SHORE;
      }
      else
        tile->type = TileType::GRASS;
    }
  },
  {
    LSI(TERRAIN, 1),
    [](Tile* tile)
    {
      tile->type = TileType::GRASS;
    }
  },
  {
    LSI(TERRAIN, 0xA5),
    [](Tile* tile)
    {
      tile->type = TileType::DESERT;
    }
  },
  {
    LSI(TERRAIN, 0xA7),
    [](Tile* tile)
    {
      tile->type = TileType::TUNDRA;
    }
  },
  {
    LSI(TERRAIN, 0xAB),
    [](Tile* tile)
    {
      tile->type = TileType::HILL;
    }
  },
  {
    LSI(TERRAIN, 0xA4),
    [](Tile* tile)
    {
      tile->type = TileType::MOUNTAIN;
    }
  },
  {
    LSI(TERRAIN, 0xA3),
    [](Tile* tile)
    {
      tile->type = TileType::FOREST;
    }
  },
  {
    LSI(TERRAIN, 0xB3),
    [](Tile* tile)
    {
      tile->type = TileType::VOLCANO;
    }
  },
  {
    LSI(TERRAIN, 0xA6),
    [](Tile* tile)
    {
      tile->type = TileType::SWAMP;
    }
  },
  {
    LSI(TERRAIN, 0xED),
    [](Tile* tile)
    {
      if (tile->type != TileType::RIVER)
        tile->type = TileType::RIVER;
      else
        tile->type = TileType::GRASS;
    }
  },
  {
    LSI(TERRAIN, 0xA8),
    [](Tile* tile)
    {

    }
  },
  {
    LSI(TERRAIN, 0xA9),
    [](Tile* tile)
    {

    }
  },
  {
    LSI(TERRAIN, 0xAA),
    [](Tile* tile)
    {

    }
  }
} };

const static std::array<Brush, 2> places = {
  {
    {
      LSI(MAPBACK, 46),
      [](Tile* tile)
      {
        //TODO: check if tile allows road
        tile->hasRoad = !tile->hasRoad;
      }
    },
    {
      LSI(MAPBACK, 55),
      [](Tile* tile)
      {
        //TODO: check if tile allows road
        if (tile->hasEnchantedRoad) { tile->hasRoad = tile->hasEnchantedRoad = false; }
        else { tile->hasRoad = tile->hasEnchantedRoad = true; }
      }
    }
  }
};

const static std::array<Resource, 11> resources = {
  Resource::ADAMANTIUM, Resource::COAL, Resource::CRYSX_CRYSTAL, Resource::GEMS,
  Resource::GOLD, Resource::IRON_ORE, Resource::MITHRIL, Resource::NIGHT_SHADE,
  Resource::QOURK_CRYSTAL, Resource::SILVER, Resource::WILD_GAME
};

MapEditorView::MapEditorView(ViewManager* gvm) : ViewWithQueue(gvm)
{
  lbx::Repository::loadLBXTerrain();
  
  size_t i = 0;
  for (auto it = brushes.begin(); it != brushes.end(); ++it)
  {
    lbx::Repository::loadLBXSpriteTerrainData(it->info);
    Point position = positionForBrush(i);
    auto* button = addButton(Button::buildSimple("", position.x, position.y, it->info));
    
    terrainButtons.push_back(button);
    button->setAction([this, it](){
      this->brush = it;
    });
    
    ++i;
  }
  
  i = 0;
  for (auto it = resources.begin(); it != resources.end(); ++it)
  {
    Point position = positionForBrush(i);
    auto* button = addButton(Button::buildSimple("", position.x, position.y, Viewport::gfxForResource(*it)));
    
    resourceButtons.push_back(button);
    button->setAction([this, it]() {
      this->resource = it;
    });
    
    ++i;
  }
  
  i = 0;
  for (auto it = places.begin(); it != places.end(); ++it)
  {
    Point position = positionForBrush(i);
    auto* button = addButton(Button::buildSimple("", position.x, position.y, it->info));
    
    placeButtons.push_back(button);
    button->setAction([this, it]() {
      this->place = it;
    });
    
    ++i;
  }
  
  std::for_each(resourceButtons.begin(), resourceButtons.end(), [](Button* button) { button->hide(); });
  std::for_each(placeButtons.begin(), placeButtons.end(), [](Button* button) { button->hide(); });

}

#include "save/OriginalSave.h"

void MapEditorView::activate()
{
  world = new World(nullptr,60,40);
  world->fill(TileType::GRASS, Plane::ARCANUS);
  world->fill(TileType::GRASS, Plane::MYRRAN);
  world->calcSubTiles();
  
	//TODO: constructor is not found in MSVC2017
#if false && !defined(_WIN32)
  Path path = "/Users/jack/Desktop/mom3x.app/Contents/Resources/mom/SAVE1.GAM";
  osave::OriginalSaveGame save(path);
  
  if (save.isLoaded())
    this->world = save.getWorld();
#endif
  
  minimap = new MiniMap(world);
  minimap->discover(Rect(0,0,60,40), Plane::ARCANUS);
  minimap->discover(Rect(0,0,60,40), Plane::MYRRAN);
  
  //WorldGenerator generator(world);
  //generator.atlasGenerate();
  
  hover = Point::INVALID;
  offset = Point::ZERO;
  plane = Plane::ARCANUS;
  
  mode = Mode::TERRAIN;
  
  brush = brushes.begin();
  resource = resources.begin();
  place = places.begin();
  
  toggleDownscale(false);
}

void MapEditorView::toggleDownscale(bool value)
{
  downscaled = value;
  
  tileSize = Size(BASE_TILE_WIDTH, BASE_TILE_HEIGHT);
  
  if (downscaled)
  {
    tileSize /= 2;
    viewportSize = Size(BASE_VIEWPORT_WIDTH*2, BASE_VIEWPORT_HEIGHT*2 - 1);
  }
  else
  {
    viewportSize = Size(BASE_VIEWPORT_WIDTH, BASE_VIEWPORT_HEIGHT);
  }
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
  if (downscaled)
  {
    Gfx::bindBuffer();
    for (size_t x = 0; x < viewportSize.w; ++x)
    {
      for (size_t y = 0; y < viewportSize.h; ++y)
      {
        Viewport::drawTile(world->get(offset.x + x, offset.y + y, plane), 0, 0, plane);
        Gfx::mergeBufferDownScaled(0, 0, OX + x*(tileSize.w+MARGIN), OY + y*(tileSize.h+MARGIN), 20, 18);
      }
    }
    Gfx::bindCanvas();
  }
  else
  {
    for (size_t x = 0; x < viewportSize.w; ++x)
    {
      for (size_t y = 0; y < viewportSize.h; ++y)
      {
        Viewport::drawTile(world->get(offset.x + x, offset.y + y, plane), OX + x*(tileSize.w+MARGIN), OY + y*(tileSize.h+MARGIN), plane);
      }
    }
  }

  if (hover.isValid())
    Gfx::rect(OX + hover.x*(tileSize.w+MARGIN)-1, OY + hover.y*(tileSize.h+MARGIN)-1, tileSize.w+MARGIN, tileSize.h+MARGIN, {255,0,0});
  
  if (mode == Mode::TERRAIN)
  {
    size_t brushIndex = std::distance(brushes.begin(), brush);
    Point brushPosition = positionForBrush(brushIndex);
    Gfx::rect(brushPosition.x - 1, brushPosition.y - 1, 21, 19, {255,0,0});
  }
  else if (mode == Mode::RESOURCES)
  {
    size_t brushIndex = std::distance(resources.begin(), resource);
    Point brushPosition = positionForBrush(brushIndex);
    Gfx::rect(brushPosition.x - 1, brushPosition.y - 1, 21, 19, {255,0,0});
  }
  else if (mode == Mode::PLACES)
  {
    size_t brushIndex = std::distance(places.begin(), place);
    Point brushPosition = positionForBrush(brushIndex);
    Gfx::rect(brushPosition.x - 1, brushPosition.y - 1, 21, 19, {255,0,0});
  }
  
  Gfx::draw(minimap->get(plane), 256, OY);
  
  if (offset.x < MAP_WIDTH - viewportSize.w)
    Gfx::rect((256 + offset.x - 1), (OY + offset.y - 1), viewportSize.w + 1, viewportSize.h + 1, {255,0,0});
  else
  {
    Gfx::rect((256 + offset.x - 1), (OY + offset.y - 1), MAP_WIDTH - offset.x + 1, viewportSize.h + 1, {255,0,0});
    Gfx::rect((256 - 1), (OY + offset.y - 1), viewportSize.w - (MAP_WIDTH - offset.x) + 1, viewportSize.h + 1, {255,0,0});
  }

  
  const s16 DEBUG_X = 2;
  const s16 DEBUG_Y = HEIGHT - 9;
  const FontSpriteSheet* font = FontFaces::Tiny::WHITE;
  Fonts::setFace(font);
  if (hover.isValid())
  {
    const Tile* tile = world->get(hover.x, hover.y, plane);
    if (tile)
    {
      Fonts::drawString(Fonts::format("join: %02X, variant: %u, river: %02X, roads: %02X", tile->gfx.joinMask, tile->gfx.variant, tile->gfx.riverMask, tile->gfx.roadMask), DEBUG_X, DEBUG_Y, ALIGN_LEFT);
    }
  }
  
}

void MapEditorView::setup()
{

}

Point MapEditorView::hoveredTile(Point pt)
{
  u16 tx = (pt.x - OX) / (tileSize.w+MARGIN);
  u16 ty = (pt.y - OY) / (tileSize.h+MARGIN);
  
  if (tx >= 0 && tx < viewportSize.w && ty >= 0 && ty < viewportSize.h)
    return Point(tx, ty);
  else
    return Point::INVALID;
}

void MapEditorView::clickOnTile(Point coords)
{
  Tile* tile = world->get(coords.x, coords.y, plane);
  if (tile)
  {
    if (mode == Mode::TERRAIN)
      brush->lambda(tile);
    else if (mode == Mode::RESOURCES)
      tile->resource = *resource;
    else if (mode == Mode::PLACES)
      place->lambda(tile);
    
    world->calcSubTile(tile->x(), tile->y(), plane);
    tile->for_each_neighbor([this](Tile* t) { if (t) world->calcSubTile(t->x(), t->y(), plane); });
    
    minimap->discover(Position(tile->x(), tile->y(), plane));
    tile->for_each_neighbor([this](const Tile* neighbor) {
      if (neighbor)
        minimap->discover(neighbor->position);
    });
  }
}

void MapEditorView::switchMode(Mode mode)
{
  this->mode = mode;
  
  if (mode == Mode::TERRAIN)
  {
    std::for_each(terrainButtons.begin(), terrainButtons.end(), [](Button* bt) { bt->show(); });
    std::for_each(resourceButtons.begin(), resourceButtons.end(), [](Button* bt) { bt->hide(); });
    std::for_each(placeButtons.begin(), placeButtons.end(), [](Button* bt) { bt->hide(); });

  }
  else if (mode == Mode::RESOURCES)
  {
    std::for_each(terrainButtons.begin(), terrainButtons.end(), [](Button* bt) { bt->hide(); });
    std::for_each(resourceButtons.begin(), resourceButtons.end(), [](Button* bt) { bt->show(); });
    std::for_each(placeButtons.begin(), placeButtons.end(), [](Button* bt) { bt->hide(); });
  }
  else if (mode == Mode::PLACES)
  {
    std::for_each(terrainButtons.begin(), terrainButtons.end(), [](Button* bt) { bt->hide(); });
    std::for_each(resourceButtons.begin(), resourceButtons.end(), [](Button* bt) { bt->hide(); });
    std::for_each(placeButtons.begin(), placeButtons.end(), [](Button* bt) { bt->show(); });

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
      offset = offset + h - Point(viewportSize.w/2, viewportSize.h/2);
      offset.y = std::max((s16)0, offset.y);
      offset.y = std::min((s16)(world->h - viewportSize.h), offset.y);
      
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
  if (oldHover != hover && hover.isValid() && b == MouseButton::BUTTON_LEFT)
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
      
    case Mode::RESOURCES:
    {
      if (dy < 0 && resource < resources.end()-1)
        ++resource;
      else if (dy > 0 && resource > resources.begin())
        --resource;
      
      break;
    }
      
    case Mode::PLACES:
    {
      if (dy < 0 && place < places.end()-1)
        ++place;
      else if (dy > 0 && place > places.begin())
        --place;
    }
  }

  return true;
}

bool MapEditorView::keyPressed(KeyboardCode key, KeyboardKey kkey, KeyboardMod mod)
{
  switch (key)
  {
    case SDL_SCANCODE_P:
    {
      plane = plane == Plane::ARCANUS ? Plane::MYRRAN : Plane::ARCANUS;
      hover = Point::INVALID;
      break;
    }
    case SDL_SCANCODE_S:
    {
      toggleDownscale(!downscaled);
      hover = Point::INVALID;
    }
    case SDL_SCANCODE_1:
    {
      switchMode(Mode::TERRAIN);
      break;
    }
    case SDL_SCANCODE_2:
    {
      switchMode(Mode::RESOURCES);
      break;
    }
    case SDL_SCANCODE_3:
    {
      switchMode(Mode::PLACES);
      break;
    }
    
    default: break;
  }
  
  return true;
}
