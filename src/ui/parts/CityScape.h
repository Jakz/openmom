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
enum HouseType : u8;

class CityLayout
{
private:
  struct BuildingSpecs
  {
    bool animated;
    SpriteInfo info;
    s16 width, depth;
    s16 pixelWidth;
    
    BuildingSpecs() : info(SpriteInfo(TextureID::CITY_FORTRESS,-1,-1)) { }
    BuildingSpecs(SpriteInfo&& info, s16 pixelWidth, s16 width, s16 depth) : info(info), pixelWidth(pixelWidth), width(width), depth(depth), animated(info.y == -1) { }
  };
  
  static std::unordered_map<const Building*, BuildingSpecs> specs;

  static constexpr int U = 5;
  static std::map<const City*, CityLayout*> layouts;

  
  static struct {
    bool operator()(const Building *b1, const Building* b2)
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
    
    bool operator<(const LayoutPosition &rhs) const { return y < rhs.y; }

  };

public:
  struct LayoutZone
  {
    s16 w, h;
    s16 x, y;
    bool central, seaside;
    
    LayoutZone() { }
    LayoutZone(s16 x, s16 y, s16 w, s16 h, bool central = false, bool seaside = false) : x(x), y(y), w(w), h(h), central(central), seaside(seaside) { }
    
    friend bool operator==(const LayoutZone& z1, const LayoutZone& z2);
  };
  
private:
  std::vector<LayoutPosition> positions;
  std::vector<LayoutZone> zones;
  
  const City* city;

public:
  static void draw(const City* city, LocalPlayer* player);
  static s16 buildingHeight(const Building* building);
  static void drawBuilding(const Building* building, s16 x, s16 y);
  static void drawBuildingCentered(const Building* building, s16 x, s16 y);
  
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
    LayoutPosition createPosition(LayoutZone& zone, s16 ox, s16 oy, const Building* building);
    const std::vector<LayoutZone> findSuitable(const Building* building);
    void placeAndSplit(const Building* building, LayoutZone& zone);
  
    const std::vector<LayoutZone> getZones() {
     return
      {
        LayoutZone(50-20,129,4,4),
        LayoutZone(98-20,129,4,4),
        LayoutZone(146-20,129,3,4),
        LayoutZone(184-20,129,4,4),
        
        LayoutZone(47-20,152,2,3,false,true),
        LayoutZone(75-20,152,4,3),
        LayoutZone(123-20,152,3,3,true,false),
        LayoutZone(161-20,152,4,3),
        LayoutZone(205-20,157,2,2),
        
        LayoutZone(57-20,170,4,4),
        LayoutZone(105-20,170,3,4),
        LayoutZone(143-20,170,4,4),
        LayoutZone(192-20,170,3,4),
      };
    }
};

inline bool operator==(const CityLayout::LayoutZone& z1, const CityLayout::LayoutZone& z2)
{
  return z1.w == z2.w && z1.h == z2.h && z1.y == z2.y && z1.y == z2.y;
}

#endif
