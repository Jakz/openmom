#pragma once

#include <cstdint>
#include <type_traits>
#include <array>

namespace osave
{
  using u8 = uint8_t;
  using s8 = int8_t;
  using u16 = uint16_t;
  using s16 = int16_t;
  using u32 = uint32_t;
  using byte = u8;
  
  template<typename T>
  struct bit_mask
  {
    using utype = typename std::underlying_type<T>::type;
    utype value;
    
    bool isSet(T flag) const { return value & static_cast<utype>(flag); }
    void set(T flag) { value |= static_cast<utype>(flag); }
    void reset(T flag) { value &= ~static_cast<utype>(flag); }
  };
  
  enum class Race : u8
  {
    BARBARIAN = 0,
    BEASTMAN,
    DARK_ELVES,
    DRACONIANS,
    DWARVES,
    GNOLLS,
    HALFLINGS,
    HIGH_ELVES,
    HIGH_MEN,
    KLACKONS,
    LIZARDMEN,
    NOMADS,
    ORCS,
    TROLLS
  };
  
  enum class HeroAbility : u32
  {
    LEADER         = 0x00000001,
    LEADER_S       = 0x00000002,
    LEGENDARY      = 0x00000008,
    LEGENDARY_S    = 0x00000010,
    BLADEMASTER    = 0x00000040,
    BLADEMASTER_S  = 0x00000080,
    ARMSMASTER     = 0x00000200,
    ARMSMASTER_S   = 0x00000400,
    CONSTITUTION   = 0x00001000,
    CONSTITUTION_S = 0x00002000,
    MIGHT          = 0x00008000,
    MIGHT_S        = 0x00010000,
    ARCANE_POWER   = 0x00040000,
    ARCANE_POWER_S = 0x00080000,
    SAGE           = 0x00200000,
    SAGE_S         = 0x00400000,
    PRAYERMASTER   = 0x01000000,
    PRAYERMASTER_S = 0x02000000,
    AGILITY_S      = 0x04000000,
    LUCKY          = 0x08000000,
    CHARMED        = 0x10000000,
    NOBLE          = 0x20000000,
    FEMALE         = 0x40000000,
    AGILITY        = 0x80000000
  };
  
  enum class WizardPersonality : s16
  {
    MANIACAL = 0,
    RUTHLESS,
    AGGRESSIVE,
    CHAOTIC,
    LAWFUL,
    PEACEFUL
  };
  
  enum class WizardObjective : s16
  {
    PRAGMATIST = 0,
    MILITARIST,
    THEURGIST,
    PERFECTIONIST,
    EXPANSIONIST
  };
  
  enum class Color : s16
  {
    BLUE = 0,
    GREEN,
    PURPLE,
    RED,
    YELLOW
  };
  
  enum class LandSize : u16
  {
    SMALL = 0,
    MEDIUM = 1,
    LARGE = 2
  };
  
  enum class MagicPower : u16
  {
    WEAK = 0,
    NORMAL = 1,
    POWERFUL = 2
  };
  
  enum class Difficulty : u16
  {
    TUTORIAL = 0,
    EASY = 1,
    NORMAL = 2,
    HARD = 3,
    IMPOSSIBLE = 4
  };
  
  enum Plane16 : s16
  {
    ARCANUS = 0,
    MYRRAN = 1
  };
  
  enum FogData : u8
  {
    
  };
  
  enum TileResource : u8
  {
    
  };
  
  enum TerrainFlag : u8
  {
    
  };
  
  using coord_t = s16;
  using spell_id = u16;
  
  struct HeroStats
  {
    s16 level;
    bit_mask<HeroAbility> abilities;
    u8 castingSkill;
    u8 spells[4];
    u8 unused;
  } __attribute__((__packed__));
  
  struct OverallGameData
  {
    byte dummy[0x1b4 - 16];
    
    u16 wizardsCount;
    LandSize landSize;
    MagicPower magicPower;
    Difficulty difficulty;
    u16 cityCount;
    s16 unitCount;
    s16 turn;
    s16 currentActiveUnit;
  } __attribute__((__packed__));
  
  using HiredHero = std::array<byte, 0x1C>;
  
  struct WizardData
  {
    u8 portrait;
    char name[20];
    Race race;
    Color color;
    WizardPersonality personality;
    WizardObjective objective;
    
    byte __unknown1[8];
    
    s16 fame;
    s16 power;
    u16 controlledVolcanoes;
    u8 researchPercent;
    u8 manaPercent;
    u8 skillPercent;
    u8 manaFromVolcanoes;
    
    coord_t summoningCircleX;
    coord_t summoningCircleY;
    Plane16 summoningCirclePlane;
    
    spell_id researchableSpells[8];
    
    s16 __unknown2;
    s16 __unknown3;
    
    s16 garrisonAverageStrength;
    
    s16 __unknown4;
    
    s16 combatSkillLeft;
    s16 spellCastManaLeft;
    s16 spellCastManaCost;
    spell_id spellCast;
    
    s16 unusedCastingSkill;
    s16 nominalCastingSkill;
    
    u16 taxRate; // TODO: to enum
    
    s16 spellBooks[5];
    
    bool traits[18];
    
    HiredHero heroes[6];
    
    u16 __unknown5;
    
    s16 items[4];
    
    bool wizardContacted[6];
    s16 wizardDiplomacy1[6];
    s16 wizardDiplomacy2[6];
    s16 wizardDiplomacy3[6];
    s8 wizardRelations[6];
    s8 wizarWarStatus[6]; // TODO enum
    
    byte __unknown6[0x24];
    s8 wizardDiplomacy4[6];
    s8 wizardDiplomacy5[6];
    
    byte __unknown7[0xCC];
    
    s16 researchLeft;
    s16 mana;
    u32 castingSkill;
    spell_id currentSpellResearch;
    bool spells[214];
    
    byte __unknown8[0x1a];
    
    u16 banishedWizardsMask;
    
    s16 gold;
    
    u16 __unknown9;
    
    u16 astrologerMagicPower;
    u16 astrologerSpellResearch;
    u16 astrologerArmyStrength;
    u16 astrologerPower;
    
    u8 historian[288];
    
    bool enchantments[24];
    
    byte __unknown10[0x2a];
    
    u16 booksColor;
    
    u16 __unknown11;
  } __attribute__((__packed__));
  
  using AllHeroData = std::array<HeroStats, 35>;
  using TileType = u16;
  
  using ManaNode = std::array<byte, 48>;
  using FortressData = std::array<byte, 4>;
  using TowerData = std::array<byte, 4>;
  using PlaceData = std::array<byte, 0x18>;
  using ItemData = std::array<byte, 0x32>;
  using CityData = std::array<byte, 0x72>;
  using UnitData = std::array<byte, 0x20>;
  
  
  constexpr size_t HERO_COUNT = 35;
  constexpr size_t WIZARD_COUNT = 5;
  constexpr size_t MAP_WIDTH = 60;
  constexpr size_t MAP_HEIGHT = 40;
  
  template<typename T> using tile_map = std::array<std::array<T, MAP_WIDTH>, MAP_HEIGHT>;
  
  template<typename T>
  struct map_data
  {
    T arcanus;
    T myrran;
  };
  
  struct MovementCostMap
  {
    tile_map<u8> unused;
    tile_map<u8> walking;
    tile_map<u8> forester;
    tile_map<u8> mountaineer;
    tile_map<u8> swimming;
    tile_map<u8> sailing;
  };
  
  struct SaveGame
  {
    std::array<AllHeroData, WIZARD_COUNT> heroes;
    
    OverallGameData overallData;
    
    std::array<WizardData, WIZARD_COUNT> wizards;
    WizardData dummyWizard;
    
    map_data<tile_map<TileType>> tilemap;
    
    map_data<byte[0xC0]> unusedMap;

    map_data<tile_map<u8>> landmass;
    
    std::array<ManaNode, 30> manaNodes;
    
    std::array<FortressData, 6> fortresses;
    std::array<TowerData, 6> towers;
    
    std::array<PlaceData, 102> places;
    
    std::array<ItemData, 128> items; // (6 heroes x 3 slots + 4 tower) x 5 wizards + unknown
    std::array<byte[0x32], 8> garbage;
    
    ItemData merchantItem;
    ItemData conjuringItem;
  
    std::array<CityData, 100> cities;
    
    std::array<UnitData, 1000> units;
    std::array<UnitData, 9> garbageUnits;
    
    map_data<tile_map<bit_mask<TileResource>>> mapResources;
    map_data<tile_map<bit_mask<FogData>>> mapFog;

    map_data<MovementCostMap> mapMovementCost;
    
    u16 events[50];
    
    map_data<tile_map<bit_mask<TerrainFlag>>> mapTerrainFlags;
    
    s16 grandVizierEnabled;
    
    bool itemsEnabled[250];
    
    std::array<char[0x10], 35> heroNames;
  } __attribute__((__packed__));
}
