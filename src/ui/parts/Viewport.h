#ifndef _VIEWPORT_H_
#define _VIEWPORT_H_

#include "Common.h"

#include <unordered_map>

class Tile;
class LocalPlayer;
class World;
class Place;


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

  static ScreenCoord screenCoordsForTile(const LocalPlayer* player, Position p);
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
};

#endif
