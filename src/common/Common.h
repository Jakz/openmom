#ifndef _COMMON_H_
#define _COMMON_H_

#include <cassert>
#include <cstdint>
#include "SDL.h"

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

constexpr Color RGB(u32 r, u32 g, u32 b) { return 0xFF000000 | (r << 16) | (g << 8) | b; }
constexpr Color RGB16(u32 r, u32 g, u32 b) { return RGB(r<<2, g<<2, b<<2); }
constexpr Color RGBA(u32 r, u32 g, u32 b, u32 a) { return (a << 24) | (r << 16) | (g << 8) | b; }
constexpr u8 GET_RED(Color color) { return (color & 0x00FF0000) >> 16; }
constexpr u8 GET_GREEN(Color color) { return (color & 0x0000FF00) >> 8; }
constexpr u8 GET_BLUE(Color color) { return (color & 0x000000FF); }

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

enum LBXFileID : u16;
struct LBXSpriteInfo
{
  LBXFileID lbx;
  u16 index;
  LBXSpriteInfo(LBXFileID lbx, u16 index) : lbx(lbx), index(index) { }
  
  LBXSpriteInfo relative(s16 delta) const { return LBXSpriteInfo(lbx, index+delta); }
};

enum LBXFileID : u16
{
  LBX_ARMYLIST = 0,
  LBX_BACKGRND,
  LBX_UNITS1,
  LBX_UNITS2,
  LBX_MAINSCRN,
  LBX_MAIN,
  LBX_SPECFX,
  LBX_FIGURE1,
  LBX_FIGURE2,
  LBX_FIGURE3,
  LBX_FIGURE4,
  LBX_FIGURE5,
  LBX_FIGURE6,
  LBX_FIGURE7,
  LBX_FIGURE8,
  LBX_FIGURE9,
  LBX_FIGURE10,
  LBX_FIGURE11,
  LBX_FIGURE12,
  LBX_SPELLSCR,
  
  LBX_COUNT
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

struct ScreenCoord
{
  s16 x;
  s16 y;
  
  ScreenCoord(s16 x, s16 y) : x(x), y(y) { }
};

using Coord = ScreenCoord;

class Palette;

class SpriteSheet
{
public:
  virtual Color at(u16 x, u16 y, u16 c = 0, u16 r = 0) const { return 0x00000000; }
  virtual void set(u16 x, u16 y, Color c) { }
  virtual ~SpriteSheet() { }
  virtual u16 tw() const = 0;
  virtual u16 th() const = 0;
  
  virtual u16 sw(u16 r, u16 c) const = 0;
  virtual u16 sh(u16 r, u16 c) const = 0;
  
  virtual void lock() const { }
  virtual void unlock() const { }
  
  virtual const Palette* getPalette() const { return nullptr; }
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
  
  VIEW_LBX,
  
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
