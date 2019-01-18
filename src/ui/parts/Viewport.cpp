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
#include "common/Util.h"
#include "Game.h"
#include "GUISettings.h"

#include "Game.h"

enum lbx_indices
{
  city_sprite_with_walls = LBXI(MAPBACK, 20),
  city_sprite_no_walls = LBXI(MAPBACK, 21),
  
  road_none = LBXI(MAPBACK,45),
  road_north = LBXI(MAPBACK,46),
  road_north_west = LBXI(MAPBACK,47),
  road_west = LBXI(MAPBACK,48),
  road_south_west = LBXI(MAPBACK,49),
  road_south = LBXI(MAPBACK,50),
  road_south_east = LBXI(MAPBACK,51),
  road_east = LBXI(MAPBACK,52),
  road_north_east = LBXI(MAPBACK,53),
  
  road_ench_none = LBXI(MAPBACK,54),
  road_ench_north = LBXI(MAPBACK,55),
  road_ench_north_west = LBXI(MAPBACK,56),
  road_ench_west = LBXI(MAPBACK,57),
  road_ench_south_west = LBXI(MAPBACK,58),
  road_ench_south = LBXI(MAPBACK,59),
  road_ench_south_east = LBXI(MAPBACK,60),
  road_ench_east = LBXI(MAPBACK,61),
  road_ench_north_east = LBXI(MAPBACK,62),
  
  place_tower_unowned = LBXI(MAPBACK, 69),
  place_tower_owned = LBXI(MAPBACK, 70),
  place_mound = LBXI(MAPBACK, 71),
  place_temple = LBXI(MAPBACK, 72),
  place_keep = LBXI(MAPBACK, 73),
  place_ruins = LBXI(MAPBACK, 74),
  place_fallen_temple = LBXI(MAPBACK, 75),
  place_mud = LBXI(MAPBACK, 76),
  
  place_corruption = LBXI(MAPBACK, 77), 
  
  resource_iron = LBXI(MAPBACK, 78),
  resource_coal = LBXI(MAPBACK, 79),
  resource_silver = LBXI(MAPBACK, 80),
  resource_gold = LBXI(MAPBACK, 81),
  resource_gems = LBXI(MAPBACK, 82),
  resource_mithril = LBXI(MAPBACK, 83),
  resource_adamantium = LBXI(MAPBACK, 84),
  resource_qourk = LBXI(MAPBACK, 85),
  resource_crysx = LBXI(MAPBACK, 86),
  resource_nightshade = LBXI(MAPBACK, 91),
  resource_wild_game = LBXI(MAPBACK, 92),
  
  fog_line_ne = LBXI(MAPBACK, 0),
  fog_line_se = LBXI(MAPBACK, 1),
  fog_line_e = LBXI(MAPBACK, 2),
  fog_line_sw = LBXI(MAPBACK, 3),
  fog_line_s = LBXI(MAPBACK, 5),
  fog_line_nw = LBXI(MAPBACK, 7),
  fog_line_n = LBXI(MAPBACK, 8),
  fog_line_w = LBXI(MAPBACK, 11),
  
  fog_corner_nw_se = LBXI(MAPBACK, 4),
  fog_corner_sw_ne = LBXI(MAPBACK, 9),
  fog_corner_se = LBXI(MAPBACK, 6),
  fog_corner_ne = LBXI(MAPBACK, 10),
  fog_corner_nw = LBXI(MAPBACK, 12),
  fog_corner_sw = LBXI(MAPBACK, 13)
};

const static sprite_ref roads[] = { road_none, road_north, road_north_east, road_east, road_south_east, road_south, road_south_west, road_west, road_north_west };
const static sprite_ref roads_ench[] = { road_ench_none, road_ench_north, road_ench_north_east, road_ench_east, road_ench_south_east, road_ench_south, road_ench_south_west, road_ench_west, road_ench_north_west };

SpriteInfo Viewport::gfxForPlace(const Place* place)
{
  switch (place->type)
  {
    //TODO: there is 1 pixel shift compared to other tower gfx
    case PlaceType::TOWER_OF_WIZARDRY:
      return place->cleared ? place_tower_owned : place_tower_unowned;
      
    case PlaceType::MYSTERIOUS_CAVE: return place_mound;
    case PlaceType::RUINS: return place_ruins;
    case PlaceType::ABANDONED_KEEP: return place_keep;
    case PlaceType::DUNGEON: return place_ruins;
    case PlaceType::ANCIENT_TEMPLE: return place_temple;
    case PlaceType::MONSTER_LAIR: return place_mound;
    case PlaceType::FALLEN_TEMPLE: return place_fallen_temple;
      
    case PlaceType::MANA_NODE: assert(false); /* should never end up here */
  }
}

SpriteInfo Viewport::gfxForResource(Resource resource)
{
  switch (resource)
  {
    case Resource::ADAMANTIUM: return resource_adamantium;
    case Resource::COAL: return resource_coal;
    case Resource::CRYSX_CRYSTAL: return resource_crysx;
    case Resource::GEMS: return resource_gems;
    case Resource::GOLD: return resource_gold;
    case Resource::IRON_ORE: return resource_iron;
    case Resource::MITHRIL: return resource_mithril;
    case Resource::NIGHT_SHADE: return resource_nightshade;
    case Resource::QOURK_CRYSTAL: return resource_qourk;
    case Resource::SILVER: return resource_silver;
    case Resource::WILD_GAME: return resource_wild_game;
    default: assert(false);
  }
}

SpriteInfo Viewport::gfxForTerrain(const Tile* t)
{
  const auto& mapping = t->plane() == Plane::ARCANUS ? arcanus : myrran;
  const auto& gfx = t->gfx;

  if (t->node())
  {
    switch (t->node()->school)
    {
      case School::NATURE: return mapping.manaNodes.nature;
      case School::CHAOS: return mapping.manaNodes.chaos;
      case School::SORCERY: return mapping.manaNodes.sorcery;
      default: assert(false);
    }
  }
  else
  {
    switch (t->type)
    {
      case TileType::OCEAN: return mapping.ocean[gfx.variant];
      case TileType::SHORE:
      {
        if (t->gfx.riverMask == DirJoin::NONE)
          return mapping.shores[gfx.joinMask];
        else
          return mapping.riverMouths.spriteForRiverAndJoinMask(t->gfx.riverMask, gfx.joinMask);
      }
        
      case TileType::GRASS: return mapping.grasslands[gfx.variant];

      case TileType::DESERT: return gfx.joinMask == DirJoin::NONE ? mapping.desert[gfx.variant] : mapping.desertJoin[gfx.joinMask];
      case TileType::TUNDRA: return gfx.joinMask == DirJoin::NONE ? mapping.tundra[gfx.variant] : mapping.tundraJoin[gfx.joinMask];

      case TileType::MOUNTAIN:
      case TileType::HILL:
      {
        const auto& tiles = t->type == TileType::MOUNTAIN ? mapping.mountains : mapping.hills;
        return tiles[gfx.variant];
      }
        
      case TileType::FOREST: return mapping.forest[gfx.variant];
      case TileType::SWAMP: return mapping.swamp[gfx.variant];
        
      case TileType::VOLCANO: return mapping.volcano;

      case TileType::RIVER: return mapping.rivers.spriteForMask(gfx.riverMask, gfx.variant);
    }
  }
  
  assert(false);
}

void Viewport::drawTile(const Tile* t, u16 x, s16 y, Plane plane)
{
  constexpr u16 animSpeed = 2;
  
  const auto& gfx = t->gfx;
  
  const SpriteInfo terrain = gfxForTerrain(t);
  Gfx::drawAnimated(terrain, x, y, gfx.animationOffset, animSpeed);
  
  if (t->resource != Resource::NONE)
    Gfx::draw(gfxForResource(t->resource), x, y);
  
  //TODO: raw pointer
  if (t->place() && !t->place()->isNode())
    Gfx::draw(gfxForPlace(t->place().get()), x, y);
  
  if (t->isCorrupted())
    Gfx::draw(place_corruption, x, y);
  
  if (t->hasRoad || t->city)
  {
    const auto& roadMask = t->gfx.roadMask;
    if (roadMask == DirJoin::NONE)
      Gfx::drawAnimated(t->hasEnchantedRoad ? roads_ench[0] : roads[0], x, y, gfx.animationOffset, animSpeed);
    else
    {
      DirJoin d = DirJoin::N;
      for (int i = 0; i < 8; ++i)
      {
        if ((roadMask & d) != DirJoin::NONE)
          Gfx::drawAnimated(t->hasEnchantedRoad ? roads_ench[i+1] : roads[i+1], x, y, gfx.animationOffset, animSpeed);
        d >>= 1;
      }
      
    }

  }
}

Point Viewport::screenCoordsForTile(const LocalPlayer* player, Position p)
{
  if (!isOutsideViewport(player, p.x, p.y))
  {
    const Position v = player->getViewport();
    s16 dx = p.x - (v.x - viewportW/2);
    s16 dy = p.y - (v.y - viewportH/2);
    
    return Point(baseX + dx*tileWidth, baseY + dy*tileHeight);
  }
  
  return Point::INVALID;
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
  
  if (GUISettings::drawOverlandGrid)
  {
    for (s16 y = 1; y < h; ++y)
    {
      Gfx::drawLine({0, 0, 0, 40}, ttx, tty + y*tileHeight, ttx + w*tileWidth, tty + y*tileHeight);
      Gfx::drawLine({0, 0, 0, 40}, ttx, tty + y*tileHeight - 1, ttx + w*tileWidth, tty + y*tileHeight - 1);
    }

    
    for (s16 x = 1; x < w; ++x)
    {
      Gfx::drawLine({0, 0, 0, 40}, ttx + x*tileWidth, tty, ttx + x*tileWidth, tty + h*tileHeight);
      Gfx::drawLine({0, 0, 0, 40}, ttx + x*tileWidth - 1, tty, ttx + x*tileWidth - 1, tty + h*tileHeight);
    }

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
          const City* city = t->city;
          /* draw city */
          if (city)
          {
            Gfx::bindColorMap(&MiscMaps::FLAG_COLORS_MAP[city->getOwner()->color]);
            SpriteInfo citySprite = LBXU(city->hasBuilding(Building::CITY_WALLS) ? city_sprite_with_walls : city_sprite_no_walls);
            Gfx::draw(citySprite.frame(city->tileSize()), sx - 8, sy - 6);
            Gfx::unbindColorMap();
          }
          
          /* draw node auras */
          if (t->node() && t->node()->owner)
          {
            auto& auraGfx = GfxData::playerGfxSpec(t->node()->owner->color).nodeAura;
            Gfx::drawAnimated(auraGfx, sx, sy, t->gfx.animationOffset);
            for (auto aura : t->node()->auras)
            {
              int tx = x + aura.x;
              int ty = y + aura.y;
              int animationOffset = map->get(tx, ty, plane)->gfx.animationOffset;
              if (tx > 0 && tx < w && ty > 0 && ty < h)
              {
                tx = sx + aura.x*tileWidth;
                ty = sy + aura.y*tileHeight;
                Gfx::drawAnimated(auraGfx, tx, ty, animationOffset);
              }
            }
          }
          
          /* TODO: simplify checks? */
          if (t->army && !t->army->isPatrolling() && (t->army != player->getSelectedArmy() || LocalGame::i->currentPlayer()->shouldDrawSelectedArmy()))
            UnitDraw::drawStatic(t->army, sx, sy);
          else if (t->army && t->army->isPatrolling() && !t->city)
            UnitDraw::drawStatic(t->army, sx, sy);
          
          // draw the fog sprites
          // TODO: if it is needed to save CPU just don't do it dinamically but with a tileMap
          DirMask fogMask = t->computeMask([player](const Tile* tile) {
            return !tile || !player->fog()->get(tile->position);
          });
          fogMask = fogMask.pruneDiagonalIfIsolated();
          
          for (DirJoin f : fogMask)
          {
            switch (f) {
              case DirJoin::N: Gfx::draw(fog_line_n, sx, sy); break;
              case DirJoin::S: Gfx::draw(fog_line_s, sx, sy); break;
              case DirJoin::W: Gfx::draw(fog_line_w, sx, sy); break;
              case DirJoin::E: Gfx::draw(fog_line_e, sx, sy); break;
              case DirJoin::NW: Gfx::draw(fog_corner_nw, sx, sy); break;
              case DirJoin::SW: Gfx::draw(fog_corner_sw, sx, sy); break;
              case DirJoin::NE: Gfx::draw(fog_corner_ne, sx, sy); break;
              case DirJoin::SE: Gfx::draw(fog_corner_se, sx, sy); break;
            }
          }
          
          if (darkenEdges && ((y == 0 || y == h-1) && (x == 0 || x == w - 1)))
            Gfx::fillRect(sx, sy, tileWidth, tileHeight, Color(0, 0, 0, 160));
          else
          {
            if (t->isResourceShared() && darkenEdges)
            {
              const SpriteInfo half = LSI(BACKGRND, 0);
              Gfx::draw(half, sx + tileWidth/2 - half.sw()/2, sy + tileHeight/2 - half.sh()/2);
            }
          }
        }
      }
      
      // TODO should this be here on in MainView?
      const auto& route = player->getRoute();
      if (route)
      {
        if (route->passesBy(t->position))
        {
          Gfx::rect(sx+10-3, sy+9-3, 5, 5, {255,0,0});
        }
      }
      

      
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

void Viewport::drawMicroMap(const LocalPlayer* player, s16 dx, s16 dy, s16 w, s16 h, s16 vx, s16 vy, Plane plane, Color fillColor)
{
  Gfx::fillRect(dx, dy, w, h, fillColor);
  drawMicroMap(player, dx, dy, w, h, vx, vy, plane);
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

#pragma Original Graphics Mapping

#include <bitset>
#include "LBXRepository.h"

const size_t TILE_COUNT = 1524;
const size_t TILE_PER_PLANE = 0x2FA;
constexpr size_t TILE_WIDTH = 20;
constexpr size_t TILE_HEIGHT = 18;

TileToSpriteMap Viewport::arcanus;
TileToSpriteMap Viewport::myrran;

std::bitset<TILE_COUNT> used;

// Repository::spriteFor
inline gfx_tile_t spriteInfoToGfxTile(SpriteInfo info) { return info; }
inline const SpriteSheet* gfxTileToSprite(SpriteInfo info) { return lbx::Repository::spriteFor(info); }


void mapSprite(size_t gfxIndex, gfx_tile_t& dest)
{
  used[gfxIndex] = true;
  dest = spriteInfoToGfxTile(LSI(TERRAIN, gfxIndex));
}

/* map a specific tile index to destination for both planes */
void mapSprite(size_t gfxIndex, gfx_tile_t& arcanusDest, gfx_tile_t& myrranDest)
{
  mapSprite(gfxIndex, arcanusDest);
  mapSprite(gfxIndex+TILE_PER_PLANE, myrranDest);
}

/* find matching index in source and use its position to map destIndex to both planes */
template<size_t SIZE> void mapRiverSprite(const std::array<size_t,SIZE> srcIndices, const std::array<size_t,SIZE>& destIndices, gfx_tile_mapping<256>& source, gfx_tile_mapping<256>& arcanus, gfx_tile_mapping<256>& myrran)
{
  for (size_t i = 0; i < srcIndices.size(); ++i)
  {
    const auto srcIndex = srcIndices[i];
    for (size_t j = 0; j < source.size(); ++j)
    {
      if (source[j].index() == srcIndex)
      {
        mapSprite(destIndices[i], arcanus[j]);
        mapSprite(destIndices[i] + TILE_PER_PLANE, myrran[j]);
      }
    }
  }
}

template<size_t SIZE> void mapRiverSprite(const std::array<DirJoin,SIZE> srcIndices, const std::array<size_t,SIZE>& destIndices, gfx_tile_mapping<256>& source, gfx_tile_mapping<256>& arcanus, gfx_tile_mapping<256>& myrran)
{
  std::array<size_t, SIZE> mapped;
  std::transform(srcIndices.begin(), srcIndices.end(), mapped.begin(), [&source](DirJoin dj) { return source[(size_t)dj].index(); });
  mapRiverSprite(mapped, destIndices, source, arcanus, myrran);
}

/* map a serie of indices for both planes */
template<size_t SIZE> void mapSprite(const std::array<size_t, SIZE>& indices, gfx_tile_mapping<SIZE>& arcanus, gfx_tile_mapping<SIZE>& myrran)
{
  for (size_t i = 0; i < SIZE; ++i)
  {
    mapSprite(indices[i], arcanus[i]);
    mapSprite(indices[i]+TILE_PER_PLANE, myrran[i]);
  }
}

template<size_t SIZE1, size_t SIZE2> void mapSprite(const std::array<size_t, SIZE1*SIZE2>& indices, std::array<gfx_tile_mapping<SIZE1>, SIZE2>& arcanus, std::array<gfx_tile_mapping<SIZE1>, SIZE2>& myrran)
{
  for (size_t j = 0; j < SIZE2; ++j)
  {
    for (size_t i = 0; i < SIZE1; ++i)
    {
      mapSprite(indices[i + j*SIZE1], arcanus[j][i]);
      mapSprite(indices[i + j*SIZE1]+TILE_PER_PLANE, myrran[j][i]);
    }
  }
}

void blitTileToAtlas(size_t tileIndex, size_t atlasIndex, size_t atlasWidth, SDL_Surface* atlas)
{
  using namespace lbx;
  
  const LBXSpriteData* sprite = Repository::spriteFor(LSI(TERRAIN, tileIndex));
  const size_t bx = 20 * (atlasIndex % atlasWidth);
  const size_t by = 18 * (atlasIndex / atlasWidth);
  
  for (coord_t x = 0; x < TILE_WIDTH; ++x)
  {
    for (coord_t y = 0; y < TILE_HEIGHT; ++y)
    {
      Color* pixel = reinterpret_cast<Color*>(atlas->pixels) + bx + x + (by + y)*atlas->w;
      *pixel = sprite->getPalette()->get(sprite->at(x, y, 0, 0));
    }
  }
}

void blitTileToAtlas(gfx_tile_t info, size_t xx, size_t yy, SDL_Surface* atlas)
{
  using namespace lbx;
  
  const size_t bx = 20 * xx;
  const size_t by = 18 * yy;
  
  const SpriteSheet* sprite = gfxTileToSprite(info);
  
  for (coord_t x = 0; x < TILE_WIDTH; ++x)
  {
    for (coord_t y = 0; y < TILE_HEIGHT; ++y)
    {
      Color* pixel = reinterpret_cast<Color*>(atlas->pixels) + bx + x + (by + y)*atlas->w;
      *pixel = sprite->getPalette()->get(sprite->at(x, y, 0, 0));
    }
  }
}

gfx_tile_mapping<256> createJoiningTileTextureAtlas8dirs(size_t gfxOffset, size_t arrayIndex, Plane plane, const char* fileName)
{
  using namespace lbx;
  constexpr size_t ATLAS_WIDTH = 8;
  constexpr size_t ATLAS_HEIGHT = 32;
  
  SDL_Surface* atlas = nullptr;
  
  if (fileName)
    atlas = SDL_CreateRGBSurface(0, ATLAS_WIDTH*TILE_WIDTH, ATLAS_HEIGHT*TILE_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
  
  const LBXArrayData* mapping = Repository::arrayFor(LBXID::TERRTYPE, arrayIndex);
  const size_t delta = plane == Plane::ARCANUS ? 0 : TILE_PER_PLANE;
  
  gfx_tile_mapping<256> result;
  
  for (size_t i = 1; i < 256; ++i)
  {
    u16 gfxIndex = mapping->get<u16>(i);
    u16 adjustedIndex = gfxIndex + delta + gfxOffset;
    
    result[i] = spriteInfoToGfxTile(LSI(TERRAIN, adjustedIndex));
    used[adjustedIndex] = true;
    
    if (fileName)
      blitTileToAtlas(adjustedIndex, i, ATLAS_WIDTH, atlas);
  }
  
  if (fileName)
  {
    IMG_SavePNG(atlas, fileName);
    SDL_FreeSurface(atlas);
  }
  
  return result;
}

gfx_tile_mapping<16> createJoiningTileTextureAtlas4dirs(size_t gfxOffset, size_t arrayIndex, Plane plane, const char* fileName)
{
  using namespace lbx;
  constexpr size_t ATLAS_WIDTH = 16;
  constexpr size_t ATLAS_HEIGHT = 1;
  
  SDL_Surface* atlas = nullptr;
  
  if (fileName)
    SDL_CreateRGBSurface(0, ATLAS_WIDTH*TILE_WIDTH, ATLAS_HEIGHT*TILE_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
  
  const LBXArrayData* mapping = Repository::arrayFor(LBXID::TERRTYPE, arrayIndex);
  const size_t delta = plane == Plane::ARCANUS ? 0 : TILE_PER_PLANE;
  
  gfx_tile_mapping<16> result;
  
  static const u16 indices[] = { 1, 4, 1+4, 16, 1+16, 4+16, 1+4+16, 64, 64+1, 64+4, 64+1+4, 64+16, 64+1+16, 64+4+16, 64+1+4+16 };
  
  for (size_t i = 0; i < 15; ++i)
  {
    u16 gfxIndex = mapping->get<u16>(indices[i]);
    u16 adjustedIndex = gfxIndex + delta + gfxOffset;
    
    result[i+1] = spriteInfoToGfxTile(LSI(TERRAIN, adjustedIndex));
    used[adjustedIndex] = true;
    
    if (fileName)
      blitTileToAtlas(adjustedIndex, i, ATLAS_WIDTH, atlas);
  }
  
  if (fileName)
  {
    IMG_SavePNG(atlas, fileName);
    SDL_FreeSurface(atlas);
  }
  
  return result;
}

void Viewport::createMapTextureAtlas()
{
  using namespace lbx;

  Repository::loadLBXTerrain();
  Repository::loadLBXTerrainMap();
  Repository::loadMultipleLBXSpriteData(LBXID::TERRAIN);
  
  /*createJoiningTileTextureAtlas8dirs(0, 0, Plane::ARCANUS, used, "arcanus-shore.png");
  createJoiningTileTextureAtlas8dirs(0, 0, Plane::MYRRAN, used, "myrran-shore.png");
  createJoiningTileTextureAtlas8dirs(-2 + 0x124, 0, Plane::ARCANUS, used, "arcanus-desert.png");
  createJoiningTileTextureAtlas8dirs(-2 + 0x124, 0, Plane::MYRRAN, used, "myrran-desert.png");
  createJoiningTileTextureAtlas8dirs(-2 + 0x25A, 0, Plane::ARCANUS, used, "arcanus-tundra.png");
  createJoiningTileTextureAtlas8dirs(-2 + 0x25A, 0, Plane::MYRRAN, used, "myrran-tundra.png");
  
  createJoiningTileTextureAtlas4dirs(0, 1, Plane::ARCANUS, used, "arcanus-mountains.png");
  createJoiningTileTextureAtlas4dirs(-0x103 + 0x113, 1, Plane::ARCANUS, used, "arcanus-hills.png");
  createJoiningTileTextureAtlas4dirs(0, 1, Plane::MYRRAN, used, "myrran-mountains.png");
  createJoiningTileTextureAtlas4dirs(-0x103 + 0x113, 1, Plane::MYRRAN, used, "myrran-hills.png");*/
  
  static_assert(sizeof(TileToSpriteMap::rivers) == sizeof(gfx_tile_t)*43, "");
  
  /* nodes and special things */
  mapSprite(0x0A8, arcanus.manaNodes.sorcery, myrran.manaNodes.sorcery);
  mapSprite(0x0A9, arcanus.manaNodes.nature, myrran.manaNodes.nature);
  mapSprite(0x0AA, arcanus.manaNodes.chaos, myrran.manaNodes.chaos);
  mapSprite(0x0B3, arcanus.volcano, myrran.volcano);

  /* standand full terrain types */
  mapSprite({{0xA3, 0xB7, 0xB8}}, arcanus.forest, myrran.forest);
  mapSprite({{0xA7, 0xB5, 0xB6}}, arcanus.tundra, myrran.tundra);
  mapSprite({{0xA6, 0xB1, 0xB2}}, arcanus.swamp, myrran.swamp);
  mapSprite({{0xA5, 0xAE, 0xAF, 0xB0}}, arcanus.desert, myrran.desert);
  // TODO: 0x01 is excluded because reported as buggy in I Like Serena MoM Save Format doc
  mapSprite({{0xA2, 0xAC, 0xAD, 0xB4}}, arcanus.grasslands, myrran.grasslands);
  mapSprite({{0x00, 0x259}}, arcanus.ocean, myrran.ocean);
  
  /* hills and mountains */
  arcanus.mountains = createJoiningTileTextureAtlas4dirs(0, 1, Plane::ARCANUS, nullptr);
  arcanus.hills = createJoiningTileTextureAtlas4dirs(-0x103 + 0x113, 1, Plane::ARCANUS, nullptr);
  myrran.mountains = createJoiningTileTextureAtlas4dirs(0, 1, Plane::MYRRAN, nullptr);
  myrran.hills = createJoiningTileTextureAtlas4dirs(-0x103 + 0x113, 1, Plane::MYRRAN, nullptr);
  /* single hill or mountain are mapped manually to position 0 */
  mapSprite(0xA4, arcanus.mountains[0], myrran.mountains[0]);
  mapSprite(0xAB, arcanus.hills[0], myrran.hills[0]);
  
  /* shores */
  arcanus.shores = createJoiningTileTextureAtlas8dirs(0, 0, Plane::ARCANUS, nullptr);
  myrran.shores = createJoiningTileTextureAtlas8dirs(0, 0, Plane::MYRRAN, nullptr);
  
  /* desert joinings */
  arcanus.desertJoin = createJoiningTileTextureAtlas8dirs(-2 + 0x124, 0, Plane::ARCANUS, nullptr);
  myrran.desertJoin = createJoiningTileTextureAtlas8dirs(-2 + 0x124, 0, Plane::MYRRAN, nullptr);
  
  /* tundra joinings */
  arcanus.tundraJoin = createJoiningTileTextureAtlas8dirs(-2 + 0x25A, 0, Plane::ARCANUS, nullptr);
  myrran.tundraJoin = createJoiningTileTextureAtlas8dirs(-2 + 0x25A, 0, Plane::MYRRAN, nullptr);
  
  /* rivers */
  mapSprite({ 0xB9, 0xBA, 0xBB, 0xBC }, arcanus.rivers.cap, myrran.rivers.cap);
  mapSprite({ 0xBD, 0xC1, 0xE9, 0xBF, 0xC3, 0xEB, 0xBE, 0xC2, 0xEA, 0xC0, 0xC4, 0xEC }, arcanus.rivers.corner,  myrran.rivers.corner);
  mapSprite({ 0xED, 0xEE, 0xEF, 0xF0, 0xF1, 0xF2 }, arcanus.rivers.straight, myrran.rivers.straight);
  mapSprite({ 0xFB, 0xFC, 0xFD, 0xFE, 0xF7, 0xF8, 0xF9, 0xFA, 0xFF, 0x100, 0x101, 0x102, 0xF3, 0xF4, 0xF5, 0xF6 }, arcanus.rivers.tcross, myrran.rivers.tcross);
  mapSprite({ 0x1D4, 0x1D5, 0x1D6, 0x1D7, 0x1D8 }, arcanus.rivers.cross, myrran.rivers.cross);
  
  /* river mouths */
  /* TODO: not enough because for example the closed lake is used also for shores with diagonal elements */
  {
    /* set all to default shore tiles to be fancy also when not correct */
    {
      //TODO: define an iterator in common code to avoid this, like for (DirJoin join : Util::orthogonalDirections())
      const std::array<DirJoin, 8> masks = { DirJoin::N, DirJoin::E, DirJoin::S, DirJoin::W, DirJoin::OCORNER_NE, DirJoin::OCORNER_NW, DirJoin::OCORNER_SE, DirJoin::OCORNER_SW };
      for (int j = 0; j < 8; ++j)
      {
        arcanus.riverMouths.mapForRiverMask(masks[j]) = arcanus.shores;
        myrran.riverMouths.mapForRiverMask(masks[j]) = myrran.shores;
      }
    }
    
    const auto& s = arcanus.shores;
    using DJ = DirJoin;
    
    /* full lakes */
    SpriteInfo info = arcanus.shores[DirJoin::ALL];
    mapRiverSprite<1UL>({ DJ::ALL }, {0xC5}, arcanus.shores, arcanus.riverMouths.west, myrran.riverMouths.west);
    mapRiverSprite<1UL>({ DJ::ALL }, {0xC6}, arcanus.shores, arcanus.riverMouths.north, myrran.riverMouths.north);
    mapRiverSprite<1UL>({ DJ::ALL }, {0xC7}, arcanus.shores, arcanus.riverMouths.east, myrran.riverMouths.east);
    mapRiverSprite<1UL>({ DJ::ALL }, {0xC8}, arcanus.shores, arcanus.riverMouths.south, myrran.riverMouths.south);

    /* shore corners with double river */
    {
      /* order is not the same as in normal shore mapping so we need to adjust it by hand */
      std::array<size_t, 4> dest = { 0xC9 + 0, 0xC9 + 1, 0xC9 + 2, 0xC9 + 3};
      std::array<size_t, 4> src = { 0x22 + 1, 0x22 + 0, 0x22 + 3, 0x22 + 2};
      //TODO: still wrong in some corners
      std::array<decltype(arcanus.riverMouths.corner_nw)*, 4> destArcanus = { &arcanus.riverMouths.corner_nw, &arcanus.riverMouths.corner_se, &arcanus.riverMouths.corner_ne, &arcanus.riverMouths.corner_sw };
      std::array<decltype(arcanus.riverMouths.corner_nw)*, 4> destMyrran = { &myrran.riverMouths.corner_nw, &myrran.riverMouths.corner_se, &myrran.riverMouths.corner_ne, &myrran.riverMouths.corner_sw };
      
      for (int i = 0; i < 4; ++i)
      {
        mapRiverSprite(src, dest, arcanus.shores, *destArcanus[i], *destMyrran[i]);
        for (int j = 0; j < dest.size(); ++j) { dest[j] += 4; src[j] += 4; }
      }
    }
    
    /* shore single edges with single river */
    {
      std::array<size_t, 4> dest = { 0xD9 + 0, 0xD9 + 1, 0xD9 + 2, 0xD9 + 3 };
      std::array<DirJoin, 4> src = { DJ::EDGE_S, DJ::S|DJ::SE, DJ::S|DJ::SW, DJ::S };
      DJ riverDest = DJ::S;
      
      for (int i = 0; i < 4; ++i)
      {
        mapRiverSprite(src, dest, arcanus.shores, arcanus.riverMouths.mapForRiverMask(riverDest), myrran.riverMouths.mapForRiverMask(riverDest));
        for (int j = 0; j < dest.size(); ++j)
        {
          dest[j] += 4;
          src[j] <<= 2;
        }
        
        riverDest <<= 2;
      }
    }
    
    /* U shore with river on opposite side */
    /* \ /  | /  \ |  | |
       | |  | |  | |  | |
       ---  ---  ---  ---
     */
    {
      std::array<DirJoin, 4> src = { DJ::EDGE_S | DJ::W | DJ::E, DJ::EDGE_S | DJ::EDGE_W | DJ::E, DJ::EDGE_S | DJ::EDGE_E | DJ::W, DJ::ALL - DJ::N };
      std::array<size_t, 4> dest = { 0x1C4, 0x1C4 + 4, 0x1C4 + 8, 0x1C4 + 12};
      DJ riverDest = DJ::S;
      
      for (int i = 0; i < 4; ++i)
      {
        mapRiverSprite(src, dest, arcanus.shores, arcanus.riverMouths.mapForRiverMask(riverDest), myrran.riverMouths.mapForRiverMask(riverDest));
        
        std::for_each(src.begin(), src.end(), [](DirJoin& dj) { dj <<= 2; });
        std::for_each(dest.begin(), dest.end(), [](size_t& index) { ++index; });
        riverDest <<= 2;
      }
    }
    
    /* U shore with river one one of the sides */
    {
      const std::array<DirJoin, 4> srcs = { DJ::EDGE_S | DJ::W | DJ::E, DJ::EDGE_S | DJ::EDGE_W | DJ::E, DJ::EDGE_S | DJ::EDGE_E | DJ::W, DJ::ALL - DJ::N };
      const std::array<DirJoin, 2> riverMasks = { DJ::W, DJ::E };
      const size_t base = 0x1F9;
      constexpr size_t total = 4 * 4 * 2;
      
      for (int i = 0; i < total; ++i)
      {
        DirJoin src = srcs[(i / 4) % 4]; // 0 0 0 0 1 1 1 1 2 2 2 2 3 3 3 3 0 0 0 0 ...
        src <<= (i % 4) * 2; // 0 2 4 6 0 2 4 6 0 ...
        DirJoin riverMask = riverMasks[i / 16] << ((i % 4) * 2); // W N E S W N E S W N E S W N E S E S W N E S W ...
        
        mapRiverSprite<1UL>({ src }, { base + i }, arcanus.shores, arcanus.riverMouths.mapForRiverMask(riverMask), myrran.riverMouths.mapForRiverMask(riverMask));
      }
    }
    
    /* double horizontal edges with rivers north and south */
    /* --- --- --- ---  --/ --/ --/ --/  \-- \-- \-- \--  \-/ \-/ \-/ \-/
       --- --\ /-- /-\  --- --\ /-- /-\  --- --\ /-- /-\  --- --\ /-- /-\
     */
    {
      std::array<DirJoin, 4> top = { DJ::EDGE_N, DJ::NW | DJ::N, DJ::N | DJ::NE, DJ::N };
      std::array<DirJoin, 4> bottom = { DJ::EDGE_S, DJ::SW | DJ::S, DJ::S | DJ::SE, DJ::S };
      
      for (size_t i = 0; i < top.size(); ++i)
      {
        for (size_t j = 0; j < bottom.size(); ++j)
        {
          mapRiverSprite<1UL>( { top[i] | bottom[j] }, { 0x219 + i*4 + j }, arcanus.shores, arcanus.riverMouths.mapForRiverMask(DJ::N), myrran.riverMouths.mapForRiverMask(DJ::N));
          mapRiverSprite<1UL>( { top[i] | bottom[j] }, { 0x219 + i*4 + j + 16 }, arcanus.shores, arcanus.riverMouths.mapForRiverMask(DJ::S), myrran.riverMouths.mapForRiverMask(DJ::S));
        }
      }
    }
    
    /* double vertical edges with rivers west and east */
    {
      std::array<DirJoin, 4> left = { DJ::EDGE_W, DJ::NW | DJ::W, DJ::W | DJ::SW, DJ::W };
      std::array<DirJoin, 4> right = { DJ::E, DJ::E | DJ::SE, DJ::NE | DJ::E, DJ::EDGE_E };
      
      for (size_t i = 0; i < left.size(); ++i)
      {
        for (size_t j = 0; j < right.size(); ++j)
        {
          mapRiverSprite<1UL>( { left[i] | right[j] }, { 0x239 + i*4 + j }, arcanus.shores, arcanus.riverMouths.mapForRiverMask(DJ::W), myrran.riverMouths.mapForRiverMask(DJ::W));
          mapRiverSprite<1UL>( { left[i] | right[j] }, { 0x239 + i*4 + j + 16 }, arcanus.shores, arcanus.riverMouths.mapForRiverMask(DJ::E), myrran.riverMouths.mapForRiverMask(DJ::E));
        }
      }
    }
    
    /* corners with single rivers */
    {
      std::array<DirJoin, 8> src = {
        DJ::CORNER_NW, DJ::CORNER_NW | DJ::NE, DJ::CORNER_NW | DJ::SW, DJ::CORNER_NW | DJ::NE | DJ::SW,
        DJ::CORNER_SE, DJ::CORNER_SE | DJ::NE, DJ::CORNER_SE | DJ::SW, DJ::CORNER_SE | DJ::NE | DJ::SW
      };
      std::array<DirJoin, 8> riverMask = { DJ::W, DJ::S, DJ::N, DJ::S, DJ::N, DJ::E, DJ::E, DJ::W };
      size_t base = 0x1D9;

      for (size_t i = 0; i < 32; ++i)
      {
        const DirJoin fj = src[i % 8] << (((i / 8) % 2 == 1) ? 2 : 0);
        mapRiverSprite<1UL>( { fj }, { base + i }, arcanus.shores, arcanus.riverMouths.mapForRiverMask(riverMask[i/4]), myrran.riverMouths.mapForRiverMask(riverMask[i/4]));
      }
    }
  }
  

  

  
  {
    SDL_Surface* atlas = SDL_CreateRGBSurface(0, 40*TILE_WIDTH + 40, 40*TILE_HEIGHT + 40, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    
    size_t unused = TILE_COUNT - used.count();
    size_t side = ::ceil(::sqrt(unused));
    SDL_Surface* uatlas = SDL_CreateRGBSurface(0, (int)side*TILE_WIDTH, (int)side*TILE_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    size_t ucount = 0;
    
    SDL_Rect rect = {0,0,40*TILE_WIDTH+40, 40*TILE_HEIGHT+40};
    SDL_FillRect(atlas, &rect, 0xFF000000);
    
    for (size_t i = 0; i < TILE_COUNT; ++i)
    {
      const LBXSpriteData* sprite = Repository::spriteFor(LSI(TERRAIN, i));
      
      const size_t bx = (20+1) * (i % 40);
      const size_t by = (18+1) * (i / 40);
      
      for (size_t x = 0; x < TILE_WIDTH; ++x)
      {
        for (size_t y = 0; y < TILE_HEIGHT; ++y)
        {
          Color* pixel = reinterpret_cast<Color*>(atlas->pixels) + bx + x + (by + y)*atlas->w;
          Color color = sprite->getPalette()->get(sprite->at(x, y, 0, 0));
          
          /*if (!used[i])
            color = color.blend(Color(255,0,0,128));*/
          
          *pixel = color;
        }
      }
      
      /* draw number */
      {
        constexpr size_t fw = 3, fh = 5;
        static byte font[5][3*16] = {
          { 1,1,1,  0,1,0,  1,1,1,  1,1,1,  1,0,0,  1,1,1,  1,1,1,  1,1,1,  1,1,1,  1,1,1,  1,1,1,  1,1,1,  0,1,1,  1,1,0,  1,1,1,  1,1,1 },
          { 1,0,1,  0,1,0,  0,0,1,  0,0,1,  1,0,0,  1,0,0,  1,0,0,  0,0,1,  1,0,1,  1,0,1,  1,0,1,  1,0,1,  1,0,0,  1,0,1,  1,0,0,  1,0,0 },
          { 1,0,1,  0,1,0,  1,1,1,  1,1,1,  1,0,0,  1,1,1,  1,1,1,  0,0,1,  1,1,1,  1,1,1,  1,1,1,  1,1,0,  1,0,0,  1,0,1,  1,1,1,  1,1,0 },
          { 1,0,1,  0,1,0,  1,0,0,  0,0,1,  1,1,1,  0,0,1,  1,0,1,  0,0,1,  1,0,1,  0,0,1,  1,0,1,  1,0,1,  1,0,0,  1,0,1,  1,0,0,  1,0,0 },
          { 1,1,1,  0,1,0,  1,1,1,  1,1,1,  0,1,0,  1,1,1,  1,1,1,  0,0,1,  1,1,1,  1,1,1,  1,0,1,  1,1,1,  0,1,1,  1,1,0,  1,1,1,  1,0,0 }
        };
        static char buffer[16];
        
        sprintf(buffer, "%X", (int)i);

        color_d fontColor = 0xFFFFFFFF;
        
        if (!used[i])
          fontColor = 0xFFFF0000;
        
        for (size_t c = 0; c < strlen(buffer); ++c)
        {
          const size_t bfx = bx + 1 + (fw+1)*c, bfy = by + 1;
          const size_t index = buffer[c] >= 'A' ? (buffer[c] - 'A' + 10) : (buffer[c] - '0');
          for (size_t fy = 0; fy < fh; ++fy)
            for (size_t fx = 0; fx < fw; ++fx)
            {
              int v = font[fy][3*index + fx];
              
              if (v)
              {
                Color* pixel = reinterpret_cast<Color*>(atlas->pixels) + bfx + fx + (bfy + fy)*atlas->w;
                *pixel = pixel->blend(fontColor, (u8)160);
              }
            }
        }
      }
      
      if (!used[i])
      {
        blitTileToAtlas(LSI(TERRAIN, i), ucount % side, ucount / side, uatlas);
        ++ucount;
      }
    }
    
    IMG_SavePNG(atlas, "atlas.png");
    SDL_FreeSurface(atlas);
    
    IMG_SavePNG(uatlas, "unused.png");
    SDL_FreeSurface(uatlas);
  }
  
  {
    bool withRivers = true;
    
    SDL_Surface* atlas = SDL_CreateRGBSurface(0, (66 + (withRivers ? 8*8 : 0)) *TILE_WIDTH, 32*TILE_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    const auto& source = arcanus;
    
    for (size_t i = 0; i < source.grasslands.size(); ++i) blitTileToAtlas(source.grasslands[i], 0, i, atlas);
    for (size_t i = 0; i < source.forest.size(); ++i) blitTileToAtlas(source.forest[i], 1, i, atlas);
    for (size_t i = 0; i < source.tundra.size(); ++i) blitTileToAtlas(source.tundra[i], 2, i, atlas);
    for (size_t i = 0; i < source.swamp.size(); ++i) blitTileToAtlas(source.swamp[i], 3, i, atlas);
    for (size_t i = 0; i < source.desert.size(); ++i) blitTileToAtlas(source.desert[i], 4, i, atlas);
    for (size_t i = 0; i < source.ocean.size(); ++i) blitTileToAtlas(source.ocean[i], 5, i, atlas);
    
    blitTileToAtlas(source.manaNodes.chaos, 6, 0, atlas);
    blitTileToAtlas(source.manaNodes.nature, 6, 1, atlas);
    blitTileToAtlas(source.manaNodes.sorcery, 6, 2, atlas);
    blitTileToAtlas(source.volcano, 6, 3, atlas);
    
    for (size_t i = 0; i < source.hills.size(); ++i) blitTileToAtlas(source.hills[i], 7, i, atlas);
    for (size_t i = 0; i < source.mountains.size(); ++i) blitTileToAtlas(source.mountains[i], 8, i, atlas);
    
    const DirJoin dirs[] = {
      DirJoin::N, DirJoin::E, DirJoin::S, DirJoin::W,
      DirJoin::OCORNER_NE, DirJoin::OCORNER_SE, DirJoin::OCORNER_SW, DirJoin::OCORNER_NW,
      DirJoin::VERTICAL, DirJoin::HORIZONTAL,
      DirJoin::TCROSS_N, DirJoin::TCROSS_E, DirJoin::TCROSS_S, DirJoin::TCROSS_W,
      DirJoin::OCROSS
    };
    
    for (size_t i = 0; i < 15; ++i)
    {
      for (size_t j = 0; j < source.rivers.countForMask(dirs[i]); ++j)
        blitTileToAtlas(source.rivers.spriteForMask(dirs[i], j), 9 + i, j, atlas);
    }
    
    std::array<DirJoin, 8> riverMasks = { DirJoin::N, DirJoin::E, DirJoin::S, DirJoin::W, DirJoin::OCORNER_NE, DirJoin::OCORNER_NW, DirJoin::OCORNER_SE, DirJoin::OCORNER_SW };
    for (size_t i = 0; i < 256; ++i)
    {
      if (i)
      {
        blitTileToAtlas(source.shores[i], 9 + 15 + i % 8, i / 8, atlas);
        blitTileToAtlas(source.desertJoin[i], 9 + 15 + 8 + i % 8, i / 8, atlas);
        blitTileToAtlas(source.tundraJoin[i], 9 + 15 + 8 + 8 + i % 8, i / 8, atlas);
        
        if (withRivers)
        {
          for (int r = 0; r < 8; ++r)
            blitTileToAtlas(source.riverMouths.mapForRiverMask(riverMasks[r])[i], 9 + 15 + 8 + 8 + 8 + 8*r + i % 8, i / 8, atlas);
        }
      
      }

    }

    
    IMG_SavePNG(atlas, "mapped.png");
    SDL_FreeSurface(atlas);
    
  }
  
  
}
