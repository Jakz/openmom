#ifndef _COMMON_H_
#define _COMMON_H_

#include <cassert>
#include <cstdint>
#include <type_traits>
#include "SDL.h"

#ifdef DEBUG
  extern void debugprintf(const char* str, ...);
  #define LOGD(...) debugprintf(__VA_ARGS__);
  #define LOGG(y, x, ...) LOGD("[game]%s " x, y, __VA_ARGS__);

  #if DEBUG >= 3
    #define LOGD3(...) debugprintf(__VA_ARGS__);
  #else
    #define LOGD3(...) do { } while (false);
  #endif

  #if DEBUG >= 2
    #define LOGD2(...) debugprintf(__VA_ARGS__);
  #else
    #define LOGD2(...) do { } while (false);
  #endif
#else
  #define LOGD(...) do { } while (false);
  #define LOGD2(...) do { } while (false);
  #define LOGD3(...) do { } while (false);
  #define LOGG(...) do { } while (false);
#endif

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
using byte = u8;

template<typename T>
class optional
{
private:
  T _value;
  bool _isPresent;
  
public:
  optional(T value) : _value(value), _isPresent(true) { }
  optional() : _isPresent(false) { }
  optional& operator=(T value) { _value = value; _isPresent = true; return *this; }
  bool isPresent() const { return _isPresent; }
  operator T() const { assert(_isPresent); return _value; }
  T* operator->() { assert(_isPresent); return &_value; }
  const T* operator->() const { assert(_isPresent); return &_value; }
};

struct enum_hash
{
  template <typename T>
  inline
  typename std::enable_if<std::is_enum<T>::value, size_t>::type
  operator ()(T const value) const
  {
    return static_cast<size_t>(static_cast<size_t>(value));
  }
};

typedef u32 Color;

using lbx_index = s16;
using sprite_ref = u32;

constexpr Color RGB(u32 r, u32 g, u32 b) { return 0xFF000000 | (r << 16) | (g << 8) | b; }
constexpr Color RGB16(u32 r, u32 g, u32 b) { return RGB(r<<2, g<<2, b<<2); }
constexpr Color RGBA(u32 r, u32 g, u32 b, u32 a) { return (a << 24) | (r << 16) | (g << 8) | b; }
constexpr u8 GET_RED(Color color) { return (color & 0x00FF0000) >> 16; }
constexpr u8 GET_GREEN(Color color) { return (color & 0x0000FF00) >> 8; }
constexpr u8 GET_BLUE(Color color) { return (color & 0x000000FF); }

enum class I18 : u16;
enum class TextureID : u16;
enum Plane : u8;

enum SpellKind : u8;

enum MouseButton : u8
{
  BUTTON_LEFT = SDL_BUTTON_LEFT,
  BUTTON_RIGHT = SDL_BUTTON_RIGHT,
  BUTTON_MIDDLE = SDL_BUTTON_MIDDLE
};

enum class LBXID : u8
{
  ARMYLIST = 0,
  BACKGRND,
  BOOK,
  BUILDDAT,
  BUILDESC,
  CHRIVER,
  CITYNAME,
  CITYSCAP,
  CITYWALL,
  CMBDESRC,
  CMBDESRT,
  CMBGRASC,
  CMBGRASS,
  CMBMAGIC,
  CMBMOUNC,
  CMBMOUNT,
  CMBTCITY,
  CMBTFX,
  CMBTSND,
  CMBTUNDC,
  CMBTUNDR,
  CMBTWALL,
  COMBAT,
  COMPIX,
  CONQUEST,
  DESC,
  DESERT,
  DIPLOMAC,
  DIPLOMSG,
  EVENTMSG,
  EVENTS,
  FIGURES1,
  FIGURES2,
  FIGURES3,
  FIGURES4,
  FIGURES5,
  FIGURES6,
  FIGURES7,
  FIGURES8,
  FIGURES9,
  FIGURE10,
  FIGURE11,
  FIGURE12,
  FIGURE13,
  FIGURE14,
  FIGURE15,
  FIGURE16,
  FONTS,
  HALOFAM,
  HELP,
  HERODATA,
  HIRE,
  HLPENTRY,
  INTRO,
  INTROSFX,
  INTROSND,
  ITEMDATA,
  ITEMISC,
  ITEMPOW,
  ITEMS,
  LILWIZ,
  LISTDAT,
  LOAD,
  LOSE,
  MAGIC,
  MAIN,
  MAINSCRN,
  MAPBACK,
  MESSAGE,
  MONSTER,
  MOODWIZ,
  MUSIC,
  NAMES,
  NEWGAME,
  NEWSOUND,
  PORTRAIT,
  RELOAD,
  RESOURCE,
  SCROLL,
  SNDDRV,
  SOUNDFX,
  SPECFX,
  SPECIAL,
  SPECIAL2,
  SPELLDAT,
  SPELLOSE,
  SPELLS,
  SPELLSCR,
  SPLMASTR,
  TERRAIN,
  TERRSTAT,
  TERRTYPE,
  TUNDRA,
  UNITS1,
  UNITS2,
  UNITVIEW,
  VORTEX,
  WALLRISE,
  WIN,
  WIZARDS,
  WIZLAB,
  
  COUNT
};

#define LBXI(x, y) (0x80000000 | (static_cast<u32>(LBXID::x) << 16) | (y))
#define LBXU(x) SpriteInfo(x)
#define LSI(x, y) SpriteInfo(LBXID::x, y)
#define LSI_PLACEHOLD SpriteInfo(LBXID::COUNT, 0)
#define TSI(x, y, z) SpriteInfo(TextureID::x, y, z)
#define TSIS(x, y) SpriteInfo(TextureID::x, y)


constexpr size_t LBX_COUNT = static_cast<size_t>(LBXID::COUNT);

using KeyboardKey = SDL_Keycode;
using KeyboardCode = SDL_Scancode;
using KeyboardMod = SDL_Keymod;

struct Position
{
  s16 x, y;
  Plane plane;
  
  Position() { }
  Position(s16 x, s16 y, Plane plane) : x(x), y(y), plane(plane) { }
  
  Position relative(s16 ox, s16 oy) const { return Position(x+ox, y+oy, plane); }
  
  bool wrapAndCheckValidity(u16 w, u16 h)
  {
    if (y < 0 || y >= h) return false;
    else
    {
      if (x < 0)
        x = w + x;
      else if (x >= w)
        x = x%w;
      return true;
    }
  }
  
  bool same(const Position& p) const { return p.x == x && p.y == y && p.plane == plane; }
};

struct PositionOffset
{
  s8 x;
  s8 y;
};

enum class Dir
{
  NORTH = 0,
  NORTH_EAST,
  EAST,
  SOUTH_EAST,
  SOUTH,
  SOUTH_WEST,
  WEST,
  NORTH_WEST,
  
  INVALID = -1
};

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

struct SpriteInfo
{
private:
  enum : u32
  {
    lbx_flag_mask   = 0x80000000,
    lbx_id_mask     = 0x00FF0000,
    lbx_frame_mask  = 0x7F000000,
    lbx_index_mask  = 0x0000FFFF,
    
    texture_id_mask = lbx_id_mask,
    texture_x_mask  = 0x000000FF,
    texture_y_mask  = 0x0000FF00,
    
    lbx_id_shift     = 16,
    lbx_frame_shift  = 24,
    texture_id_shift = 16,

    lbx_index_shift = 0,
    
    texture_x_shift = 0,
    texture_y_shift = 8,
  };

  union
  {
    u32 data;
    struct { u16 _index : 16; LBXID _lbx : 8; u8 _frame : 7; bool __is_lbx : 1; } as_lbx;
    struct { u16 _y : 8; u16 _x : 8; TextureID _tex : 15; bool _is_lbx : 1; } as_texture;
  };
  
public:
  SpriteInfo() : SpriteInfo(0) { }
  SpriteInfo(decltype(data) data) : data(data) { }
  
  SpriteInfo& operator=(const SpriteInfo& o) { this->data = o.data; return *this; }
  
  explicit SpriteInfo(TextureID texture, s8 x, s8 y) : data((static_cast<u32>(texture) << texture_id_shift) | (x << texture_x_shift) | (y << texture_y_shift)) { assert(x >= 0 && y >= 0); }
  explicit SpriteInfo(TextureID texture, u16 i);
  
  explicit SpriteInfo(LBXID lbx, u16 index) : data(lbx_flag_mask | (static_cast<u32>(lbx) << lbx_id_shift) | (index << lbx_index_shift)) { }
  explicit SpriteInfo(LBXID lbx, u16 index, u8 frame) : data(lbx_flag_mask | (frame << lbx_frame_shift) | (static_cast<u32>(lbx) << lbx_id_shift) | (index << lbx_index_shift)) { assert(frame <= 128); }

  
  bool isLBX() const { return data & lbx_flag_mask; }
  
  TextureID texture() const { return static_cast<TextureID>((data & texture_id_mask) >> texture_id_shift); }
  
  u16 x() const { return isLBX() ? 0 : (data & texture_x_mask) >> texture_x_shift; }
  u16 y() const { return isLBX() ? frame() : (data & texture_y_mask) >> texture_y_shift; }
  
  LBXID lbx() const { return static_cast<LBXID>((data & lbx_id_mask) >> lbx_id_shift); }
  u16 index() const { return (data & lbx_index_mask) >> lbx_index_shift; }
  u8 frame() const { return (data & lbx_frame_mask) >> lbx_frame_shift; }
  
  u16 sw() const { return sheet()->sw(x(), y()); }
  u16 sh() const { return sheet()->sh(x(), y()); }
  
  const Palette* palette() const;
  
  u16 count() const;
  
  SpriteInfo relative(s16 offset) const { return SpriteInfo(lbx(), index()+offset); }
  SpriteInfo frame(s16 offset, u8 f) const { return SpriteInfo(lbx(), index()+offset, f); }
  SpriteInfo frame(u8 f) const { return SpriteInfo(lbx(), index(), f); }
  
  const SpriteSheet* sheet() const;
};

struct ScreenCoord
{
  s16 x;
  s16 y;
  
  ScreenCoord() : x(-1), y(-1) { }
  ScreenCoord(s16 x, s16 y) : x(x), y(y) { }
  bool operator==(const ScreenCoord& o) const { return x == o.x && y == o.y; }
  ScreenCoord& operator+=(s16 i) { x += i; y += i; return *this; }
  
  bool isValid() const { return x != -1; }
  
  static ScreenCoord INVALID;
  static ScreenCoord ZERO;
};

using Coord = ScreenCoord;

class Upkeep
{
public:
  enum class Type { GOLD, MANA, FOOD };
  
  mutable s16 gold, mana, food;
  
  Upkeep(s16 gold = 0, s16 mana = 16, s16 food = 0) : gold(gold), mana(mana), food(food) { }

  s16 operator[](Type type) const
  {
    switch (type)
    {
      case Type::GOLD: return gold;
      case Type::MANA: return mana;
      case Type::FOOD: return food;
    }
    assert(false);
  }
  
  Upkeep& operator+=(const Upkeep &upkeep)
  {
    gold += upkeep.gold;
    mana += upkeep.mana;
    food += upkeep.food;
    return *this;
  }
  
  Upkeep operator+(const Upkeep& o) const { return Upkeep(gold+o.gold, mana+o.mana, food+o.food); }
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
  VIEW_CONSOLE,
    
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
  NEUTRAL,
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
  PLACE_TOWER_OF_WIZARDRY_CLEARED,
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
