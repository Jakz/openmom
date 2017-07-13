#ifndef _VIEWPORT_H_
#define _VIEWPORT_H_

#include "common/Common.h"

#include <unordered_map>

class Tile;
class LocalPlayer;
class World;
class Place;

struct TileToSpriteMap
{
  template<size_t SIZE> using tile_mapping = std::array<const SpriteSheet*, SIZE>;
  
  enum : size_t
  {
    TILE_COUNT_OCEAN = 2,
    TILE_COUNT_FOREST = 3,
    TILE_COUNT_TUNDRA = 3,
    TILE_COUNT_SWAMP = 3,
    TILE_COUNT_DESERT = 4,
    TILE_COUNT_GRASSLANDS = 4,
    
    TILE_COUNT_HILLS = 16,
    TILE_COUNT_MOUNTAINS = 16,
    
    TILE_COUNT_RIVER_CAP = 1,
    TILE_COUNT_RIVER_CORNER = 3,
    TILE_COUNT_RIVER_STRAIGHT = 3,
    TILE_COUNT_RIVER_T_CROSS = 4,
    TILE_COUNT_RIVER_CROSS = 5,
    
    TILE_COUNT_SHORE = 256,
    TILE_COUNT_DESERT_JOIN = 256,
    TILE_COUNT_TUNDRA_JOIN = 256,
    
  };
  
  struct
  {
    const SpriteSheet* sorcery;
    const SpriteSheet* nature;
    const SpriteSheet* chaos;
  } manaNodes;
  
  struct
  {
    std::array<tile_mapping<TILE_COUNT_RIVER_CAP>, 4> cap;
    std::array<tile_mapping<TILE_COUNT_RIVER_CORNER>, 4> corner;
    std::array<tile_mapping<TILE_COUNT_RIVER_STRAIGHT>, 2> straight;
    std::array<tile_mapping<TILE_COUNT_RIVER_T_CROSS>, 4> tcross;
    tile_mapping<TILE_COUNT_RIVER_CROSS> cross;
    
    /* return river sprite for mask direction */
    /*
     |   |   | O |   | O |   |   | O | O |   | O |   | O | O | O |
     |   |O  |   |  O| OO| OO|OO |OO | O |OOO| OO|OOO|OO |OOO|OOO|
     | O |   |   |   |   | O | O |   | O |   | O | O | O |   | O |
     */
    const SpriteSheet* spriteForMask(DirJoin mask, size_t index) const
    {
      switch (mask)
      {
        case DirJoin::S: return cap[0][index];
        case DirJoin::W: return cap[1][index];
        case DirJoin::N: return cap[2][index];
        case DirJoin::E: return cap[3][index];
          
        case DirJoin::CORNER_N_E: return corner[0][index];
        case DirJoin::CORNER_S_E: return corner[1][index];
        case DirJoin::CORNER_S_W: return corner[2][index];
        case DirJoin::CORNER_N_W: return corner[3][index];
          
        case DirJoin::VERTICAL: return straight[0][index];
        case DirJoin::HORIZONTAL: return straight[1][index];
          
        case DirJoin::TCROSS_E: return tcross[0][index];
        case DirJoin::TCROSS_S: return tcross[1][index];
        case DirJoin::TCROSS_W: return tcross[2][index];
        case DirJoin::TCROSS_N: return tcross[3][index];
          
        case DirJoin::OCROSS: return cross[index];
          
        default:
          assert(false);
          return nullptr;
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
          
        case DirJoin::CORNER_N_E:
        case DirJoin::CORNER_N_W:
        case DirJoin::CORNER_S_W:
        case DirJoin::CORNER_S_E:
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
  
  const SpriteSheet* volcano;
  
  tile_mapping<TILE_COUNT_FOREST> forest;
  tile_mapping<TILE_COUNT_TUNDRA> tundra;
  tile_mapping<TILE_COUNT_TUNDRA> swamp;
  tile_mapping<TILE_COUNT_DESERT> desert;
  tile_mapping<TILE_COUNT_DESERT> grasslands;
  tile_mapping<TILE_COUNT_OCEAN> ocean;
  
  tile_mapping<TILE_COUNT_HILLS> hills;
  tile_mapping<TILE_COUNT_HILLS> mountains;
  
  tile_mapping<TILE_COUNT_SHORE> shores;
  tile_mapping<TILE_COUNT_DESERT_JOIN> desertJoin;
  tile_mapping<TILE_COUNT_TUNDRA_JOIN> tundraJoin;
  
  
};

class Viewport
{
private:
  static std::unordered_map<u8,u8> waterMap;
  
  static void drawViewport(const World* map, const LocalPlayer* player, const Position& p, s16 ttx, s16 tty, s16 w, s16 h, bool darkenEdges);
  
  static SpriteInfo gfxForPlace(const Place* place);
  static SpriteInfo gfxForResource(Resource resource);
  
public:
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
