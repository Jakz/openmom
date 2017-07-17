
#include "OriginalSave.h"

#include "platform/Path.h"


#include "common/Common.h"
#include "game/world/World.h"
#include "Viewport.h"

TileType osave::OriginalSaveGame::getTileTypeForTileValue(TileValue value)
{
  using namespace std;
  
  static constexpr size_t MAP_TILE_PER_PLANE = 0x2FA;
  static std::array<TileType, MAP_TILE_PER_PLANE> worldTileMapping = { TileType::NONE };
  
  if (worldTileMapping[0] == TileType::NONE)
  {
    const auto& mapping = Viewport::arcanus;
    
    for_each(mapping.desert.begin(), mapping.desert.end(), [](const gfx_tile_t& gfx) { worldTileMapping[gfx.index()] = TileType::DESERT; });
    for_each(mapping.tundra.begin(), mapping.tundra.end(), [](const gfx_tile_t& gfx) { worldTileMapping[gfx.index()] = TileType::TUNDRA; });
    for_each(mapping.swamp.begin(), mapping.swamp.end(), [](const gfx_tile_t& gfx) { worldTileMapping[gfx.index()] = TileType::SWAMP; });
    for_each(mapping.forest.begin(), mapping.forest.end(), [](const gfx_tile_t& gfx) { worldTileMapping[gfx.index()] = TileType::FOREST; });
    for_each(mapping.grasslands.begin(), mapping.grasslands.end(), [](const gfx_tile_t& gfx) { worldTileMapping[gfx.index()] = TileType::GRASS; });
    for_each(mapping.ocean.begin(), mapping.ocean.end(), [](const gfx_tile_t& gfx) { worldTileMapping[gfx.index()] = TileType::OCEAN; });

    for_each(mapping.hills.begin(), mapping.hills.end(), [](const gfx_tile_t& gfx) { worldTileMapping[gfx.index()] = TileType::HILL; });
    for_each(mapping.mountains.begin(), mapping.mountains.end(), [](const gfx_tile_t& gfx) { worldTileMapping[gfx.index()] = TileType::MOUNTAIN; });

    for_each(mapping.shores.begin()+1, mapping.shores.end(), [](const gfx_tile_t& gfx) { worldTileMapping[gfx.index()] = TileType::SHORE; });
    for_each(mapping.desertJoin.begin()+1, mapping.desertJoin.end(), [](const gfx_tile_t& gfx) { worldTileMapping[gfx.index()] = TileType::DESERT; });
    for_each(mapping.tundraJoin.begin()+1, mapping.tundraJoin.end(), [](const gfx_tile_t& gfx) { worldTileMapping[gfx.index()] = TileType::TUNDRA; });
    
    //TODO: missing rivers, nodes etc
  }
  
  if (value >= MAP_TILE_PER_PLANE) value -= MAP_TILE_PER_PLANE;
  
  TileType type = worldTileMapping[value];
  
  return type != TileType::NONE ? type : TileType::GRASS;
}

Resource osave::OriginalSaveGame::getResourceForValue(TileResource value)
{
  switch (value) {
    case TileResource::IRON_ORE: return Resource::IRON_ORE;
    case TileResource::COAL: return Resource::COAL;
    case TileResource::SILVER_ORE: return Resource::SILVER;
    case TileResource::GOLD_ORE: return Resource::GOLD;
    case TileResource::GEMS: return Resource::GEMS;
    case TileResource::MITHRIL: return Resource::MITHRIL;
    case TileResource::ADAMANTIUM: return Resource::ADAMANTIUM;
    case TileResource::QUORK: return Resource::QOURK_CRYSTAL;
    case TileResource::CRYSX: return Resource::CRYSX_CRYSTAL;
    case TileResource::WILD_GAME: return Resource::WILD_GAME;
    case TileResource::NIGHTSHADE: return Resource::NIGHT_SHADE;
      
    default:
      // TODO: values 0A-0E seems related to sparkles on tile?
      return Resource::NONE;
  }
}

osave::OriginalSaveGame::OriginalSaveGame(const Path& path)
{
  if (path.exists())
  {
    file_handle handle(path, file_mode::READING);
    
    if (handle.length() == sizeof(SaveGameData))
    {
      data = std::unique_ptr<SaveGameData>(new SaveGameData());
      handle.read(data.get(), sizeof(SaveGameData), 1);
    }
    
    handle.close();
  }
}

World* osave::OriginalSaveGame::getWorld()
{
  if (!isLoaded())
    return nullptr;
  else
  {
    const u16 w = 60, h = 40;
    
    World* world = new World(nullptr, w, h);
    
    for (u16 p = 0; p < PLANE_COUNT; ++p)
    {
      Plane plane = (Plane)p;
      const auto& tileMap = data->tilemap.get(plane);
      const auto& resourceMap = data->mapResources.get(plane);
      
      for (u16 y = 0; y < h; ++y)
      {
        for (u16 x = 0; x < w; ++x)
        {
          Tile* tile = world->get(x, y, plane);
          
          tile->type = getTileTypeForTileValue(tileMap[y][x]);
          tile->resource = getResourceForValue(resourceMap[y][x]);
          
        }
      }
    }
    
    world->calcSubTiles();
    return world;
  }
}


#pragma Original Save Game Checks

using namespace osave;

static_assert(sizeof(HeroStats) == 12, "");
static_assert(sizeof(WizardData) == 0x4c8, "");
static_assert(sizeof(OverallGameData) == 0x1b4, "");
static_assert(sizeof(tile_map<TileValue>) == 0x12c0, "");
static_assert(sizeof(tile_map<u8>) == 0x960, "");
static_assert(sizeof(osave::ManaNode) == 0x30, "");
static_assert(sizeof(osave::FortressData) == 4, "");
static_assert(sizeof(osave::TowerData) == 4, "");
static_assert(sizeof(osave::PlaceData) == 0x18, "");
static_assert(sizeof(osave::ItemData) == 0x32, "");
static_assert(sizeof(osave::CityData) == 0x72, "");
static_assert(sizeof(osave::UnitData) == 0x20, "");
static_assert(sizeof(SaveGameData) == 0x1e1a4, "");

static_assert(sizeof(SaveGameData::heroes) == sizeof(HeroStats)*HERO_COUNT*WIZARD_COUNT, "");
static_assert(sizeof(SaveGameData::unusedMap) == 0xc0*2, "");

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
static_assert(offsetof(SaveGameData, heroes)             == 0x00000, "");
static_assert(offsetof(SaveGameData, overallData)        == 0x00834, "");
static_assert(offsetof(SaveGameData, wizards)            == 0x009e8, "");
static_assert(offsetof(SaveGameData, dummyWizard)        == 0x021d0, "");
static_assert(offsetof(SaveGameData, tilemap)            == 0x02698, "");
static_assert(offsetof(SaveGameData, unusedMap)          == 0x04c18, "");
static_assert(offsetof(SaveGameData, landmass)           == 0x04d98, "");
static_assert(offsetof(SaveGameData, manaNodes)          == 0x06058, "");
static_assert(offsetof(SaveGameData, fortresses)         == 0x065f8, "");
static_assert(offsetof(SaveGameData, towers)             == 0x06610, "");
static_assert(offsetof(SaveGameData, places)             == 0x06628, "");
static_assert(offsetof(SaveGameData, items)              == 0x06fb8, "");
static_assert(offsetof(SaveGameData, garbage)            == 0x088b8, "");
static_assert(offsetof(SaveGameData, merchantItem)       == 0x08a48, "");
static_assert(offsetof(SaveGameData, conjuringItem)      == 0x08a7a, "");
static_assert(offsetof(SaveGameData, cities)             == 0x08aac, "");
static_assert(offsetof(SaveGameData, units)              == 0x0b734, "");
static_assert(offsetof(SaveGameData, garbageUnits)       == 0x13434, "");
static_assert(offsetof(SaveGameData, mapResources)       == 0x13554, "");
static_assert(offsetof(SaveGameData, mapFog)             == 0x14814, "");
static_assert(offsetof(SaveGameData, mapMovementCost)    == 0x15ad4, "");
static_assert(offsetof(SaveGameData, events)             == 0x1cb54, "");
static_assert(offsetof(SaveGameData, mapTerrainFlags)    == 0x1cbb8, "");
static_assert(offsetof(SaveGameData, grandVizierEnabled) == 0x1de78, "");
static_assert(offsetof(SaveGameData, itemsEnabled)       == 0x1de7a, "");
static_assert(offsetof(SaveGameData, heroNames)          == 0x1df74, "");
