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

#include "Items.h"

#include <unordered_map>

enum class Ranged : u8;
enum class Property : u8;

class HeroSpec;
class UnitSpec;
class Spell;

class TileGfxSpec
{
private:
  
  const std::vector<Color> colors[PLANE_COUNT];
  
public:
  TileGfxSpec(u8 row, u8 count, Color c1, Color c2) : row(row), count(count), colors{{c1},{c2}} { }
  TileGfxSpec(u8 row, u8 count, std::initializer_list<Color> c1, std::initializer_list<Color> c2) : row(row), count(count), colors{c1,c2} { }
  
  
  u8 row;
  u8 count;
  
  Color minimapColor(Plane plane, u16 i = 0) const
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
  const SpriteInfo fullFigure;
  const bool isFlyingFigure;
  
  UnitGfxSpec(SpriteInfo still, SpriteInfo fullFigure, bool isFlyingFigure = false) : still(still), fullFigure(fullFigure), isFlyingFigure(isFlyingFigure) { }
};

struct WizardGfxSpec
{
  WizardGfxSpec(SpriteInfo portraitSmall, SpriteInfo portraitLarge, SpriteInfo diplomacyMood) : portraitSmall(portraitSmall), portraitLarge(portraitLarge), diplomacyMood(diplomacyMood) { }
  const SpriteInfo portraitSmall;
  const SpriteInfo portraitLarge;
  const SpriteInfo diplomacyMood;

};


class GfxData
{
private:
  static std::unordered_map<WizardID, WizardGfxSpec, enum_hash> wizardSpecs;
  static std::unordered_map<const UnitSpec*, UnitGfxSpec> unitSpecs;
  static std::unordered_map<const UnitSpec*, SpriteInfo> heroPortraits;
  
  static const TileGfxSpec specs[];
  static constexpr s8 RANGED_INDEX[] = {-1,6,5,6,14,15,16,17,18};
  static constexpr s8 PROPERTY_INDEX[] = {-1,-1,-1,-1,9,22,14,15,16,17,18};
  
  static std::unordered_map<const Spell*, sprite_ref> specialSpellGfxEffects;

public:
  static Color colorForSchool(School school);
  static const TileGfxSpec& tileGfxSpec(TileType type) { return specs[type]; }
  static s8 rangedGfxIndex(Ranged ranged) { return RANGED_INDEX[static_cast<u8>(ranged)]; }
  static s8 propertyGfxIndex(Property property) { return PROPERTY_INDEX[static_cast<u8>(property)]; }
  
  static SpriteInfo itemGfxSpec(Item::TypeID type, int index);
  
  static const WizardGfxSpec& wizardGfxSpec(WizardID wizard) { return wizardSpecs[wizard]; }
  
  static const UnitGfxSpec& unitGfxSpec(const UnitSpec* spec);
  static SpriteInfo heroGfxSpec(const UnitSpec* spec);
  
  static SpriteInfo specialSpellGfxEffect(const Spell* spell);
};

#endif
