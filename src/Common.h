#ifndef _COMMON_H_
#define _COMMON_H_

#include <cstdint>
#include <SDL2/SDL.h>

#define WIDTH (320)
#define HEIGHT (200)

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef u32 Color;
#define RGB(r,g,b) (0xFF000000 | (r << 16) | (g << 8) | b)  // Gfx::color(r,g,b)
#define RGBA(r,g,b,a) ((a << 24) | (r << 16) | (g << 8) | b) //Gfx::colora(r,g,b,a)

enum class I18 : u16;
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
  
  Position() { }
  Position(s16 x, s16 y, Plane plane) : x(x), y(y), plane(plane) { }
  
  bool same(const Position& p) const { return p.x == x && p.y == y && p.plane == plane; }
};

struct PositionOffset
{
  s8 x;
  s8 y;
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
  public:
    mutable s16 gold, mana, food;
    
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

enum ViewID
{
  VIEW_MAIN,
  VIEW_SPELL_BOOK,
  VIEW_MAGIC,
  VIEW_RESEARCH,
  VIEW_ALCHEMY,
  VIEW_ARMIES,
  VIEW_ARMIES_ITEMS,
  VIEW_ITEM_CRAFT,
  VIEW_CITY,
  VIEW_UNIT,
  VIEW_ARMY,
  VIEW_MESSAGE,
  VIEW_MIRROR,
  VIEW_COMBAT,
  VIEW_PRODUCTION,
  VIEW_OUTPOST,
  VIEW_NEW_GAME,
  
  VIEW_COUNT
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
  WizardID ident;
  I18 name;
};

enum class TraitID : u8
{
  ALCHEMY = 0,
  WARLORD,
  CHANNELER,
  ARCHMAGE,
  ARTIFICER,
  CONJURER,
  SAGE_MASTER,
  MYRRAN,
  DIVINE_POWER,
  FAMOUS,
  RUNEMASTER,
  CHARISMATIC,
  CHAOS_MASTERY,
  NATURE_MASTERY,
  SORCERY_MASTERY,
  INFERNAL_POWER,
  MANA_FOCUSING,
  NODE_MASTERY
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

enum TileType : u8
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
  TILE_RIVER_MOUTH,
  
  TILE_TYPES
};

enum TileGfxType : u8
{
  TILE_GFX_NONE = 0,
  TILE_GFX_ANIMATED,
  TILE_GFX_BORDER,
  TILE_GFX_PLAIN
};

enum class Resource : u8
{
  ADAMANTIUM = 0,
  COAL,
  CRYSX_CRYSTAL,
  GEMS,
  GOLD,
  IRON_ORE,
  MITHRIL,
  NIGHT_SHADE,
  QOURK_CRYSTAL,
  SILVER,
  WILD_GAME,
  NONE
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
    static const Trait& trait(const TraitID trait);
    static const Wizard& wizard(const WizardID wizard);
};

#endif
