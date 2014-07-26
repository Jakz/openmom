#ifndef _VIEWPORT_H_
#define _VIEWPORT_H_

#include "Common.h"

#include <unordered_map>

class Tile;
class LocalPlayer;
class World;


class Viewport
{
private:
  static std::unordered_map<u8,u8> waterMap;
  
  static const u16 viewportW = 12, viewportH = 10;
  static const u16 tileWidth = 20, tileHeight = 18;
  static const u16 baseX = 0, baseY = 20;
  
  
  
public:
  static void drawTile(const Tile* tile, u16 x, s16 y, Plane plane);
  
  static inline s16 baseOffsetX(s16 vx, s16 w) { return vx - w/2; }
  static inline s16 baseOffsetY(s16 vy, s16 h) { return vy - h/2; }

  static Position tileCoords(const LocalPlayer* player, s16 x, s16 y);
  static bool isOutsideViewport(const LocalPlayer* player, s16 x, s16 y);
  
  static void drawViewport(const World* map, const LocalPlayer* player, const Position& p, s16 ttx, s16 tty, s16 w, s16 h, bool darkenEdges);

};

#endif