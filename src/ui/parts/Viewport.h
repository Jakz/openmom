#ifndef _VIEWPORT_H_
#define _VIEWPORT_H_

#include "common/Common.h"

#include "Tile.h"
#include <unordered_map>

class Tile;
class LocalPlayer;
class World;
class Place;

using gfx_tile_t = SpriteInfo;//const SpriteSheet*;
template<size_t SIZE> class gfx_tile_mapping : public std::array<gfx_tile_t, SIZE>
{
public:
  gfx_tile_mapping() : std::array<gfx_tile_t,SIZE>()
  {
    std::fill(this->begin(), this->end(), LSI(TERRAIN,0));
  }
  
  inline gfx_tile_t const & operator[](DirJoin mask) const { return this->operator[](static_cast<size_t>(mask)); }
  inline gfx_tile_t const & operator[](size_t mask) const { return std::array<gfx_tile_t, SIZE>::operator[](mask); }
  
  inline gfx_tile_t& operator[](DirJoin mask) { return this->operator[](static_cast<size_t>(mask)); }
  inline gfx_tile_t& operator[](size_t mask) { return std::array<gfx_tile_t, SIZE>::operator[](mask); }
};

using full_gfx_tile_mapping = gfx_tile_mapping<TILE_COUNT_SHORE>;


struct TileToSpriteMap
{  
  struct
  {
    gfx_tile_t sorcery;
    gfx_tile_t nature;
    gfx_tile_t chaos;
  } manaNodes;
  
  struct
  {
    std::array<gfx_tile_mapping<TILE_COUNT_RIVER_CAP>, 4> cap;
    std::array<gfx_tile_mapping<TILE_COUNT_RIVER_CORNER>, 4> corner;
    std::array<gfx_tile_mapping<TILE_COUNT_RIVER_STRAIGHT>, 2> straight;
    std::array<gfx_tile_mapping<TILE_COUNT_RIVER_T_CROSS>, 4> tcross;
    gfx_tile_mapping<TILE_COUNT_RIVER_CROSS> cross;
    
    /* return river sprite for mask direction */
    /*
     |   |   | O |   | O |   |   | O | O |   | O |   | O | O | O |
     |   |O  |   |  O| OO| OO|OO |OO | O |OOO| OO|OOO|OO |OOO|OOO|
     | O |   |   |   |   | O | O |   | O |   | O | O | O |   | O |
     */
    gfx_tile_t spriteForMask(DirJoin mask, size_t index) const
    {
      switch (mask)
      {
        case DirJoin::S: return cap[0][index];
        case DirJoin::W: return cap[1][index];
        case DirJoin::N: return cap[2][index];
        case DirJoin::E: return cap[3][index];
          
        case DirJoin::OCORNER_NE: return corner[0][index];
        case DirJoin::OCORNER_SE: return corner[1][index];
        case DirJoin::OCORNER_SW: return corner[2][index];
        case DirJoin::OCORNER_NW: return corner[3][index];
          
        case DirJoin::VERTICAL: return straight[0][index];
        case DirJoin::HORIZONTAL: return straight[1][index];
          
        case DirJoin::TCROSS_E: return tcross[0][index];
        case DirJoin::TCROSS_S: return tcross[1][index];
        case DirJoin::TCROSS_W: return tcross[2][index];
        case DirJoin::TCROSS_N: return tcross[3][index];
          
        case DirJoin::OCROSS: return cross[index];
          
        /* forced for single river tiles which shouldn't exist in final maps */
        case DirJoin::NONE: return cross[index];
          
        default:
          assert(false);
      }
    }
    
    /* return amount of sprites per type */
    size_t countForMask(DirJoin mask) const
    {
      switch (mask)
      {
        case DirJoin::S:
        case DirJoin::W:
        case DirJoin::N:
        case DirJoin::E:
          return 1;
          
        case DirJoin::OCORNER_NE:
        case DirJoin::OCORNER_NW:
        case DirJoin::OCORNER_SW:
        case DirJoin::OCORNER_SE:
          return 3;
          
        case DirJoin::VERTICAL:
        case DirJoin::HORIZONTAL:
          return 3;
          
        case DirJoin::TCROSS_W:
        case DirJoin::TCROSS_S:
        case DirJoin::TCROSS_E:
        case DirJoin::TCROSS_N:
          return 4;
          
        case DirJoin::OCROSS:
          return 5;
          
        default:
          assert(false);
          return 0;
      }
    }
    
    
  } rivers;
  
  struct
  {
    mutable full_gfx_tile_mapping north, south, west, east;
    mutable full_gfx_tile_mapping corner_ne, corner_nw, corner_sw, corner_se;

    full_gfx_tile_mapping& mapForRiverMask(DirJoin join) const
    {
      switch (join)
      {
        case DirJoin::N: return north;
        case DirJoin::S: return south;
        case DirJoin::E: return east;
        case DirJoin::W: return west;
        
        case DirJoin::OCORNER_NE: return corner_ne;
        case DirJoin::OCORNER_NW: return corner_nw;
        case DirJoin::OCORNER_SW: return corner_sw;
        case DirJoin::OCORNER_SE: return corner_se;
          
        default:
          assert(false);
          return north;
      }
    }

    gfx_tile_t spriteForRiverAndJoinMask(DirJoin river, DirJoin join) const
    {
      switch (river)
      {
        case DirJoin::N: return north[join];
        case DirJoin::E: return east[join];
        case DirJoin::S: return south[join];
        case DirJoin::W: return west[join];
          
        case DirJoin::OCORNER_NE: return corner_ne[join];
        case DirJoin::OCORNER_NW: return corner_nw[join];
        case DirJoin::OCORNER_SE: return corner_se[join];
        case DirJoin::OCORNER_SW: return corner_sw[join];
          
        default:
          //assert(false);
          return LSI(TERRAIN, 0);
      }
    }
    
    
  } riverMouths;
  
  gfx_tile_t volcano;
  
  gfx_tile_mapping<TILE_COUNT_FOREST> forest;
  gfx_tile_mapping<TILE_COUNT_TUNDRA> tundra;
  gfx_tile_mapping<TILE_COUNT_TUNDRA> swamp;
  gfx_tile_mapping<TILE_COUNT_DESERT> desert;
  gfx_tile_mapping<TILE_COUNT_DESERT> grasslands;
  gfx_tile_mapping<TILE_COUNT_OCEAN> ocean;
  
  gfx_tile_mapping<TILE_COUNT_HILLS> hills;
  gfx_tile_mapping<TILE_COUNT_HILLS> mountains;
  
  gfx_tile_mapping<TILE_COUNT_SHORE> shores;
  gfx_tile_mapping<TILE_COUNT_DESERT_JOIN> desertJoin;
  gfx_tile_mapping<TILE_COUNT_TUNDRA_JOIN> tundraJoin;
  
  
};

class Viewport
{
private:  
  static void drawViewport(const World* map, const LocalPlayer* player, const Position& p, s16 ttx, s16 tty, s16 w, s16 h, bool darkenEdges);
  
  static SpriteInfo gfxForPlace(const Place* place);
  
public:
  static SpriteInfo gfxForTerrain(const Tile* tile);
  static SpriteInfo gfxForResource(Resource resource);
  
  static void drawTile(const Tile* tile, u16 x, s16 y, Plane plane);
  
  static inline s16 baseOffsetX(s16 vx, s16 w) { return vx - w/2; }
  static inline s16 baseOffsetY(s16 vy, s16 h) { return vy - h/2; }

  static Point screenCoordsForTile(const LocalPlayer* player, Position p);
  static Position tileCoords(const LocalPlayer* player, s16 x, s16 y);
  static bool isOutsideViewport(const LocalPlayer* player, s16 x, s16 y);
  
  static void drawCityViewport(const LocalPlayer* player, const World* map, const Position& p) { drawViewport(map, player, p, 215, 3, 5, 5, true); }
  static void drawMainViewport(const LocalPlayer* player, const World* map);

  static void drawMicroMap(const LocalPlayer* player, s16 dx, s16 dy, s16 w, s16 h, s16 vx, s16 vy, Plane plane, Color fillColor);
  static void drawMicroMap(const LocalPlayer* player, s16 dx, s16 dy, s16 w, s16 h, s16 vx, s16 vy, Plane plane);

  static const u16 viewportW = 12, viewportH = 10;
  static const u16 tileWidth = 20, tileHeight = 18;
  static const u16 baseX = 0, baseY = 20;
  
  static const Position hoveredPosition(const World* world, const LocalPlayer* player, u16 x, u16 y);
  
  static void createMapTextureAtlas();
  static TileToSpriteMap arcanus, myrran;
};

#endif
