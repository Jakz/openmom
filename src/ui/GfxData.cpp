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

#define DEFINE_GFX_MAP(__key__, __value__) \
template<> gfx_map<__key__, __value__>& GfxData::containerFor<__key__, __value__>() { \
  static gfx_map<__key__, __value__> map; \
  return map; \
}

DEFINE_GFX_MAP(const Skill*, SkillGfxSpec);
DEFINE_GFX_MAP(const UnitSpec*, UnitGfxSpec);
DEFINE_GFX_MAP(const Race*, RaceGfxSpec);

#pragma mark Unit Props

UnitPropGfxMap::UnitPropGfxMap() : map({
  { Type::SWORD, { LSI(COMPIX, 61), LSI(COMPIX, 29), LSI(UNITVIEW, 13), LSI(UNITVIEW, 35) } },
  { Type::SWORD_MAGIC, { LSI(COMPIX, 64), LSI(COMPIX, 32), LSI(UNITVIEW, 16), LSI(UNITVIEW, 38) } },
  { Type::SWORD_MITHRIL, { LSI(COMPIX, 63), LSI(COMPIX, 31), LSI(UNITVIEW, 15), LSI(UNITVIEW, 37) } },
  { Type::SWORD_ADAMANTIUM, { LSI(COMPIX, 65), LSI(COMPIX, 33), LSI(UNITVIEW, 17), LSI(UNITVIEW, 39) } },
  
  { Type::RANGED_BOW, { LSI(COMPIX, 66), LSI(COMPIX, 34), LSI(UNITVIEW, 18), LSI(UNITVIEW, 40) } },
  { Type::RANGED_ROCK, { LSI(COMPIX, 67), LSI(COMPIX, 35), LSI(UNITVIEW, 19), LSI(UNITVIEW, 41) } },
  { Type::RANGED_MAGIC, { LSI(COMPIX, 62), LSI(COMPIX, 30), LSI(UNITVIEW, 14), LSI(UNITVIEW, 36) } },

  { Type::SHIELD, { LSI(COMPIX, 70), LSI_PLACEHOLD, LSI(UNITVIEW, 22), LSI(UNITVIEW, 44) } },
  { Type::RESIST, { LSI(COMPIX, 75), LSI_PLACEHOLD, LSI(UNITVIEW, 27), LSI(UNITVIEW, 49) } },
  { Type::HITS, { LSI(COMPIX, 71), LSI_PLACEHOLD, LSI(UNITVIEW, 23), LSI(UNITVIEW, 45) } },
  
  { Type::MOVEMENT_FOOT, { LSI(COMPIX, 72), LSI(COMPIX, 38), LSI(UNITVIEW, 24), LSI_PLACEHOLD } },
  { Type::MOVEMENT_WATER, { LSI(COMPIX, 74), LSI(COMPIX, 40), LSI(UNITVIEW, 26), LSI_PLACEHOLD } },
  { Type::MOVEMENT_FLYING, { LSI(COMPIX, 73), LSI(COMPIX, 39), LSI(UNITVIEW, 25), LSI_PLACEHOLD } }

})
{
  
}

template<> const UnitPropGfxMap::Data& UnitPropGfxMap::operator[](MovementBaseType type) const
{
  switch (type)
  {
    case MovementBaseType::WALKING: return map[Type::MOVEMENT_FOOT];
    case MovementBaseType::SWIMMING: return map[Type::MOVEMENT_WATER];
    case MovementBaseType::FLYING: return map[Type::MOVEMENT_FLYING];
  }
  assert(false);
}

template<> const UnitPropGfxMap::Data& UnitPropGfxMap::operator[](MeleeType type) const
{
  switch (type)
  {
    case MeleeType::NORMAL: return map[Type::SWORD];
    case MeleeType::MAGIC: return map[Type::SWORD_MAGIC];
    case MeleeType::MITHRIL: return map[Type::SWORD_MITHRIL];
    case MeleeType::ADAMANTIUM: return map[Type::SWORD_ADAMANTIUM];
  }
  assert(false);
}

template<> const UnitPropGfxMap::Data& UnitPropGfxMap::operator[](Ranged type) const
{
  switch (type)
  {
    case Ranged::NONE: assert(false); break;
    case Ranged::ARROW: return map[Type::RANGED_BOW];
    case Ranged::ROCK: return map[Type::RANGED_ROCK];
    case Ranged::BULLET: return map[Type::RANGED_ROCK];
    /* TODO: add custom option to use specific icon for ranged attack according to school */
    default: return map[Type::RANGED_MAGIC];
  }
}

template<> const UnitPropGfxMap::Data& UnitPropGfxMap::operator[](Property property) const
{
  switch (property)
  {
    case Property::MELEE: return map[Type::SWORD];
    case Property::SHIELDS: return map[Type::SHIELD];
    case Property::RESIST: return map[Type::RESIST];
    case Property::HIT_POINTS: return map[Type::HITS];
    
    default: assert(false);
  }
}

constexpr s8 GfxData::RANGED_INDEX[];
constexpr s8 GfxData::PROPERTY_INDEX[];

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
  { PlayerColor::BLUE, { LSI(MAGIC, 1), LSI(MAPBACK, 14), LSI(MAPBACK, 63), { LSI(MAGIC, 60), LSI(MAGIC, 61), LSI(MAGIC, 62) }, { LSI(RELOAD, 3), {16, 120, 252} }, { {16, 120, 252} } } },
  { PlayerColor::GREEN, { LSI(MAGIC, 2), LSI(MAPBACK, 15), LSI(MAPBACK, 64), { LSI(MAGIC, 63), LSI(MAGIC, 64), LSI(MAGIC, 65) }, { LSI(RELOAD, 4), {0, 188, 0} }, { {0, 188, 0} } } },
  { PlayerColor::PURPLE, { LSI(MAGIC, 3), LSI(MAPBACK, 16), LSI(MAPBACK, 65), { LSI(MAGIC, 66), LSI(MAGIC, 67), LSI(MAGIC, 68) }, { LSI(RELOAD, 5), {164, 84, 164} }, { {164, 84, 164} } } },
  { PlayerColor::RED, { LSI(MAGIC, 4), LSI(MAPBACK, 17), LSI(MAPBACK, 66),  { LSI(MAGIC, 69), LSI(MAGIC, 70), LSI(MAGIC, 71) }, { LSI(RELOAD, 6), {244, 0, 12} }, { {244, 0, 12} } } },
  { PlayerColor::YELLOW, { LSI(MAGIC, 5), LSI(MAPBACK, 18), LSI(MAPBACK, 67), { LSI(MAGIC, 72), LSI(MAGIC, 73), LSI(MAGIC, 74) }, { LSI(RELOAD, 7), {220, 148, 28} }, { {230, 210, 0} } } },
  { PlayerColor::NEUTRAL, { LSI_PLACEHOLD, LSI(MAPBACK, 19), LSI_PLACEHOLD,  { LSI_PLACEHOLD, LSI_PLACEHOLD, LSI_PLACEHOLD }, { LSI(RELOAD, 8), {56, 32, 28} }, { {56, 32, 28} } } }
};
/*
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
};*/

decltype(GfxData::raceHouseSpecs) GfxData::raceHouseSpecs = {
  { HouseType::NORMAL, { LSI(BACKGRND, 37), LSI(BACKGRND, 34), LSI(CITYSCAP, 25), LSI(CITYSCAP, 42), LSI(CMBTCITY, 2) } },
  { HouseType::TREE, { LSI(BACKGRND, 38), LSI(BACKGRND, 35), LSI(CITYSCAP, 30), LSI(CITYSCAP, 43), LSI(CMBTCITY, 12) } },
  { HouseType::MUD, { LSI(BACKGRND, 39), LSI(BACKGRND, 36), LSI(CITYSCAP, 35), LSI(CITYSCAP, 44), LSI(CMBTCITY, 7) } }
};

//TODO: LIFE SPELL (SPECFX, 3) uses palette with multiple white alphas, understand how to manage it
decltype(GfxData::schoolSpecs) GfxData::schoolSpecs = {
  { School::ARCANE, { {0,0,0}, LSI(SPELLSCR, 61), LSI(SPELLS, 9), LSI_PLACEHOLD, LSI_PLACEHOLD, LSI(MAGIC, 48), LSI(SPECFX, 5) } },
  { School::NATURE, { {0,255,0}, LSI(SPELLSCR, 62), LSI(SPELLS, 4), LSI(WIZLAB, 14), LSI(NEWGAME,30), LSI(MAGIC, 33), LSI(SPECFX, 0) } },
  { School::SORCERY, { {0,0,255}, LSI(SPELLSCR, 63), LSI(SPELLS, 5), LSI(WIZLAB, 15), LSI(NEWGAME, 27), LSI(MAGIC, 36), LSI(SPECFX, 1) } },
  { School::CHAOS, { {255,0,0}, LSI(SPELLSCR, 64), LSI(SPELLS, 6), LSI(WIZLAB, 16), LSI(NEWGAME, 36), LSI(MAGIC, 39), LSI(SPECFX, 2) } },
  { School::LIFE, { {255,255,255}, LSI(SPELLSCR, 65), LSI(SPELLS, 7), LSI(WIZLAB, 17), LSI(NEWGAME, 24), LSI(MAGIC, 42), LSI(SPECFX, 3) } },
  { School::DEATH, { {0,0,0}, LSI(SPELLSCR, 66), LSI(SPELLS, 8), LSI(WIZLAB, 18), LSI(NEWGAME, 33), LSI(MAGIC, 45), LSI(SPECFX, 4) } },
};

decltype(GfxData::upkeepSymbolSpec) GfxData::upkeepSymbolSpec = {
  { UpkeepSymbol::FOOD, { LSI(BACKGRND, 40), LSI(BACKGRND, 94), LSI(BACKGRND, 88), LSI(BACKGRND, 95) } },
  { UpkeepSymbol::WORK, { LSI(BACKGRND, 41), LSI_PLACEHOLD, LSI(BACKGRND, 89), LSI_PLACEHOLD } },
  { UpkeepSymbol::GOLD, { LSI(BACKGRND, 42), LSI(BACKGRND, 73), LSI(BACKGRND, 90), LSI(BACKGRND, 93) } },
  { UpkeepSymbol::MANA, { LSI(BACKGRND, 43), LSI_PLACEHOLD, LSI(BACKGRND, 91), LSI_PLACEHOLD } },
  { UpkeepSymbol::RESEARCH, { LSI(BACKGRND, 44), LSI_PLACEHOLD, LSI(BACKGRND, 92), LSI_PLACEHOLD } }
};

decltype(GfxData::unitPropSpecs) GfxData::unitPropSpecs;

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
