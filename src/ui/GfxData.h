//
//  GfxData.h
//  OpenMoM
//
//  Created by Jack on 7/21/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _GFX_DATA_H_
#define _GFX_DATA_H_

#include "Common.h"

#include <unordered_map>

enum class Ranged : u8;
enum class Property : u8;

class UnitSpec;

class TileGfxSpec
{
private:
  
  const std::vector<Color> colors[PLANE_COUNT];
  
public:
  TileGfxSpec(u8 row, u8 count, Color c1, Color c2) : row(row), count(count), colors{{c1},{c2}} { }
  TileGfxSpec(u8 row, u8 count, std::initializer_list<Color> c1, std::initializer_list<Color> c2) : row(row), count(count), colors{c1,c2} { }
  
  
  u8 row;
  u8 count;
  
  Color minimapColor(Plane plane, u16 i = 0)
  {
    if (colors[plane].size() > 1)
      return colors[plane][i];
    else
      return colors[plane][0];
  }
  
  
};

struct UnitGfxSpec
{
  const SpriteInfo still;
  const TextureID fullFigure;
  const bool isFlyingFigure;
  
  UnitGfxSpec(SpriteInfo still, TextureID fullFigure, bool isFlyingFigure = false) : still(still), fullFigure(fullFigure), isFlyingFigure(isFlyingFigure) { }
};


class GfxData
{
private:
  static std::unordered_map<const UnitSpec*, UnitGfxSpec> unitSpecs;
  static const TileGfxSpec specs[];
  static constexpr s8 rangedIndex[] = {-1,6,5,6,14,15,16,17,18};
  static constexpr s8 propertyIndex[] = {-1,-1,-1,-1,9,22,14,15,16,17,18};

public:
  static Color colorForSchool(School school);
  static const TileGfxSpec& tileGfxSpec(TileType type) { return specs[type]; }
  static const s8 rangedGfxIndex(Ranged ranged) { return rangedIndex[static_cast<u8>(ranged)]; }
  static const s8 propertyGfxIndex(Property property) { return rangedIndex[static_cast<u8>(property)]; }
  
  static const UnitGfxSpec& unitGfxSpec(const UnitSpec* spec);
};

#endif
