//
//  GfxData.cpp
//  OpenMoM
//
//  Created by Jack on 7/21/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "GfxData.h"

#include "Gfx.h"
#include "UnitSpec.h"
#include "Texture.h"
#include "Spells.h"

using namespace std;

static const Color schoolColors[] = {
  Gfx::color(0,0,0),
  Gfx::color(255, 0, 0),
  Gfx::color(0, 0, 0),
  Gfx::color(255, 255, 255),
  Gfx::color(0, 255, 0),
  Gfx::color(0, 0, 255)
};

const TileGfxSpec GfxData::specs[] =
{
  TileGfxSpec(0,6,RGB(56,94,18),RGB(81,60,48)), // GRASS
  TileGfxSpec(0,0,{RGB(0, 14, 90),RGB(7, 25, 95)},{RGB(8,4,4),RGB(36,28,24)}), // WATER
  TileGfxSpec(0,0,{RGB(0, 14, 90),RGB(7, 25, 95)},{RGB(8,4,4),RGB(36,28,24)}), // SHORE
  TileGfxSpec(0,0,{RGB(105,96,93),RGB(126, 116, 113)},{RGB(255, 255, 255)}), // MOUNTAIN
  TileGfxSpec(0,0,{RGB(105,96,93),RGB(126, 116, 113)},{RGB(255, 255, 255)}), // VOLCANO
  TileGfxSpec(0,0,RGB(53,92,17),RGB(115,83,69)), // HILL
  TileGfxSpec(1,3,RGB(0, 94, 19),RGB(0, 94, 19)),  // FOREST
  TileGfxSpec(2,3,RGB(56,47,43),RGB(56,47,43)), // SWAMP
  TileGfxSpec(3,4,{RGB(190,154,117),RGB(142,113,89)},{RGB(255,255,255)}), // DESERT
  TileGfxSpec(4,3,RGB(56,47,43),RGB(255,255,255)), // TUNDRA
  TileGfxSpec(0,0,RGB(255, 255, 255),RGB(255, 255, 255)), // RIVER
  TileGfxSpec(0,0,{RGB(0, 14, 90),RGB(7, 25, 95)},{RGB(8,4,4),RGB(36,28,24)}) // RIVER_MOUTH TODO: check colors
};

unordered_map<const Spell*, s16> GfxData::specialSpellGfxEffects = {
  {Spells::CORRUPTION, 11},
  {Spells::CHANGE_TERRAIN, 12},
  {Spells::RAISE_VOLCANO, 7}
};


constexpr s8 GfxData::RANGED_INDEX[];
constexpr s8 GfxData::PROPERTY_INDEX[];


Color GfxData::colorForSchool(const School school)
{
  return school < SCHOOL_COUNT ? schoolColors[school] : 0;
}

std::unordered_map<const UnitSpec*, UnitGfxSpec> GfxData::unitSpecs = {
  { UnitSpec::raceSpec(UnitID::SPEARMEN, RaceID::BARBARIANS), UnitGfxSpec(SpriteInfo(TextureID::UNITS_STATIC_STANDARD, (u8)RaceID::BARBARIANS, 0), LSI(FIGURE11, 112)) },
  { UnitSpec::raceSpec(UnitID::SWORDSMEN, RaceID::BARBARIANS), UnitGfxSpec(SpriteInfo(TextureID::UNITS_STATIC_STANDARD, (u8)RaceID::BARBARIANS, 1), LSI(FIGURE11, 112)) },
  { UnitSpec::raceSpec(UnitID::BOWMEN, RaceID::BARBARIANS), UnitGfxSpec(SpriteInfo(TextureID::UNITS_STATIC_STANDARD, (u8)RaceID::BARBARIANS, 2), LSI(FIGURE11, 112)) },
  { UnitSpec::raceSpec(UnitID::CAVALRY, RaceID::BARBARIANS), UnitGfxSpec(SpriteInfo(TextureID::UNITS_STATIC_STANDARD, (u8)RaceID::BARBARIANS, 3), LSI(FIGURE11, 112)) },
  { UnitSpec::raceSpec(UnitID::SHAMANS, RaceID::BARBARIANS), UnitGfxSpec(SpriteInfo(TextureID::UNITS_STATIC_STANDARD, (u8)RaceID::BARBARIANS, 4), LSI(FIGURE11, 112)) },
  { UnitSpec::raceSpec(UnitID::BERSERKERS, RaceID::BARBARIANS), UnitGfxSpec(SpriteInfo(TextureID::UNITS_STATIC_STANDARD, (u8)RaceID::BARBARIANS, 8), LSI(FIGURES4, 0)) },
  { UnitSpec::raceSpec(UnitID::SETTLERS, RaceID::BARBARIANS), UnitGfxSpec(SpriteInfo(TextureID::UNITS_STATIC_STANDARD, (u8)RaceID::BARBARIANS, 10), LSI(FIGURE11, 112)) },

  //{ UnitSpec::raceSpec(UnitID::SPEARMEN, RaceID::BEASTMEN), UnitGfxSpec(SpriteInfo(UNITS_STATIC_STANDARD, 0, (u8)RaceID::BEASTMEN), UNIT_BEASTMEN_SPEARMEN) },
  //{ UnitSpec::raceSpec(UnitID::SWORDSMEN, RaceID::BEASTMEN), UnitGfxSpec(SpriteInfo(UNITS_STATIC_STANDARD, 1, (u8)RaceID::BEASTMEN), UNIT_BEASTMEN_SWORDSMEN) },
  //{ UnitSpec::raceSpec(UnitID::BOWMEN, RaceID::BEASTMEN), UnitGfxSpec(SpriteInfo(UNITS_STATIC_STANDARD, 2, (u8)RaceID::BEASTMEN), UNIT_BEASTMEN_BOWMEN) }
  
  
  { UnitSpec::heroSpec(UnitID::HERO_DWARF), UnitGfxSpec(SpriteInfo(TextureID::UNITS_HEROES, 0, 0), LSI(FIGURES1, 0)) },
  //{ UnitSpec::heroSpec(UnitID::HERO_ORC_WARRIOR), UnitGfxSpec(SpriteInfo(UNITS_HEROES, 6, 0), UNIT_HERO_ORC_WARRIOR) }
  
  
  { UnitSpec::summonSpec(UnitID::MAGIC_SPIRIT), UnitGfxSpec(SpriteInfo(TextureID::UNITS_FANTASTIC, 0, 0), LSI(FIGURE11, 32)) },
  
  { UnitSpec::summonSpec(UnitID::GUARDIAN_SPIRIT), UnitGfxSpec(SpriteInfo(TextureID::UNITS_FANTASTIC, 3, 1), LSI(FIGURE12, 88)) }, // TODO: full figure wrong

  
  { UnitSpec::summonSpec(UnitID::HELL_HOUNDS), UnitGfxSpec(SpriteInfo(TextureID::UNITS_FANTASTIC, 1, 0), LSI(FIGURE11, 40)) },
  { UnitSpec::summonSpec(UnitID::GREAT_DRAKE), UnitGfxSpec(SpriteInfo(TextureID::UNITS_FANTASTIC, 1, 9), LSI(FIGURE11, 112), true) }

};

std::unordered_map<const UnitSpec*, SpriteInfo> GfxData::heroPortraits = {
  { UnitSpec::heroSpec(UnitID::HERO_DWARF), LSI(PORTRAIT, 10) },
  { UnitSpec::heroSpec(UnitID::HERO_DWARF), LSI(PORTRAIT, 6) }
};

const UnitGfxSpec& GfxData::unitGfxSpec(const UnitSpec* spec)
{
  const UnitGfxSpec& gspec = unitSpecs.find(spec)->second;
  return gspec;
}

SpriteInfo GfxData::heroGfxSpec(const UnitSpec *spec)
{
  assert(heroPortraits.find(spec) != heroPortraits.end());
  return heroPortraits.find(spec)->second;
}

SpriteInfo GfxData::itemGfxSpec(Item::TypeID type, int index)
{
  using ID = Item::TypeID;
  switch (type)
  {
    case ID::SWORD: return LSI(ITEMS, 0).relative(index);
    case ID::MACE: return LSI(ITEMS, 9).relative(index);
    case ID::AXE: return LSI(ITEMS, 20).relative(index);
    case ID::BOW: return LSI(ITEMS, 29).relative(index);
    case ID::STAFF: return LSI(ITEMS, 38).relative(index);
    case ID::CHAIN: return LSI(ITEMS, 47).relative(index);
    case ID::PLATE: return LSI(ITEMS, 55).relative(index);
    case ID::SHIELD: return LSI(ITEMS, 62).relative(index);
    case ID::MISC: return LSI(ITEMS, 72).relative(index);
    case ID::WAND: return LSI(ITEMS, 107).relative(index);
  }
  assert(false);
}

s16 GfxData::specialSpellGfxEffect(const Spell *spell) { return specialSpellGfxEffects[spell]; }
