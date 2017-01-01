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
#include "Texture.h"
#include "City.h"

#include <unordered_map>
#include <map>
#include <vector>

class Building;
class City;
class LocalPlayer;
enum class HouseType : u8;

class CityLayout
{
private:
  struct BuildingSpecs
  {
    SpriteInfo info;
    s16 width, depth;
    s16 pixelWidth;
    
    BuildingSpecs() : info(LSI_PLACEHOLD) { }
    BuildingSpecs(SpriteInfo info, s16 pixelWidth, s16 width, s16 depth) : info(info), pixelWidth(pixelWidth), width(width), depth(depth) { }
  };
  
  static std::unordered_map<const Building*, BuildingSpecs> specs;

  static constexpr int U = 5;
  static std::map<const City*, CityLayout*> layouts;

  
  static struct {
    bool operator()(const Building *b1, const Building* b2) const
    {
      const BuildingSpecs& s1 = specs[b1];
      const BuildingSpecs& s2 = specs[b2];
      
      s32 t1 = s1.width*s1.depth;
      s32 t2 = s2.width*s2.depth;
      
      return t1 < t2;
    }
  } GFX_COMPARATOR;
  
  struct LayoutPosition
  {
    s16 x, y;
    const Building* building;
    u8 house;
    
    LayoutPosition() { }
    LayoutPosition(s16 x, s16 y, const Building* building) : x(x), y(y), building(building) { }
    LayoutPosition(s16 x, s16 y, u8 house) : x(x), y(y), house(house), building(nullptr) { }
    
    bool operator==(const LayoutPosition& rhs) const { return y == rhs.y && x == rhs.x; }
    bool operator<(const LayoutPosition& rhs) const { return y == rhs.y ? (x < rhs.x) : (y < rhs.y); }
  };

public:
  struct LayoutZone
  {
    ScreenCoord coords;
    s16 x, y;
    s16 w, h;
    
    bool central, seaside;
    
    LayoutZone() : coords(0,0) { }
    LayoutZone(ScreenCoord coords, s16 x, s16 y, s16 w, s16 h, bool central = false, bool seaside = false) : coords(coords), x(x), y(y), w(w), h(h), central(central), seaside(seaside) { }
    LayoutZone(s16 x, s16 y, s16 w, s16 h, bool central = false, bool seaside = false) : coords(x,y), x(0), y(0), w(w), h(h), central(central), seaside(seaside) { }
    
    bool operator==(const LayoutZone& z2) const
    {
      return coords == z2.coords && w == z2.w && h == z2.h && x == z2.x && y == z2.y;
    }
    
  };
  
private:
  std::vector<LayoutPosition> positions;
  std::vector<LayoutZone> zones;
  using zone_iterator = decltype(zones)::const_iterator;
  
  const City* city;

public:
  static void draw(const City* city, LocalPlayer* player);
  static void drawBuilding(const Building* building, s16 x, s16 y);
  static void drawBuildingSprite(SpriteInfo info, s16, s16 y);
  static void drawBuildingCentered(const City* city, const Building* building, s16 x, s16 y);
  
  static void createLayout(const City* city)
  {
    CityLayout* layout = new CityLayout(city);
    layouts.emplace(city, layout);
    layout->deploy();
  }
  
  static void updateLayout(const City* city)
  {
    CityLayout* layout = layouts[city];
    layout->deploy();
    city->revalidateLayout = false;
  }
  
  static bool contains(const City* city) { return layouts.find(city) != layouts.end(); }

  CityLayout(const City* city) : city(city) { }
  
  void deploy();
  
private:
  LayoutPosition createPosition(const LayoutZone& zone, s16 ox, s16 oy, const Building* building);
  const std::vector<LayoutZone> findSuitable(const Building* building);
  void placeAndSplit(const Building* building, zone_iterator zone);
  const std::vector<LayoutZone> getZones(const City* city);
};

#endif
