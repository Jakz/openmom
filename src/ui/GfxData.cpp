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



Color GfxData::colorForSchool(const School school)
{
  return school < SCHOOL_COUNT ? schoolColors[school] : 0;
}

unordered_map<const UnitSpec*, UnitGfxSpec> GfxData::unitSpecs = {
  { UnitSpec::raceSpec(UnitID::SPEARMEN, RaceID::BARBARIANS), UnitGfxSpec(SpriteInfo(UNITS_STATIC_STANDARD, 0, (u8)RaceID::BARBARIANS), UNIT_BARBARIANS_SPEARMEN) },
  { UnitSpec::raceSpec(UnitID::SWORDSMEN, RaceID::BARBARIANS), UnitGfxSpec(SpriteInfo(UNITS_STATIC_STANDARD, 1, (u8)RaceID::BARBARIANS), UNIT_BARBARIANS_SWORDSMEN) },
  { UnitSpec::raceSpec(UnitID::BOWMEN, RaceID::BARBARIANS), UnitGfxSpec(SpriteInfo(UNITS_STATIC_STANDARD, 2, (u8)RaceID::BARBARIANS), UNIT_BARBARIANS_BOWMEN) },
  { UnitSpec::raceSpec(UnitID::CAVALRY, RaceID::BARBARIANS), UnitGfxSpec(SpriteInfo(UNITS_STATIC_STANDARD, 3, (u8)RaceID::BARBARIANS), UNIT_BARBARIANS_CAVALRY) },
  { UnitSpec::raceSpec(UnitID::SHAMANS, RaceID::BARBARIANS), UnitGfxSpec(SpriteInfo(UNITS_STATIC_STANDARD, 4, (u8)RaceID::BARBARIANS), UNIT_BARBARIANS_SHAMAN) },
  { UnitSpec::raceSpec(UnitID::BERSERKERS, RaceID::BARBARIANS), UnitGfxSpec(SpriteInfo(UNITS_STATIC_STANDARD, 8, (u8)RaceID::BARBARIANS), UNIT_BARBARIANS_BERSERKERS) },
  { UnitSpec::raceSpec(UnitID::SETTLERS, RaceID::BARBARIANS), UnitGfxSpec(SpriteInfo(UNITS_STATIC_STANDARD, 10,(u8)RaceID::BARBARIANS), UNIT_BARBARIANS_SETTLERS) },

  //{ UnitSpec::raceSpec(UnitID::SPEARMEN, RaceID::BEASTMEN), UnitGfxSpec(SpriteInfo(UNITS_STATIC_STANDARD, 0, (u8)RaceID::BEASTMEN), UNIT_BEASTMEN_SPEARMEN) },
  //{ UnitSpec::raceSpec(UnitID::SWORDSMEN, RaceID::BEASTMEN), UnitGfxSpec(SpriteInfo(UNITS_STATIC_STANDARD, 1, (u8)RaceID::BEASTMEN), UNIT_BEASTMEN_SWORDSMEN) },
  //{ UnitSpec::raceSpec(UnitID::BOWMEN, RaceID::BEASTMEN), UnitGfxSpec(SpriteInfo(UNITS_STATIC_STANDARD, 2, (u8)RaceID::BEASTMEN), UNIT_BEASTMEN_BOWMEN) }
  
  
  { UnitSpec::heroSpec(UnitID::HERO_DWARF), UnitGfxSpec(SpriteInfo(UNITS_HEROES, 0, 0), UNIT_HERO_DWARF, false) },
  //{ UnitSpec::heroSpec(UnitID::HERO_ORC_WARRIOR), UnitGfxSpec(SpriteInfo(UNITS_HEROES, 6, 0), UNIT_HERO_ORC_WARRIOR) }
  
  
  { UnitSpec::summonSpec(UnitID::MAGIC_SPIRIT), UnitGfxSpec(SpriteInfo(UNITS_FANTASTIC, 0, 0), UNIT_MAGIC_SPIRIT) },
  
  { UnitSpec::summonSpec(UnitID::HELL_HOUNDS), UnitGfxSpec(SpriteInfo(UNITS_FANTASTIC, 0, 1), UNIT_HELL_HOUND) },
  { UnitSpec::summonSpec(UnitID::GREAT_DRAKE), UnitGfxSpec(SpriteInfo(UNITS_FANTASTIC, 9, 1), UNIT_GREAT_DRAKE, true) }

};

const UnitGfxSpec& GfxData::unitGfxSpec(const UnitSpec* spec)
{
  const UnitGfxSpec& gspec = unitSpecs.find(spec)->second;
  return gspec;
}

s16 GfxData::specialSpellGfxEffect(const Spell *spell) { return specialSpellGfxEffects[spell]; }