//
//  CityScape.h
//  OpenMoM
//
//  Created by Jack on 7/25/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _CITY_SCAPE_H_
#define _CITY_SCAPE_H_

#include "Common.h"

class Building;
class City;
class LocalPlayer;

#include <unordered_map>

class CityScape
{
private:
  struct BuildingSpecs
  {
    const bool animated;
    const SpriteInfo info;
    const s16 width, depth;
    const s16 pixelWidth;
    
    BuildingSpecs(SpriteInfo&& info, s16 pixelWidth, s16 width, s16 depth) : info(info), pixelWidth(pixelWidth), width(width), depth(depth), animated(info.y == -1) { }
  };
  
  static std::unordered_map<const Building*, BuildingSpecs> specs;
  
public:
  static void draw(const City* city, LocalPlayer* player);
  static s16 buildingHeight(const Building* building);
  static void drawBuilding(const Building* building, s16 x, s16 y);
  static void drawBuildingCentered(const Building* building, s16 x, s16 y);
};

#endif
