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

unordered_map<const Spell*, sprite_ref> GfxData::specialSpellGfxEffects = {
  {Spells::CORRUPTION, LBXI(SPECFX, 7)},
  {Spells::CHANGE_TERRAIN, LBXI(SPECFX, 8)},
  {Spells::RAISE_VOLCANO, LBXI(SPECFX, 11)}
};

std::unordered_map<WizardID, WizardGfxSpec, enum_hash> GfxData::wizardSpecs = {
  { WizardID::MERLIN, { LSI(MAGIC, 18), LSI(WIZARDS, 0), LSI(MOODWIZ, 0), LSI(LILWIZ, 0), LSI(WIZLAB, 0), LSI(SPELLSCR, 46) } },
  { WizardID::RAVEN, { LSI(MAGIC, 19), LSI(WIZARDS, 1), LSI(MOODWIZ, 1), LSI(LILWIZ, 5), LSI(WIZLAB, 1), LSI(SPELLSCR, 47) } },
  { WizardID::SHAREE, { LSI(MAGIC, 20), LSI(WIZARDS, 2), LSI(MOODWIZ, 2), LSI(LILWIZ, 10), LSI(WIZLAB, 2), LSI(SPELLSCR, 48) } },
  { WizardID::LO_PAN, { LSI(MAGIC, 21), LSI(WIZARDS, 3), LSI(MOODWIZ, 3), LSI(LILWIZ, 15), LSI(WIZLAB, 3), LSI(SPELLSCR, 49) } },
  { WizardID::JAFAR, { LSI(MAGIC, 22), LSI(WIZARDS, 4), LSI(MOODWIZ, 4), LSI(LILWIZ, 20), LSI(WIZLAB, 4), LSI(SPELLSCR, 50) } },
  { WizardID::OBERIC, { LSI(MAGIC, 23), LSI(WIZARDS, 5), LSI(MOODWIZ, 5), LSI(LILWIZ, 25), LSI(WIZLAB, 5), LSI(SPELLSCR, 51) } },
  { WizardID::RJAK, { LSI(MAGIC, 24), LSI(WIZARDS, 6), LSI(MOODWIZ, 6), LSI(LILWIZ, 30), LSI(WIZLAB, 6), LSI(SPELLSCR, 52) } },
  { WizardID::SSS_RA, { LSI(MAGIC, 25), LSI(WIZARDS, 7), LSI(MOODWIZ, 7), LSI(LILWIZ, 35), LSI(WIZLAB, 7), LSI(SPELLSCR, 53) } },
  { WizardID::TAURON, { LSI(MAGIC, 26), LSI(WIZARDS, 8), LSI(MOODWIZ, 8), LSI(LILWIZ, 40), LSI(WIZLAB, 8), LSI(SPELLSCR, 54) } },
  { WizardID::FREYA, { LSI(MAGIC, 27), LSI(WIZARDS, 9), LSI(MOODWIZ, 9), LSI(LILWIZ, 45), LSI(WIZLAB, 9), LSI(SPELLSCR, 55) } },
  { WizardID::HORUS, { LSI(MAGIC, 28), LSI(WIZARDS, 10), LSI(MOODWIZ, 10), LSI(LILWIZ, 50), LSI(WIZLAB, 10), LSI(SPELLSCR, 56) } },
  { WizardID::ARIEL, { LSI(MAGIC, 29), LSI(WIZARDS, 11), LSI(MOODWIZ, 11), LSI(LILWIZ, 55), LSI(WIZLAB, 11), LSI(SPELLSCR, 57) } },
  { WizardID::TLALOC, { LSI(MAGIC, 30), LSI(WIZARDS, 12), LSI(MOODWIZ, 12), LSI(LILWIZ, 60), LSI(WIZLAB, 12), LSI(SPELLSCR, 58) } },
  { WizardID::KALI, { LSI(MAGIC, 31), LSI(WIZARDS, 13), LSI(MOODWIZ, 13), LSI(LILWIZ, 65), LSI(WIZLAB, 13), LSI(SPELLSCR, 59) } }
};

std::unordered_map<PlayerColor, PlayerGfxSpec, enum_hash> GfxData::playerSpecs = {
  { PlayerColor::BLUE, { LSI(MAGIC, 1), LSI(MAPBACK, 14) } },
  { PlayerColor::GREEN, { LSI(MAGIC, 2), LSI(MAPBACK, 15) } },
  { PlayerColor::PURPLE, { LSI(MAGIC, 3), LSI(MAPBACK, 16) } },
  { PlayerColor::RED, { LSI(MAGIC, 4), LSI(MAPBACK, 17) } },
  { PlayerColor::YELLOW, { LSI(MAGIC, 5), LSI(MAPBACK, 18) } },
  { PlayerColor::NEUTRAL, { LSI_PLACEHOLD, LSI(MAPBACK, 19) } }
};


constexpr s8 GfxData::RANGED_INDEX[];
constexpr s8 GfxData::PROPERTY_INDEX[];


Color GfxData::colorForSchool(const School school)
{
  return school < SCHOOL_COUNT ? schoolColors[school] : 0;
}

std::unordered_map<const UnitSpec*, UnitGfxSpec> GfxData::unitSpecs = {
  { UnitSpec::raceSpec(UnitID::SPEARMEN, RaceID::BARBARIANS), UnitGfxSpec(LSI(UNITS1, 39), LSI(FIGURES3, 72)) },
  { UnitSpec::raceSpec(UnitID::SWORDSMEN, RaceID::BARBARIANS), UnitGfxSpec(LSI(UNITS1, 40), LSI(FIGURES3, 80)) },
  { UnitSpec::raceSpec(UnitID::BOWMEN, RaceID::BARBARIANS), UnitGfxSpec(LSI(UNITS1, 41), LSI(FIGURES3, 88)) },
  { UnitSpec::raceSpec(UnitID::CAVALRY, RaceID::BARBARIANS), UnitGfxSpec(LSI(UNITS1, 42), LSI(FIGURES3, 96)) },
  { UnitSpec::raceSpec(UnitID::SHAMANS, RaceID::BARBARIANS), UnitGfxSpec(LSI(UNITS1, 43), LSI(FIGURES3, 104)) },
  { UnitSpec::raceSpec(UnitID::BERSERKERS, RaceID::BARBARIANS), UnitGfxSpec(LSI(UNITS1, 45), LSI(FIGURES4, 0)) },
  { UnitSpec::raceSpec(UnitID::SETTLERS, RaceID::BARBARIANS), UnitGfxSpec(LSI(UNITS1, 44), LSI(FIGURES3, 112)) },

  //{ UnitSpec::raceSpec(UnitID::SPEARMEN, RaceID::BEASTMEN), UnitGfxSpec(SpriteInfo(UNITS_STATIC_STANDARD, 0, (u8)RaceID::BEASTMEN), UNIT_BEASTMEN_SPEARMEN) },
  //{ UnitSpec::raceSpec(UnitID::SWORDSMEN, RaceID::BEASTMEN), UnitGfxSpec(SpriteInfo(UNITS_STATIC_STANDARD, 1, (u8)RaceID::BEASTMEN), UNIT_BEASTMEN_SWORDSMEN) },
  //{ UnitSpec::raceSpec(UnitID::BOWMEN, RaceID::BEASTMEN), UnitGfxSpec(SpriteInfo(UNITS_STATIC_STANDARD, 2, (u8)RaceID::BEASTMEN), UNIT_BEASTMEN_BOWMEN) }
  
  
  { UnitSpec::heroSpec(UnitID::HERO_DWARF), UnitGfxSpec(LSI(UNITS1, 0), LSI(FIGURES1, 0)) },
  //{ UnitSpec::heroSpec(UnitID::HERO_ORC_WARRIOR), UnitGfxSpec(SpriteInfo(UNITS_HEROES, 6, 0), UNIT_HERO_ORC_WARRIOR) }
  
  
  { UnitSpec::summonSpec(UnitID::MAGIC_SPIRIT), UnitGfxSpec(LSI(UNITS2, 34), LSI(FIGURE11, 32)) },
  
  { UnitSpec::summonSpec(UnitID::GUARDIAN_SPIRIT), UnitGfxSpec(LSI(UNITS2, 56), LSI(FIGURE12, 88)) }, // TODO: full figure wrong

  
  { UnitSpec::summonSpec(UnitID::HELL_HOUNDS), UnitGfxSpec(LSI(UNITS2, 45), LSI(FIGURE11, 40)) },
  { UnitSpec::summonSpec(UnitID::GREAT_DRAKE), UnitGfxSpec(LSI(UNITS2, 44), LSI(FIGURE11, 112), true) }

};

std::unordered_map<const UnitSpec*, SpriteInfo> GfxData::heroPortraits = {
  { UnitSpec::heroSpec(UnitID::HERO_DWARF), LSI(PORTRAIT, 10) },
  { UnitSpec::heroSpec(UnitID::HERO_ORC_WARRIOR), LSI(PORTRAIT, 6) }
};

SpriteInfo WizardGfxSpec::getGemmedPortrait(PlayerColor color) const
{
  switch (color)
  {
    case BLUE: return gemmedPortrait.relative(0);
    case GREEN: return gemmedPortrait.relative(1);
    case PURPLE: return gemmedPortrait.relative(2);
    case RED: return gemmedPortrait.relative(3);
    case YELLOW: return gemmedPortrait.relative(4);
    default: assert(false);
  }
}

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

SpriteInfo GfxData::itemGfxSpec(items::Item::TypeID type, int index)
{
  using ID = items::Item::TypeID;
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

SpriteInfo GfxData::specialSpellGfxEffect(const Spell *spell) { return specialSpellGfxEffects[spell]; }
