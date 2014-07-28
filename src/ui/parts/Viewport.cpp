#include "Viewport.h"

#include "Texture.h"
#include "Tile.h"
#include "Gfx.h"
#include "GfxData.h"
#include "UnitDraw.h"
#include "LocalPlayer.h"
#include "World.h"
#include "ColorMap.h"
#include "City.h"
#include "Army.h"
#include "Util.h"
#include "Game.h"

std::unordered_map<u8,u8> Viewport::waterMap = {
  {0, 0},
  {255, 2},
  // corners
  {255-2-4-8-16-32,6},
  {255-2-4-8-16-32-128,6},
  {255-4-8-16-32,7},
  {255-4-8-16-32-128,7},
  {255-2-4-8-16,8},
  {255-2-4-8-16-128,8},
  {255-4-8-16,9},
  {255-4-8-16-128,9},
  {255-32-64-128-1-2,10},
  {255-32-64-128-1-2-8,10},
  {255-64-128-1-2,11},
  {255-64-128-1-2-8,11},
  {255-32-64-128-1,12},
  {255-32-64-128-1-8,12},
  {255-64-128-1,13},
  {255-64-128-1-8,13},
  {255-8-16-32-64-128,14},
  {255-8-16-32-64-128-2,14},
  {255-16-32-64-128,15},
  {255-16-32-64-128-2,15},
  {255-8-16-32-64,16},
  {255-8-16-32-64-2,16},
  {255-16-32-64,17},
  {255-16-32-64-2,17},
  {255-128-1-2-4-8,18},
  {255-128-1-2-4-8-32,18},
  {255-128-1-2-4,19},
  {255-128-1-2-4-32,19},
  {255-1-2-4-8,20},
  {255-1-2-4-8-32,20},
  {255-1-2-4,21},
  {255-1-2-4-32,21}
};

static TextureID planeTextures[][8] = {
  {TextureID::ARCANUS_SHORE, TextureID::ARCANUS_ANIMATED, TextureID::ARCANUS_DESERT, TextureID::ARCANUS_TUNDRA, TextureID::ARCANUS_TILES, TextureID::ARCANUS_RIVERS, TextureID::ARCANUS_MOUNTAINS, TextureID::ARCANUS_NODES},
  {TextureID::MYRRAN_SHORE, TextureID::MYRRAN_ANIMATED, TextureID::MYRRAN_DESERT, TextureID::MYRRAN_TUNDRA, TextureID::MYRRAN_TILES, TextureID::MYRRAN_RIVERS, TextureID::MYRRAN_MOUNTAINS, TextureID::MYRRAN_NODES},
};

enum TileSheet : u8 {
  SHORE = 0,
  ANIMATED,
  DESERT,
  TUNDRA,
  TILES,
  RIVERS,
  MOUNTAINS,
  NODES
};


void Viewport::drawTile(const Tile* t, u16 x, s16 y, Plane plane)
{
  TextureID* tx = planeTextures[plane];
  
  if (t->node)
  {
    u16 index = 0;
    if (t->node->school == NATURE) index = 1;
    else if (t->node->school == CHAOS) index = 2;
    Gfx::drawAnimated(tx[NODES], index, x, y, t->animationOffset);
  }
  else
  {
    if (t->type == TILE_WATER || t->type == TILE_SHORE)
    {
      if (waterMap.find(t->subtype) != waterMap.end() && (t->subtype != 0 || t->tileGfxType == TILE_GFX_ANIMATED))
        Gfx::drawAnimated(tx[ANIMATED], waterMap[t->subtype], x, y, t->animationOffset);
      /*else if (waterMap.contains(t->subtype))
       TextureID::drawAnimated(animated, waterMap.get(t->subtype), x, y, t->animationOffset());*/
      else if (t->tileGfxType == TILE_GFX_BORDER)
        Gfx::draw(tx[SHORE], t->subtype/10, t->subtype%10, x, y);
      else
        Gfx::draw(tx[TILES], 5, 0, x, y);
    }
    else if (t->type == TILE_RIVER)
      Gfx::draw(tx[RIVERS], t->subtype/8, t->subtype%8, x, y);
    else if (t->type == TILE_DESERT)
      Gfx::draw(tx[DESERT], t->subtype/10, t->subtype%10, x, y);
    else if (t->type == TILE_TUNDRA)
    {
      // the sprite doesn't have borders with other types, then a random sprite of basic terrain is used
      if (t->tileGfxType == TILE_GFX_PLAIN)
        Gfx::draw(tx[TILES], GfxData::tileGfxSpec(t->type).row, t->subtype, x, y);
      else
        Gfx::draw(tx[TUNDRA], t->subtype/10, t->subtype%10, x, y);
    }
    else if (t->type == TILE_MOUNTAIN)
      Gfx::draw(tx[MOUNTAINS], 0, t->subtype, x, y);
    else if (t->type == TILE_VOLCANO)
      Gfx::drawAnimated(tx[ANIMATED], 1, x, y, t->animationOffset);
    else if (t->type == TILE_HILL)
      Gfx::draw(tx[MOUNTAINS], 1, t->subtype, x, y);
    else if (t->type == TILE_GRASS || t->type == TILE_SWAMP || t->type == TILE_FOREST || t->type == TILE_DESERT || t->type == TILE_TUNDRA)
    {
      Gfx::draw(tx[TILES], GfxData::tileGfxSpec(t->type).row, t->subtype, x, y);
    }
  }
  
  if (t->resource != Resource::NONE)
    Gfx::draw(TextureID::TILE_RESOURCES, 0, static_cast<u8>(t->resource), x, y);
  
  if (t->place)
    Gfx::draw(TextureID::TILE_PLACES, 0, static_cast<u8>(t->place->type), x, y);
  
  if (t->isCorrupted())
    Gfx::draw(TextureID::TILE_PLACES, 1, 0, x, y);
  
  if (t->hasRoad || t->city)
  {
    if (t->roads == 0)
      Gfx::draw(TextureID::TILE_ROADS, 0, 0, x, y);
    else
      for (int i = 0; i < 8; ++i)
        if ((t->roads & (1<<i)) == 1<<i)
        {
          if (t->hasEnchantedRoad)
            Gfx::drawAnimated(TextureID::TILE_ROADS_ENCHANTED, 1+i, x, y, 0/*t->animationOffset()*/);
          else
            Gfx::draw(TextureID::TILE_ROADS, 0, 1+i, x, y);
        }
  }
  
  //if (LocalGame.i.currentPlayer.mapGridEnabled)
  //	Gfx::draw(TextureID::TILE_FOG, 0, 9, x, y);
}


Position Viewport::tileCoords(const LocalPlayer* player, s16 x, s16 y)
{
  const Position pos = player->getViewport();
  s16 dx = x - (pos.x - viewportW/2);
  s16 dy = y - (pos.y - viewportH/2);
  return Position(dx, dy, pos.plane);
}

bool Viewport::isOutsideViewport(const LocalPlayer* player, s16 x, s16 y)
{
  const Position pos = player->getViewport();
  s16 dx = pos.x - viewportW/2;
  s16 dy = pos.y - viewportH/2;
  
  // TODO: wrapping
  return x < dx || x >= dx+viewportW || y < dy || y >= dy+viewportH;
}

void Viewport::drawViewport(const World* map, const LocalPlayer* player, const Position& p, s16 ttx, s16 tty, s16 w, s16 h, bool darkenEdges)
{
  s16 vx = baseOffsetX(p.x, w), vy = baseOffsetY(p.y, h);
  Plane plane = p.plane;

  s16 sx = ttx, sy = tty;
  for (s16 y = 0; y < h; ++y)
  {
    for (s16 x = 0; x < w; ++x)
    {
      Position op = Position(vx+x, vy+y, p.plane);
      
      Tile* t = map->get(op);
      if (t)
      {
        if (player->fog()->get(op))
          drawTile(t, sx, sy, p.plane);
        
        sx += tileWidth;
      }
    }
    sy += tileHeight;
    sx = ttx;
  }
  
  /* draw thing on the map: cities, which goes over the boundaries of the sprite, mana node auras, pathfinding infos .. */
  sx = ttx; sy = tty;
  for (s16 y = 0; y < h; ++y)
  {
    for (s16 x = 0; x < w; ++x)
    {
      Position op = Position(vx+x, vy+y, p.plane);
      Tile* t = map->get(op);
            
      if (t)
      {
        if (player->fog()->get(op))
        {
          /* draw city */
          if (t->city)
          {
            Gfx::bindColorMap(&MiscMaps::FLAG_COLORS_MAP[t->city->getOwner()->color]);
            
            if (t->city->hasBuilding(Building::CITY_WALLS))
              Gfx::draw(TextureID::CITY_MAP_TILE, 0, t->city->tileSize(), sx - 8, sy - 6);
            else
              Gfx::draw(TextureID::CITY_MAP_TILE, 1, t->city->tileSize(), sx - 8, sy - 6);
            
            Gfx::unbindColorMap();
          }
          
          /* draw node auras */
          if (t->node && t->node->owner)
          {
            Gfx::drawAnimated(TextureID::TILE_NODE_AURAS, t->node->owner->color, sx, sy, t->animationOffset);
            for (auto aura : t->node->auras)
            {
              int tx = x + aura.x;
              int ty = y + aura.y;
              int animationOffset = map->get(tx, ty, plane)->animationOffset;
              if (tx > 0 && tx < w && ty > 0 && ty < h)
              {
                tx = sx + aura.x*tileWidth;
                ty = sy + aura.y*tileHeight;
                Gfx::drawAnimated(TextureID::TILE_NODE_AURAS, t->node->owner->color, tx, ty, animationOffset);
              }
            }
          }
          
          /* TODO: simplify checks? */
          if (t->army && !t->army->isPatrolling() && (t->army != player->getSelectedArmy() /* TODO || LocalGame.i.currentPlayer.drawSelectedArmy*/))
            UnitDraw::drawStatic(t->army, sx, sy);
          else if (t->army && t->army->isPatrolling() && !t->city)
            UnitDraw::drawStatic(t->army, sx, sy);
          
          if (darkenEdges && ((y == 0 || y == h-1) && (x == 0 || x == w - 1)))
            Gfx::draw(TextureID::TILE_FOG, 0, 8, sx, sy);
          
          // draw the fog sprites
          // TODO: if it is needed to save CPU just don't do it dinamically but with a tileMap
          for (int i = 0; i < Util::DIRS_LENGTH; ++i)
          {
            Position od = Position(vx+x+Util::DIRS[i].x,vy+y+Util::DIRS[i].y, p.plane);
            /*int xd = vx+x+Util::DIRS[i].x;
            int yd = vy+y+Util::DIRS[i].y;*/
            
            if (i % 2 == 0)
            {
              if (!player->fog()->get(od))
                Gfx::draw(TextureID::TILE_FOG, 0, i, sx, sy);
            }
            else // TODO: check if this else goes with the correct if
            {
              Position od2 = Position(vx+x+Util::DIRS[(i+1)%8].x, vx+x+Util::DIRS[(i+1)%8].y, plane);
              Position od3 = Position(vx+x+Util::DIRS[i-1 < 0 ? 8 + (i-1) : i-1].x, vx+x+Util::DIRS[i-1 < 0 ? 8 + (i-1) : i-1].y, plane);
              
              if (!player->fog()->get(od) && player->fog()->get(od2) && player->fog()->get(od3))
                Gfx::draw(TextureID::TILE_FOG, 0, i, sx, sy);
            }
          }
        }
      }
      
      // TODO draw route info on map
      /*if (player->selectedRoute() != null)
      {
        Route r = player.selectedRoute();
        
        if (r != null && r.has((vx+x)%map.width(),vy+y))
        {
          Gfx::canvas.noFill();
          Gfx::canvas.stroke(Gfx::color(255, 0, 0));
          Gfx::canvas.rect(sx+10-3, sy+9-3, 5, 5);
        }
      }*/
      
      sx += tileWidth;
    }
    sy += tileHeight;
    sx = ttx;
  }
}

void Viewport::drawMainViewport(const LocalPlayer* player, const World* map)
{
  drawViewport(map, player, player->getViewport(), baseX, baseY, viewportW, viewportH, false);
}

void Viewport::drawMicroMap(const LocalPlayer* player, s16 dx, s16 dy, s16 w, s16 h, s16 vx, s16 vy, Plane plane)
{
  s16 lw = w/2, rw = w - lw;
  s16 uh = h/2, lh = h - uh;
  
  s16 ww = player->game()->world->w;
  s16 wh = player->game()->world->h;
  
  s16 fx1, fx2 = -1, fy;
  s16 fw1, fw2 = -1, fh;
  s16 dx1 = dx, dx2 = -1;
  
  // maps upper edge starts before frame edge, clip minimap
  if (vy > uh)
  {
    fy = (vy - uh);
    fh = std::min(static_cast<s16>(wh - (vy - uh)), h);
  }
  else
  {
    fy = 0;
    dy += uh - vy;
    fh = std::min(h, static_cast<s16>(h - (uh - vy )));
  }
  
  if (vx > lw)
  {
    fx1 = (vx - lw);
    fw1 = std::min(static_cast<s16>(ww - (vx - lw)), w);
    
    // wrap map to the right
    dx2 = dx + fw1;
    fw2 = w - fw1;
    fx2 = 0;
  }
  else
  {
    fx1 = 0;
    dx1 += lw - vx;
    fw1 = std::min(w, static_cast<s16>(w - (lw - vx)));
    
    // wrap map to the left
    dx2 = dx;
    fw2 = w - fw1;
    fx2 = ww - fw2;
  }
  
  Gfx::canvasBlit(player->miniMap()->get(plane), fx1, fy, dx1, dy, fw1, fh);
  
  if (dx2 != -1)
    Gfx::canvasBlit(player->miniMap()->get(plane), fx2, fy, dx2, dy, fw2, fh);
}

const Position Viewport::hoveredPosition(const World* world, const LocalPlayer* player, u16 x, u16 y)
{
  const Position vp = player->getViewport();
  
  if (x > 0 && x < tileWidth*viewportW && y >= 20 && y <= (20 + tileWidth)*viewportH)
  {
    int cx = (x / 20) + baseOffsetX(vp.x, viewportW);
    int cy = ((y - 20) / 18) + baseOffsetY(vp.y, viewportH);
    
    return Position(Util::wrap(cx, world->w), cy, vp.plane);
  }
  else
    return Position(-1,-1,ARCANUS);
}