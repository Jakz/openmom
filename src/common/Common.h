#ifndef _COMMON_H_
#define _COMMON_H_

#include <cassert>
#include <cstdint>
#include <type_traits>
#include <unordered_map>
#include <array>
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

  #define INTEGRITY_CHECK(x) x

#else
  #define LOGD(...) do { } while (false);
  #define LOGD2(...) do { } while (false);
  #define LOGD3(...) do { } while (false);
  #define LOGG(...) do { } while (false);

  #define INTEGRITY_CHECK(x)
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

template<typename... T> using predicate = std::function<bool(T...)>;

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

template<typename K, typename V, size_t SIZE>
class enum_simple_map
{
public:
  static constexpr size_t size = SIZE;
  using init_element = std::pair<K, V>;
  
  struct dummy_pair
  {
    const V& second;
    const dummy_pair* operator->() const { return this; }
  };
  
private:
  std::array<V, size> data;
public:
  enum_simple_map(V value) : data({value}) { }
  
  enum_simple_map(const std::initializer_list<init_element>& elements)
  {
    assert(elements.size() == size);
    size_t i = 0;
    for (auto it = elements.begin(); it != elements.end(); ++i, ++it)
    {
      assert(static_cast<size_t>(it->first) < size);
      data[static_cast<size_t>(it->first)] = it->second;
    }
  }
  
  using iterator = typename std::array<V, size>::iterator;
  using const_iterator = typename std::array<V, size>::const_iterator;
  
  iterator begin() { return data.begin(); }
  const_iterator begin() const { return data.begin(); }
  
  iterator end() { return data.end(); }
  const_iterator end() const { return data.end(); }
  
  void set(K key, const V& value) { data[static_cast<size_t>(key)] = value; }
  
  const V& operator[](K key) const { return data[static_cast<size_t>(key)]; }
  dummy_pair find(K key) const { return { operator[](key) }; }
};

using color_d = u32;
struct Color
{
  union
  {
    struct { u8 b,g,r,a; };
    u32 data;
  };
  
  Color() = default;
  constexpr Color(u8 r, u8 g, u8 b, u8 a = 0xFF) : b(b), g(g), r(r), a(a) { }
  constexpr Color(u32 data) : data(data) { }
  
  Color blend(Color dst) const { return blend(dst, dst.a); }
  
  Color blend(Color dst, float dstAlpha) const {
    float da = dstAlpha, sa = 1.0f - dstAlpha;
    return Color(r*sa + dst.r*da, g*sa + dst.g*da, b*sa + dst.b*da);
  }

  Color blend(Color dst, u8 dstAlpha) const {
    float sa = (255 - dstAlpha) / 255.0f;
    float da = dstAlpha / 255.0f;
    return Color(r*sa + dst.r*da, g*sa + dst.g*da, b*sa + dst.b*da);
  }
  operator u32() const { return data; }
  
  static const Color BLACK, WHITE, NONE;
};

namespace std
{
  template<> struct hash<Color>
  {
    size_t operator()(const Color& color) const { return std::hash<u32>()(color.data); }
  };
}

class FontSpriteSheet;

enum class I18 : u32;
enum class TextureID : u16;
enum Plane : u8;

enum class SpellKind : u8;

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
#define LBXF(x, y, f) (0x80000000 | (static_cast<u32>(f) << 24) | (static_cast<u32>(LBXID::x) << 16) | (y))
#define LBXU(x) SpriteInfo(x)
#define LSI(x, y) SpriteInfo(LBXID::x, y)
#define LSI_PLACEHOLD SpriteInfo(LBXID::COUNT, 0)
#define TSI(x, y, z) SpriteInfo(TextureID::x, y, z)
#define TSIS(x, y) SpriteInfo(TextureID::x, y)

#pragma mark Geometric Types

using int_type = s16;
struct Size;

struct Point
{
  int_type x;
  int_type y;
  
  Point() : x(-1), y(-1) { }
  Point(int_type x, int_type y) : x(x), y(y) { }
  Point(const Size& size);
  template<typename T, typename std::enable_if<std::is_base_of<Point, T>::value, int>::type = 0>
  Point(const T& other) : x(other.x), y(other.y) { }
  
  bool operator!=(const Point& o) const { return !(*this == o); }
  bool operator==(const Point& o) const { return x == o.x && y == o.y; }
  
  Point delta(int_type x, int_type y) const { return Point(this->x + x, this->y + y); }
  
  Point& operator+=(const Point& other) { x += other.x; y += other.y; return *this; }
  Point& operator+=(int_type i) { x += i; y += i; return *this; }
  
  Point operator+(const Point& o) const { return Point(x + o.x, y + o.y); }
  Point operator-(const Point& o) const { return Point(x - o.x, y - o.y); }
  
  Point operator*(float v) const { return Point(x*v, y*v); }
  
  Point operator-(int_type v) const { return Point(x+v, y+v); }
  
  bool isValid() const { return x != -1; }
  
  static Point INVALID;
  static Point ZERO;
};

struct Size
{
  int_type w;
  int_type h;
  
  Size() : w(0), h(0) { }
  Size(int_type w, int_type h) : w(w), h(h) { }
  Size(const Point& point);
  template<typename T, typename std::enable_if<std::is_base_of<Size, T>::value, int>::type = 0>
  Size(const Size& other) : w(other.w), h(other.h) { }
  
  bool operator==(const Size& size) const { return w == size.w && h == size.h; }
  bool operator!=(const Size& size) const { return !this->operator==(size); }
  
  Size operator*(int_type v) const { return Size(this->w*v, this->h*v); }
  Size& operator*=(int_type v) { this->w *= v; this->h *= v; return *this; }
  
  Size operator/(int_type v) const { return Size(this->w/v, this->h/v); }
  Size& operator/=(int_type v) { this->w /= v; this->h /= v; return *this; }
};

using Coord = Point;

struct Rect
{
  Point origin;
  Size size;
  
  Rect(int_type x, int_type y, int_type w, int_type h) : origin(x,y), size(w,h) { }
  Rect(const Point& origin, const Size& size) : origin(origin), size(size) { }
  
  inline int_type right() const { return origin.x + size.w; }
  inline int_type left() const { return origin.x; }
  inline int_type top() const { return origin.y; }
  inline int_type bottom() const { return origin.y + size.h; }
  
  bool contains(const Point& point) const
  {
    return point.x >= left() && point.x < right() && point.y >= top() && point.y <= bottom();
  }
  
  using int_type = ::int_type;
};

inline Point::Point(const Size& size) : x(size.w), y(size.h) { }
inline Size::Size(const Point& point) : w(point.x), h(point.y) { }


constexpr size_t LBX_COUNT = static_cast<size_t>(LBXID::COUNT);

using KeyboardKey = SDL_Keycode;
using KeyboardCode = SDL_Scancode;
using KeyboardMod = SDL_Keymod;
using sdl_text_input = const char (&)[32];

struct Position
{
  s16 x, y;
  Plane plane;
  
  Position() { }
  Position(s16 x, s16 y, Plane plane) : x(x), y(y), plane(plane) { }
  Position(Point pt, Plane plane) : x(pt.x), y(pt.y), plane(plane) { }
  
  Position relative(s16 ox, s16 oy) const { return Position(x+ox, y+oy, plane); }
  Position dx(s16 ox, s16 oy) const { return relative(ox, oy); }
  
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

static size_t COMBAT_DIRS = 8;

inline Dir operator~(const Dir& dir)
{
  switch (dir)
  {
    case Dir::NORTH: return Dir::SOUTH;
    case Dir::NORTH_EAST: return Dir::SOUTH_WEST;
    case Dir::EAST: return Dir::WEST;
    case Dir::SOUTH_EAST: return Dir::NORTH_WEST;
    case Dir::SOUTH: return Dir::NORTH;
    case Dir::SOUTH_WEST: return Dir::NORTH_EAST;
    case Dir::WEST: return Dir::EAST;
    case Dir::NORTH_WEST: return Dir::SOUTH_EAST;
    default: return Dir::INVALID;
  }
}

#pragma mark DirJoin mask management

enum class DirJoin
{
  NONE = 0x00,
  
  N  = 0x01,
  NE = 0x02,
  E  = 0x04,
  SE = 0x08,
  S  = 0x10,
  SW = 0x20,
  W  = 0x40,
  NW = 0x80,
  
  /* orthogonal for map */
  
  HORIZONTAL = W | E,
  VERTICAL = N | S,
  
  CORNER_N_E = N | E,
  CORNER_N_W = N | W,
  CORNER_S_E = S | E,
  CORNER_S_W = S | W,
  
  OCROSS = N | E | W | S,
  
  TCROSS_N = N | W | E,
  TCROSS_S = S | W | E,
  TCROSS_E = N | E | S,
  TCROSS_W = N | W | S,
  
  EDGE_N = NW | N | NE,
  EDGE_S = SW | S | SE,
  EDGE_W = NW | W | SW,
  EDGE_E = NE | E | SE,
  
  
  /* diagonal for combat isometric */
  
  HORIZONTAL_NW_SE = NW | SE,
  HORIZONTAL_NE_SW = NE | SW,
  
  DIAGONAL_NW_SE = NW | SE,
  DIAGONAL_NE_SW = NE | SW,
  
  CORNER_NW_NE = NW | NE,
  CORNER_NE_SE = NE | SE,
  CORNER_SE_SW = SE | SW,
  CORNER_SW_NW = SW | NW,
  
  HALF_NW = NW | NE | SW,
  HALF_NE = NW | NE | SE,
  HALF_SW = SW | SE | NW,
  HALF_SE = SE | SW | NE,
  
  CROSS = NW | SE | NE | SW,
  
  ALL = 0xFF
};

/* these utility functions are used to work with enum class through standard
 boolean operators
 */
inline DirJoin operator~(const DirJoin& lhs)
{
  using utype_t = std::underlying_type<DirJoin>::type;
  return static_cast<DirJoin>(~static_cast<utype_t>(lhs));
}

inline DirJoin operator&(const DirJoin& lhs, const DirJoin& rhs) {
  using utype_t = std::underlying_type<DirJoin>::type;
  return static_cast<DirJoin>(static_cast<utype_t>(lhs) & static_cast<utype_t>(rhs));
}

inline DirJoin operator|(const DirJoin& lhs, const DirJoin& rhs) {
  using utype_t = std::underlying_type<DirJoin>::type;
  return static_cast<DirJoin>(static_cast<utype_t>(lhs) | static_cast<utype_t>(rhs));
}

inline void operator|=(DirJoin& lhs, const DirJoin& rhs) {
  using utype_t = std::underlying_type<DirJoin>::type;
  lhs = static_cast<DirJoin>(static_cast<utype_t>(lhs) | static_cast<utype_t>(rhs));
}

/* shift and rotate left */
inline DirJoin operator<<(DirJoin lhs, u32 v) {
  using utype_t = std::underlying_type<DirJoin>::type;
  constexpr size_t bits = 8;/*sizeof(utype_t)*8;*/
  v %= bits;
  
  utype_t keep = static_cast<utype_t>(lhs) >> v;
  utype_t rotate = static_cast<utype_t>(lhs) << (bits - v);
  
  return static_cast<DirJoin>((keep | rotate) & 0xFF);
}

/* shift and rotate right */
inline DirJoin operator>>(DirJoin lhs, u32 v) {
  using utype_t = std::underlying_type<DirJoin>::type;
  constexpr size_t bits = 8;/*sizeof(utype_t)*8;*/
  v %= bits;
  
  utype_t keep = static_cast<utype_t>(lhs) << v;
  utype_t rotate = static_cast<utype_t>(lhs) >> (bits - v);
  
  return static_cast<DirJoin>((keep | rotate) & 0xFF);
}

inline DirJoin& operator<<=(DirJoin& lhs, u32 v)
{
  lhs = lhs << v;
  return lhs;
}

inline DirJoin& operator>>=(DirJoin& lhs, u32 v)
{
  lhs = lhs >> v;
  return lhs;
}

/* compress to orthogonal */
inline std::underlying_type<DirJoin>::type operator!(const DirJoin& lhs)
{
  return
  ((lhs & DirJoin::N) != DirJoin::NONE ? 0x01 : 0) |
  ((lhs & DirJoin::E) != DirJoin::NONE ? 0x02 : 0) |
  ((lhs & DirJoin::S) != DirJoin::NONE ? 0x04 : 0) |
  ((lhs & DirJoin::W) != DirJoin::NONE ? 0x08 : 0)
  ;
}


#pragma mark SpriteSheet generic interface

class Palette;

class SpriteSheet
{
public:
  virtual u32 at(u16 x, u16 y, u16 c = 0, u16 r = 0) const { return 0x00000000; }
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
  using data_type = u32;
  
private:
  enum : data_type
  {
    /*lbx_palette_flag_mask  = 0x8000000000000000LL,
    lbx_palette_id_mask    = 0x80FF000000000000LL,
    lbx_palette_index_mask = 0x8000FFFF00000000LL,*/
    
    lbx_palette_id_shift = 32 + 16,
    lbx_palette_index_shift = 32,
    
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
    data_type data;
    struct { u16 _index : 16; LBXID _lbx : 8; u8 _frame : 7; bool __is_lbx : 1; } as_lbx;
    struct { u16 _y : 8; u16 _x : 8; TextureID _tex : 15; bool _is_lbx : 1; } as_texture;
  };
  
public:
  SpriteInfo() : SpriteInfo(0) { }
  SpriteInfo(data_type data) : data(data) { }
  
  SpriteInfo& operator=(const SpriteInfo& o) { this->data = o.data; return *this; }
  
  explicit SpriteInfo(TextureID texture, s8 x, s8 y) : data((static_cast<u32>(texture) << texture_id_shift) | (x << texture_x_shift) | (y << texture_y_shift)) { assert(x >= 0 && y >= 0); }
  explicit SpriteInfo(TextureID texture, u16 i);
  
  explicit constexpr SpriteInfo(LBXID lbx, u16 index) : data(lbx_flag_mask | (static_cast<u32>(lbx) << lbx_id_shift) | (index << lbx_index_shift)) { }
  explicit constexpr SpriteInfo(LBXID lbx, u16 index, u8 frame) : data(lbx_flag_mask | (frame << lbx_frame_shift) | (static_cast<u32>(lbx) << lbx_id_shift) | (index << lbx_index_shift)) { assert(frame <= 128); }

  
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
  SpriteInfo lbx(LBXID l) const { return SpriteInfo(l, index(), frame()); }
  
  bool isValid() const { return lbx() < LBXID::COUNT; }
  
  bool operator==(const SpriteInfo& other) const { return data == other.data; }
  bool operator!=(const SpriteInfo& other) const { return data != other.data; }
  
  const SpriteSheet* sheet() const;
};

using lbx_index = s16;
using sprite_ref = SpriteInfo::data_type;

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
  VIEW_ITEM_CRAFT_CHARGES,
  VIEW_MERCHANT,
  VIEW_CITY,
  VIEW_UNIT,
  VIEW_ARMY,
  VIEW_MESSAGE,
  VIEW_CITIES,
  VIEW_MIRROR,
  VIEW_INFO_MENU,
  VIEW_CARTOGRAPHER,
  VIEW_ASTROLOGER,
  VIEW_HISTORIAN,
  VIEW_COMBAT,
  VIEW_PRODUCTION,
  VIEW_OUTPOST,
  VIEW_NEW_GAME,
  VIEW_LOAD,
  VIEW_OPTIONS,
  VIEW_START,
  VIEW_INTRO,
  
  VIEW_MAP_EDITOR,

  VIEW_CONSOLE,
  VIEW_DATA,
  
  VIEW_COUNT
};



#include <vector>
#include <string>

enum School : s8
{
  ARCANE = 0,
  CHAOS,
  DEATH,
  LIFE,
  NATURE,
  SORCERY,
  
  SCHOOL_COUNT,
  SCHOOL_NO_ARCANE_COUNT = 5,
  
  NO_SCHOOL = -1
};

template<typename T> using school_map = enum_simple_map<School, T, SCHOOL_COUNT>;
using school_value_map = school_map<s16>;

enum PlayerColor : u8
{
  GREEN = 0,
  BLUE,
  RED,
  PURPLE,
  YELLOW,
  NEUTRAL,
};

struct Trait
{
  const std::string identifier;
  const u16 cost;
  school_value_map requirement;
  I18 i18n; //TODO: should be moved in GfxData?
  
  Trait(const std::string& identifier, u16 cost) : identifier(identifier), cost(cost), requirement(0) { }
  Trait(const std::string& identifier, u16 cost, School school, u16 booksRequired) : identifier(identifier), cost(cost), requirement(0) { requirement.set(school, booksRequired); }
  Trait(const std::string& identifier, u16 cost, school_value_map&& requirement) : identifier(identifier), cost(cost), requirement(requirement) { }
  
  bool canBePicked(u16 availablePicks, const school_value_map& books) const
  {
    if (availablePicks < cost)
      return false;
    
    auto it1 = requirement.begin(), it2 = books.begin();
    
    for (; it1 != requirement.end(); ++it1, ++it2)
      if (*it1 > *it2)
        return false;
    
    return true;
  }
};

using Retort = Trait;

struct Wizard
{
  school_value_map defaultBooks;
  std::vector<const Retort*> defaultRetorts;
  
  Wizard() : defaultBooks(0) { }
};

struct PlayerSetupInfo
{
  const Wizard* portrait;
  std::string name;
  school_value_map books;
  std::vector<const Retort*> retorts;
  PlayerColor color;
};



#pragma mark Map Related

enum Plane : u8
{
  ARCANUS = 0,
  MYRRAN,
  
  PLANE_COUNT
};

enum class TileType : u8
{
  NONE = 0, 
  GRASS = 1,
  OCEAN,
  SHORE,
  MOUNTAIN,
  VOLCANO,
  HILL,
  FOREST,
  SWAMP,
  DESERT,
  TUNDRA,
  RIVER,
  RIVER_MOUTH,

};

static constexpr size_t TILE_TYPES = 12;

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

enum class PlaceType : u8
{  
  TOWER_OF_WIZARDRY = 1,
  
  MYSTERIOUS_CAVE,
  DUNGEON,
  ANCIENT_TEMPLE,
  ABANDONED_KEEP,
  MONSTER_LAIR,
  RUINS,
  FALLEN_TEMPLE,
  
  MANA_NODE
};

/* forward declarations */

class Race;
class Game;
class City;
class Army;
class Hero;
class ManaNode;
class Unit;

namespace combat
{
  class Combat;
  class CombatUnit;
}

namespace anims
{
  class Animation;
}

namespace msgs
{
  class Message;
}


#endif
