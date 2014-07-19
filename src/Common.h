#ifndef _COMMON_H_
#define _COMMON_H_

#include <cstdint>
#include <SDL2/SDL.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef u32 Color;
#define RGB(r,g,b) Gfx::color(r,g,b)
#define RGBA(r,g,b,a) Gfx::colora(r,g,b,a)


enum I18 : u16;
enum TextureID : u16;
enum Plane : u8;

enum SpellKind : u8;

enum MouseButton : u8
{
  BUTTON_LEFT = SDL_BUTTON_LEFT,
  BUTTON_RIGHT = SDL_BUTTON_RIGHT,
  BUTTON_MIDDLE = SDL_BUTTON_MIDDLE
};

typedef SDL_Scancode KeyboardKey;
typedef SDL_Keymod KeyboardMod;


struct Position
{
  s16 x, y;
  Plane plane;
};

struct SpriteInfo
{
  TextureID texture;
  s16 x;
  s16 y;
  
  SpriteInfo(TextureID texture, s16 x, s16 y) : texture(texture), x(x), y(y) { }
};

class Upkeep
{
  private:
    s16 gold, mana, food;
    
  public:
    Upkeep(s16 gold = 0, s16 mana = 16, s16 food = 0) : gold(gold), mana(mana), food(food) { }
    
  public:
    void add(const Upkeep &upkeep)
    {
      gold += upkeep.gold;
      mana += upkeep.mana;
      food += upkeep.food;
    }
};




#include <vector>


enum School : s8
{
  ARCANE = 0,
  CHAOS,
  DEATH,
  LIFE,
  NATURE,
  SORCERY,
  
  SCHOOL_COUNT,
  
  NO_SCHOOL = -1
};

enum PlayerColor : u8
{
  GREEN = 0,
  BLUE,
  RED,
  PURPLE,
  YELLOW,
  NEUTRAL
};

enum WizardID : u8
{
  MERLIN = 0,
  RAVEN,
  SHAREE,
  LO_PAN,
  JAFAR,
  OBERIC,
  RJAK,
  SSS_RA,
  TAURON,
  FREYA,
  HORUS,
  ARIEL,
  TLALOC,
  KALI
};

struct Wizard
{
  I18 name;
};

enum TraitID : u8
{
  TRAIT_ALCHEMY = 0,
  TRAIT_WARLORD,
  TRAIT_CHANNELER,
  TRAIT_ARCHMAGE,
  TRAIT_ARTIFICER,
  TRAIT_CONJURER,
  TRAIT_SAGE_MASTER,
  TRAIT_MYRRAN,
  TRAIT_DIVINE_POWER,
  TRAIT_FAMOUS,
  TRAIT_RUNEMASTER,
  TRAIT_CHARISMATIC,
  TRAIT_CHAOS_MASTERY,
  TRAIT_NATURE_MASTERY,
  TRAIT_SORCERY_MASTERY,
  TRAIT_INFERNAL_POWER,
  TRAIT_MANA_FOCUSING,
  TRAIT_NODE_MASTERY
};

struct Trait
{
  const TraitID ident;
  const u8 cost;
  const School school;
};


#pragma mark Map Related

enum Plane : u8
{
  ARCANUS = 0,
  MYRRAN,
  
  PLANE_COUNT
};

enum TileType
{
  TILE_GRASS = 0,
  TILE_WATER,
  TILE_SHORE,
  TILE_MOUNTAIN,
  TILE_VOLCANO,
  TILE_HILL,
  TILE_FOREST,
  TILE_SWAMP,
  TILE_DESERT,
  TILE_TUNDRA,
  TILE_RIVER,
  TILE_RIVER_MOUTH
};

enum TileGfxType
{
  TILE_GFX_NONE = 0,
  TILE_GFX_ANIMATED,
  TILE_GFX_BORDER,
  TILE_GFX_PLAIN
};

enum TileResource : u8
{
  RESOURCE_ADAMANTIUM = 0,
  RESOURCE_COAL,
  RESOURCE_CRYSX_CRYSTAL,
  RESOURCE_GEMS,
  RESOURCE_GOLD,
  RESOURCE_IRON_ORE,
  RESOURCE_MITHIL,
  RESOURCE_NIGHT_SHADE,
  RESOURCE_QOURK_CRYSTAL,
  RESOURCE_SILVER,
  RESOURCE_WILD_GAME
};

enum PlaceType : u8
{
  PLACE_TOWER_OF_WIZARDRY = 0,
  PLACE_TOWER_OF_WIZARDRY_EMPTY,
  PLACE_CAVE,
  PLACE_RUINS,
  PLACE_KEEP,
  PLACE_TEMPLE,
  PLACE_ANCIENT_RUINS
};


class Data
{
  public:
    static Color colorForSchool(const School school);
    static const Trait& trait(const TraitID trait);
    static const Wizard& wizard(const WizardID wizard);
  
    static const std::vector<const SpellKind>& spellKinds(bool combat);
};

#endif
