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
#include "Race.h"

using namespace std;

template<> gfx_map<const Skill*, SkillGfxSpec>& GfxData::containerFor<const Skill*, SkillGfxSpec>()
{
  static gfx_map<const Skill*, SkillGfxSpec> skillfGfxSpec;
  return skillfGfxSpec;
}

template<> gfx_map<const UnitSpec*, UnitGfxSpec>& GfxData::containerFor<const UnitSpec*, UnitGfxSpec>()
{
  static gfx_map<const UnitSpec*, UnitGfxSpec> unitGfxSpec;
  return unitGfxSpec;
}

constexpr s8 GfxData::RANGED_INDEX[];
constexpr s8 GfxData::PROPERTY_INDEX[];

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

gfx_map<const Spell*, sprite_ref> GfxData::specialSpellGfxEffects = {
  {Spells::CORRUPTION, LBXI(SPECFX, 7)},
  {Spells::CHANGE_TERRAIN, LBXI(SPECFX, 8)},
  {Spells::RAISE_VOLCANO, LBXI(SPECFX, 11)}
};

decltype(GfxData::wizardSpecs) GfxData::wizardSpecs = {
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

decltype(GfxData::playerSpecs) GfxData::playerSpecs = {
  { PlayerColor::BLUE, { LSI(MAGIC, 1), LSI(MAPBACK, 14), LSI(MAPBACK, 63), { LSI(MAGIC, 60), LSI(MAGIC, 61), LSI(MAGIC, 62) } } },
  { PlayerColor::GREEN, { LSI(MAGIC, 2), LSI(MAPBACK, 15), LSI(MAPBACK, 64), { LSI(MAGIC, 63), LSI(MAGIC, 64), LSI(MAGIC, 65) } } },
  { PlayerColor::PURPLE, { LSI(MAGIC, 3), LSI(MAPBACK, 16), LSI(MAPBACK, 65), { LSI(MAGIC, 66), LSI(MAGIC, 67), LSI(MAGIC, 68) } } },
  { PlayerColor::RED, { LSI(MAGIC, 4), LSI(MAPBACK, 17), LSI(MAPBACK, 66), { LSI(MAGIC, 69), LSI(MAGIC, 70), LSI(MAGIC, 71) } } },
  { PlayerColor::YELLOW, { LSI(MAGIC, 5), LSI(MAPBACK, 18), LSI(MAPBACK, 67), { LSI(MAGIC, 72), LSI(MAGIC, 73), LSI(MAGIC, 74) } } },
  { PlayerColor::NEUTRAL, { LSI_PLACEHOLD, LSI(MAPBACK, 19), LSI_PLACEHOLD, { LSI_PLACEHOLD, LSI_PLACEHOLD, LSI_PLACEHOLD } } }
};

decltype(GfxData::raceSpecs) GfxData::raceSpecs = {
  { RaceID::BARBARIANS, { LSI(BACKGRND, 45), LSI(BACKGRND, 59), LSI(BACKGRND, 74) } },
  { RaceID::BEASTMEN, { LSI(BACKGRND, 46), LSI(BACKGRND, 60), LSI(BACKGRND, 75) } },
  { RaceID::DARK_ELVES, { LSI(BACKGRND, 47), LSI(BACKGRND, 61), LSI(BACKGRND, 76) } },
  { RaceID::DRACONIANS, { LSI(BACKGRND, 48), LSI(BACKGRND, 62), LSI(BACKGRND, 77) } },
  { RaceID::DWARVES, { LSI(BACKGRND, 49), LSI(BACKGRND, 63), LSI(BACKGRND, 78) } },
  { RaceID::GNOLLS, { LSI(BACKGRND, 50), LSI(BACKGRND, 64), LSI(BACKGRND, 79) } },
  { RaceID::HALFLINGS, { LSI(BACKGRND, 51), LSI(BACKGRND, 65), LSI(BACKGRND, 80) } },
  { RaceID::HIGH_ELVES, { LSI(BACKGRND, 52), LSI(BACKGRND, 66), LSI(BACKGRND, 81) } },
  { RaceID::HIGH_MEN, { LSI(BACKGRND, 53), LSI(BACKGRND, 67), LSI(BACKGRND, 82) } },
  { RaceID::KLACKONS, { LSI(BACKGRND, 54), LSI(BACKGRND, 68), LSI(BACKGRND, 83) } },
  { RaceID::LIZARDMEN, { LSI(BACKGRND, 55), LSI(BACKGRND, 69), LSI(BACKGRND, 84) } },
  { RaceID::NOMADS, { LSI(BACKGRND, 56), LSI(BACKGRND, 70), LSI(BACKGRND, 85) } },
  { RaceID::ORCS, { LSI(BACKGRND, 57), LSI(BACKGRND, 71), LSI(BACKGRND, 86) } },
  { RaceID::TROLLS, { LSI(BACKGRND, 58), LSI(BACKGRND, 72), LSI(BACKGRND, 87) } },
};

decltype(GfxData::raceHouseSpecs) GfxData::raceHouseSpecs = {
  { HouseType::NORMAL, { LSI(BACKGRND, 37), LSI(BACKGRND, 34), LSI(CITYSCAP, 25), LSI(CITYSCAP, 42) } },
  { HouseType::TREE, { LSI(BACKGRND, 38), LSI(BACKGRND, 35), LSI(CITYSCAP, 30), LSI(CITYSCAP, 43) } },
  { HouseType::MUD, { LSI(BACKGRND, 39), LSI(BACKGRND, 36), LSI(CITYSCAP, 35), LSI(CITYSCAP, 44) } }
};

//TODO: LIFE SPELL (SPECFX, 3) uses palette with multiple white alphas, understand how to manage it
decltype(GfxData::schoolSpecs) GfxData::schoolSpecs = {
  { School::ARCANE, { RGB(0,0,0), LSI(SPELLSCR, 61), LSI(SPELLS, 9), LSI_PLACEHOLD, LSI_PLACEHOLD, LSI(MAGIC, 48), LSI(SPECFX, 5) } },
  { School::NATURE, { RGB(0,255,0), LSI(SPELLSCR, 62), LSI(SPELLS, 4), LSI(WIZLAB, 14), LSI(NEWGAME,30), LSI(MAGIC, 33), LSI(SPECFX, 0) } },
  { School::SORCERY, { RGB(0,0,255), LSI(SPELLSCR, 63), LSI(SPELLS, 5), LSI(WIZLAB, 15), LSI(NEWGAME, 27), LSI(MAGIC, 36), LSI(SPECFX, 1) } },
  { School::CHAOS, { RGB(255,0,0), LSI(SPELLSCR, 64), LSI(SPELLS, 6), LSI(WIZLAB, 16), LSI(NEWGAME, 36), LSI(MAGIC, 39), LSI(SPECFX, 2) } },
  { School::LIFE, { RGB(255,255,255), LSI(SPELLSCR, 65), LSI(SPELLS, 7), LSI(WIZLAB, 17), LSI(NEWGAME, 24), LSI(MAGIC, 42), LSI(SPECFX, 3) } },
  { School::DEATH, { RGB(0,0,0), LSI(SPELLSCR, 66), LSI(SPELLS, 8), LSI(WIZLAB, 18), LSI(NEWGAME, 33), LSI(MAGIC, 45), LSI(SPECFX, 4) } },
};

decltype(GfxData::upkeepSymbolSpec) GfxData::upkeepSymbolSpec = {
  { UpkeepSymbol::FOOD, { LSI(BACKGRND, 40), LSI(BACKGRND, 94), LSI(BACKGRND, 88), LSI(BACKGRND, 95) } },
  { UpkeepSymbol::WORK, { LSI(BACKGRND, 41), LSI_PLACEHOLD, LSI(BACKGRND, 89), LSI_PLACEHOLD } },
  { UpkeepSymbol::GOLD, { LSI(BACKGRND, 42), LSI(BACKGRND, 73), LSI(BACKGRND, 90), LSI(BACKGRND, 93) } },
  { UpkeepSymbol::MANA, { LSI(BACKGRND, 43), LSI_PLACEHOLD, LSI(BACKGRND, 91), LSI_PLACEHOLD } },
  { UpkeepSymbol::RESEARCH, { LSI(BACKGRND, 44), LSI_PLACEHOLD, LSI(BACKGRND, 92), LSI_PLACEHOLD } }
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

SpriteInfo GfxData::itemGfxSpec(items::TypeID type, int index)
{
  using ID = items::TypeID;
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
