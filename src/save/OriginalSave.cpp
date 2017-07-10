
#include "OriginalSave.h"

using namespace osave;

static_assert(sizeof(HeroStats) == 12, "");
static_assert(sizeof(WizardData) == 0x4c8, "");
static_assert(sizeof(OverallGameData) == 0x1b4, "");
static_assert(sizeof(tile_map<osave::TileType>) == 0x12c0, "");
static_assert(sizeof(tile_map<u8>) == 0x960, "");
static_assert(sizeof(osave::ManaNode) == 0x30, "");
static_assert(sizeof(osave::FortressData) == 4, "");
static_assert(sizeof(osave::TowerData) == 4, "");
static_assert(sizeof(osave::PlaceData) == 0x18, "");
static_assert(sizeof(osave::ItemData) == 0x32, "");
static_assert(sizeof(osave::CityData) == 0x72, "");
static_assert(sizeof(osave::UnitData) == 0x20, "");
static_assert(sizeof(SaveGame) == 0x1e1a4, "");

static_assert(sizeof(SaveGame::heroes) == sizeof(HeroStats)*HERO_COUNT*WIZARD_COUNT, "");
static_assert(sizeof(SaveGame::unusedMap) == 0xc0*2, "");

#pragma WizardData check
static_assert(offsetof(WizardData, portrait)               == 0x000, "");
static_assert(offsetof(WizardData, name)                   == 0x001, "");
static_assert(offsetof(WizardData, race)                   == 0x015, "");
static_assert(offsetof(WizardData, color)                  == 0x016, "");
static_assert(offsetof(WizardData, personality)            == 0x018, "");
static_assert(offsetof(WizardData, objective)              == 0x01A, "");
static_assert(offsetof(WizardData, __unknown1)             == 0x01C, "");
static_assert(offsetof(WizardData, fame)                   == 0x024, "");
static_assert(offsetof(WizardData, power)                  == 0x026, "");
static_assert(offsetof(WizardData, controlledVolcanoes)    == 0x028, "");
static_assert(offsetof(WizardData, researchPercent)        == 0x02a, "");
static_assert(offsetof(WizardData, manaPercent)            == 0x02b, "");
static_assert(offsetof(WizardData, skillPercent)           == 0x02c, "");
/* ... */
static_assert(offsetof(WizardData, taxRate)                == 0x058, "");
/* ... */
static_assert(offsetof(WizardData, researchLeft)           == 0x25a, "");
/* ... */
static_assert(offsetof(WizardData, historian)              == 0x362, "");
static_assert(offsetof(WizardData, enchantments)           == 0x482, "");
static_assert(offsetof(WizardData, __unknown10)            == 0x49a, "");
static_assert(offsetof(WizardData, booksColor)             == 0x4c4, "");
static_assert(offsetof(WizardData, __unknown11)            == 0x4c6, "");

#pragma SaveGame check
static_assert(offsetof(SaveGame, heroes)             == 0x00000, "");
static_assert(offsetof(SaveGame, overallData)        == 0x00834, "");
static_assert(offsetof(SaveGame, wizards)            == 0x009e8, "");
static_assert(offsetof(SaveGame, dummyWizard)        == 0x021d0, "");
static_assert(offsetof(SaveGame, tilemap)            == 0x02698, "");
static_assert(offsetof(SaveGame, unusedMap)          == 0x04c18, "");
static_assert(offsetof(SaveGame, landmass)           == 0x04d98, "");
static_assert(offsetof(SaveGame, manaNodes)          == 0x06058, "");
static_assert(offsetof(SaveGame, fortresses)         == 0x065f8, "");
static_assert(offsetof(SaveGame, towers)             == 0x06610, "");
static_assert(offsetof(SaveGame, places)             == 0x06628, "");
static_assert(offsetof(SaveGame, items)              == 0x06fb8, "");
static_assert(offsetof(SaveGame, garbage)            == 0x088b8, "");
static_assert(offsetof(SaveGame, merchantItem)       == 0x08a48, "");
static_assert(offsetof(SaveGame, conjuringItem)      == 0x08a7a, "");
static_assert(offsetof(SaveGame, cities)             == 0x08aac, "");
static_assert(offsetof(SaveGame, units)              == 0x0b734, "");
static_assert(offsetof(SaveGame, garbageUnits)       == 0x13434, "");
static_assert(offsetof(SaveGame, mapResources)       == 0x13554, "");
static_assert(offsetof(SaveGame, mapFog)             == 0x14814, "");
static_assert(offsetof(SaveGame, mapMovementCost)    == 0x15ad4, "");
static_assert(offsetof(SaveGame, events)             == 0x1cb54, "");
static_assert(offsetof(SaveGame, mapTerrainFlags)    == 0x1cbb8, "");
static_assert(offsetof(SaveGame, grandVizierEnabled) == 0x1de78, "");
static_assert(offsetof(SaveGame, itemsEnabled)       == 0x1de7a, "");
static_assert(offsetof(SaveGame, heroNames)          == 0x1df74, "");


