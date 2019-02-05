#pragma once

#include <cstdint>
#include <type_traits>
#include <array>
#include <memory>

#include "common/Common.h"

class Path;

class World;

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

    bit_mask<T> operator&(T flag) const
    {
      bit_mask<T> mask;
      mask.value = this->value & static_cast<utype>(flag);
      return mask;
    }
    
    bit_mask<T> operator|(T flag) const
    {
      bit_mask<T> mask;
      mask.value = this->value | static_cast<utype>(flag);
      return mask;
    }
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
  
  enum class TileResource : u8
  {
    NONE = 0x00,
    
    IRON_ORE = 0x01,
    COAL = 0x02,
    SILVER_ORE = 0x03,
    GOLD_ORE = 0x04,
    GEMS = 0x05,
    MITHRIL = 0x06,
    ADAMANTIUM = 0x07,
    QUORK = 0x08,
    CRYSX = 0x09,
    
    WILD_GAME = 0x40,
    NIGHTSHADE = 0x80
  };
  
  enum TerrainFlag : u8
  {
    VOLCANO_FLAG = 0b00000111,
    VOLCANO_WIZARD1 = 1,
    VOLCANO_WIZARD2 = 2,
    VOLCANO_WIZARD3 = 3,
    VOLCANO_WIZARD4 = 4,
    VOLCANO_WIZARD5 = 5,
    
    ROAD = 0x08,
    ENCHANTED_ROAD = 0x10,
    CORRUPTION = 0x20,
    UNUSED1 = 0x40,
    UNUSED2 = 0x80
  };
  
  using coord_t = s16;
  using spell_id = u16;
  using creature_id = u8;

  STRUCT_PACKING_PUSH
  
  struct HeroStats
  {
    s16 level;
    bit_mask<HeroAbility> abilities;
    u8 castingSkill;
    u8 spells[4];
    u8 unused;
  } PACKED;
  
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
  } PACKED;
    
  struct HiredHero
  {
    enum class ItemSlotType : s16
    {
      NONE = 0,
      SWORD = 1,
      BOW = 2,
      SWORD_STAFF_WAND = 3,
      STAFF_WAND = 4,
      ARMOR_SHIELD = 5,
      AMULET = 6
    };
    
    s16 unitIndex;
    char name[0xE];
    s16 itemIndex[3];
    ItemSlotType itemType[3];
    
  } PACKED;
  
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
  } PACKED;
  
  using AllHeroData = std::array<HeroStats, 35>;
  using TileValue = u16;
  
  struct ManaNodeData
  {
    enum class Type : u8
    {
      SORCERY = 0,
      NATURE = 1,
      CHAOS = 2
    };
    
    enum class Flag : u8
    {
      WARPED = 0x01,
      GUARDIAN_SPIRIT = 0x02
    };
    
    u8 x;
    u8 y;
    u8 plane;
    s8 owner;
    u8 power;
    u8 sparkleX[20];
    u8 sparkleY[20];
    Type type;
    bit_mask<Flag> flags;
    u8 uknown;
  } PACKED;
  
  struct FortressData
  {
    u8 x;
    u8 y;
    u8 plane;
    bool active;
  } PACKED;
  
  struct TowerData
  {
    u8 x;
    u8 y;
    u8 plane;
    bool cleared;
  } PACKED;
  
  struct ItemData
  {
    enum Slot : s8
    {
      // TODO
    };
    
    enum Type : s8
    {
      SWORD,
      MACE,
      AXE,
      BOW,
      STAFF,
      WAND,
      MISC,
      SHIELD,
      CHAIN_MAIL,
      PLATE_MAIL
    };
    
    enum Power : u32
    {
      VAMPIRIC = 0X00000080,
      GUARDIAN_WIND = 0X00000040,
      LIGHTNING = 0X00000020,
      CLOAK_OF_FEAR = 0X00000010,
      DESTRUCTION = 0X00000008,
      WRAITHFORM = 0X00000004,
      REGENERATION = 0X00000002,
      PATHFINDING = 0X00000001,
      WATER_WALKING = 0X00008000,
      RESIST_ELEMENTS = 0X00004000,
      ELEMENTAL_ARMOUR = 0X00002000,
      DOOM = 0X00001000,
      STONING = 0X00000800,
      ENDURANCE = 0X00000400,
      HASTE = 0X00000200,
      INVISIBILITY = 0X00000100,
      DEATH = 0X00800000,
      FLIGHT = 0X00400000,
      RESIST_MAGIC = 0X00200000,
      MAGIC_IMMUNITY = 0X00100000,
      FLAMING = 0X00080000,
      HOLY_AVENGER = 0X00040000,
      TRUE_SIGHT = 0X00020000,
      PHANTASMAL = 0X00010000,
      POWER_DRAIN = 0X80000000,
      BLESS = 0X40000000,
      LIONHEART = 0X20000000,
      GIANT_STRENGTH = 0X10000000,
      PLANAR_TRAVEL = 0X08000000,
      MERGING = 0X04000000,
      RIGHTEOUSNESS = 0X02000000,
      INVULNERABILITY = 0X01000000
    };
    
    char name[30];
    s16 icon;
    Slot slot;
    Type type;
    s16 cost; // -1 = destroyed, 0 = not yet used
    //struct
    //{
      s8 attack;
      s8 toHit;
      s8 defense;
      s8 halfMovements;
      s8 resistance;
      s8 spellPoints;
      s8 spellSave;
    //} bonuses __attribute__((__packed__));
    u8 spellChargesImbuedIndex; // 0 = no spell
    s16 spellChargesAmount;
    bit_mask<Power> powers;
  } PACKED;
    
  struct UnitData
  {
    enum Status : s8
    {
      READY = 0x00,
      PATROL = 0x01,
      BUILDING_ROAD = 0x02,
      GOING_TO = 0x03,
      REACHED_DEST_AND_FINISHED = 0x04,
      WAIT_FOR_ALL_UNITS = 0x05,
      PURIFY = 0x08,
      MELD = 0x09,
      SETTLE = 0x0A,
      SEEK_TRANSPORT = 0x0B,
      MOVE = 0x10, // this can be found ORed with others, eg 0x1C move and road
      PURIFY_ENDED = 0x6F
    };
    
    enum WeaponFlagsAndMutations : u8
    {
      STASIS = 0x80, // lingering, not resisted
      STASIS_INITIAL = 0x40,
      UNDEAD = 0x20,
      CHAOS_CHANNELS_FIRE_BREATH = 0x10,
      CHAOS_CHANNELS_DEMON_WINGS = 0x08,
      CHAOS_CHANNELS_DEMON_SKIN = 0x04,
      WEAPON_ADAMANTIUM = 0x03,
      WEAPON_MITHRIL = 0x02,
      WEAPON_MAGIC = 0x01
    };
    
    enum Enchantments : u32
    {
      PATHFINDING = 0x00000080,
      REGENERATE = 0x00000040,
      WRAITH_FORM = 0x00000020,
      BLACK_CHANNELS = 0x00000010,
      CLOAK_OF_FEAR = 0x00000008,
      BERSERK = 0x00000004,
      GUARDIAN_WIND = 0x00000002,
      IMMOLATION = 0x00000001,
      
      INVISIBILITY = 0x00008000,
      SPELL_LOCK = 0x00004000,
      ENDURANCE = 0x00002000,
      IRON_SKIN = 0x00001000,
      STONE_SKIN = 0x00000800,
      ELEMENTAL_ARMOR = 0x00000400,
      RESIST_ELEMENTS = 0x00000200,
      WATER_WALKING = 0x00000100,
      
      HOLY_WEAPON = 0x00800000,
      TRUE_SIGHT = 0x00400000,
      ELDRITCH_WEAPON = 0x00200000,
      FLAME_BLADE = 0x00100000,
      MAGIC_IMMUNITY = 0x00080000,
      RESIST_MAGIC = 0x00040000,
      FLIGHT = 0x00020000,
      WIND_WALKING = 0x00010000,
      
      INVULNERABLITY = 0x80000000,
      RIGHTEOUSNESS = 0x40000000,
      HOLY_ARMOR = 0x20000000,
      PLANAR_TRAVEL = 0x10000000,
      GIANT_STRENGTH = 0x08000000,
      LION_HEART = 0x04000000,
      BLESS = 0x02000000,
      HEROISM = 0x01000000,
    };
    
    s8 x;
    s8 y;
    s8 plane;
    s8 owner;
    s8 halfMoves;
    u8 icon;
    s8 heroSlot;
    bool stillActive;
    s8 halfMovesLeft;
    s8 destX;
    s8 destY;
    Status status;
    u8 level;
    u8 unkown;
    s16 experience;
    u8 lifeDrainDamageMaybe; // for Ghoul turning
    s8 damage;
    u8 grouping;
    u8 unknownBitmask1;
    u8 unknownBitmask2; // 0 1 in tower without planar seal, 0x10 web
    u8 unknownBitmask3;
    u8 scoutingRange;
    bit_mask<WeaponFlagsAndMutations> weaponFlagsAndMutations;
    bit_mask<Enchantments> enchants;
    u8 turnsToBuildRoad;
    s8 roadStartX;
    s8 roadStartY;
    u8 roadStartParamForBuildAndMove;
  } PACKED;
  
  struct CityData
  {
    enum class Race : s8
    {
      BARBARIAN = 0,
      BEASTMAN,
      DARK_ELF,
      DRACONIAN,
      DWARF,
      GNOLL,
      HALFLING,
      HIGH_ELF,
      HIGH_MAN,
      KLACKON,
      LIZARZMEN,
      NOMAD,
      ORC,
      TROLL
    };
    
    enum class BuildingStatus : s8
    {
      NOT_BUILT = -1,
      REPLACED = 0,
      BUILD = 1,
      DESTROYED = 2
    };
    
    char name[14];
    Race race;
    u8 x;
    u8 y;
    u8 plane;
    s8 owner;
    s8 size;
    s8 populationHigh;
    s8 farmers;
    bool hasSoldBuilding;
    u8 unknown1;
    s8 populationLow;
    u8 unknown2[3];
    s16 currentProduction;
    u8 buildingsCount;
    BuildingStatus buildings[36];
    u8 enchantments[25];
    bool hasNightshade;
    s8 hammers;
    s16 hammersTotal;
    s8 coins;
    s8 upkeep;
    s8 mana;
    s8 research;
    s8 foodProduced;
    u8 connectedCities[0xD];
  } PACKED;
  
  struct PlaceData
  {
    enum class Type : u8
    {
      TOWER = 0,
      CHAOS_NODE = 1,
      NATURE_NODE = 2,
      SORCERY_NODE = 3,
      MYSTERIOUS_CAVE = 4,
      DUNGEON = 5,
      ANCIENT_TEMPLE = 6,
      ABANDONED_KEEP = 7,
      MONSTER_LAIR = 8,
      RUINS = 9,
      FALLEN_TEMPLE = 0xA
    };
    
    enum class Reward : u8
    {
      NONE,
      COMMON_SPELL,
      UNCOMMON_SPELL,
      RARE_SPELL,
      VERY_RARE_SPELL,
      SPELLBOOK_OR_SKILL1,
      SPELLBOOK_OR_SKILL2
    };
    
    struct monster_spec
    {
      u8 quantity : 4;
      u8 left : 4;
    };
    
    u8 x;
    u8 y;
    u8 plane;
    
    bool intact;
    Type type;
    
    creature_id monster1;
    monster_spec monsterSpec1;
    creature_id monster2;
    monster_spec monsterSpec2;
    
    u8 unknown;
    
    u16 goldReward;
    u16 manaReward;
    Reward reward;
    
    u8 flags; // 0x01 prisoner present 0x02 you explored creature 1, 0x04 you explored creature 2
    
    u16 itemsReward;
    u16 itemIndex1;
    u16 itemIndex2;
    u16 itemIndex3;
  } PACKED;
  
  
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
    
    const T& get(Plane plane) const { return plane == Plane::ARCANUS ? arcanus : myrran; }
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
  
  struct SaveGameData
  {
    std::array<AllHeroData, WIZARD_COUNT> heroes;
    
    OverallGameData overallData;
    
    std::array<WizardData, WIZARD_COUNT> wizards;
    WizardData dummyWizard;
    
    map_data<tile_map<TileValue>> tilemap;
    
    map_data<byte[0xC0]> unusedMap;

    map_data<tile_map<u8>> landmass;
    
    std::array<ManaNodeData, 30> manaNodes;
    
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
    
    map_data<tile_map<TileResource>> mapResources;
    map_data<tile_map<bit_mask<FogData>>> mapFog;

    map_data<MovementCostMap> mapMovementCost;
    
    u16 events[50];
    
    map_data<tile_map<bit_mask<TerrainFlag>>> mapTerrainFlags;
    
    s16 grandVizierEnabled;
    
    bool itemsEnabled[250];
    
    std::array<char[0x10], 35> heroNames;
  } PACKED;

  STRUCT_PACKING_POP
  
  class OriginalSaveGame
  {
  private:
    std::unique_ptr<SaveGameData> data;
    
    TileType getTileTypeForTileValue(TileValue value);
    Resource getResourceForValue(TileResource value);
    PlaceType getPlaceTypeForPlaceValue(PlaceData::Type value);
    
  public:
    OriginalSaveGame(const Path& path);
    
    bool isLoaded() const { return data != nullptr; }
    
    World* getWorld();
  };
}
