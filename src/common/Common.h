#ifndef _COMMON_H_
#define _COMMON_H_

#include <cassert>
#include <cstdint>
#include <type_traits>
#include <functional>
#include <unordered_map>
#include <array>
#include <algorithm>
#include "SDL.h"

//#define OM_CONFIG_DONT_USE_STRING_VIEW false


#define DEBUG 3

#if defined(DEBUG) || true
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

  #define DEBUG_ASSERT(x, y) if (!(x)) { LOGD("Assertion failed: %s", y); abort(); }

#else
  #define LOGD(...) do { } while (false);
  #define LOGD2(...) do { } while (false);
  #define LOGD3(...) do { } while (false);
  #define LOGG(...) do { } while (false);

  #define DEBUG_ASSERT(x)
#endif

#ifdef _MSC_VER
#define STRUCT_PACKING_PUSH __pragma(pack(push,1))
#define STRUCT_PACKING_POP __pragma(pack(pop))
#define PACKED
#define PACKED_STRUCTS_REQUIRE_SAME_TYPE true
#else
#define STRUCT_PACKED
#define STRUCT_PACKING_PUSH
#define STRUCT_PACKING_POP
#define PACKED __attribute__((packed))
#define PACKED_STRUCTS_REQUIRE_SAME_TYPE false
#endif

#if __has_include(<string_view>) && !OM_CONFIG_DONT_USE_STRING_VIEW
#include <string_view>
using fast_string_ref = std::string_view;
#else
using fast_string_ref = const std::string&;
#endif



#include <common/Typedefs.h>
#include <common/Types.h>
#include <common/Enums.h>

#define WIDTH (320)
#define HEIGHT (200)

template<typename... T> using predicate = std::function<bool(T...)>;

//TODO: optimize for size(T) <= u32 to use a single u64
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
    K first;
    const V& second;
    const dummy_pair* operator->() const { return this; }
  };
  
protected:
  std::array<V, size> data;
public:
  enum_simple_map() { std::fill(std::begin(data), std::end(data), V()); }
  enum_simple_map(V value) { std::fill(std::begin(data), std::end(data), value); }

  enum_simple_map(V value, const std::initializer_list<init_element>& elements)
  {
    std::fill(data.begin(), data.end(), value);
    std::for_each(elements.begin(), elements.end(), [this](const init_element& entry) {
      assert(static_cast<size_t>(entry.first) < size);
      set(entry.first, entry.second);
    });
  }
  
  enum_simple_map(const std::initializer_list<init_element>& elements)
  {
    //assert(elements.size() == size);
    size_t i = 0;
    for (auto it = elements.begin(); it != elements.end(); ++i, ++it)
    {
      assert(static_cast<size_t>(it->first) < size);
      data[static_cast<size_t>(it->first)] = it->second;
    }
  }
  
  using iterator = typename std::array<V, size>::iterator;
  using const_iterator = typename std::array<V, size>::const_iterator;
  using mapped_type = V;
  
  iterator begin() { return data.begin(); }
  const_iterator begin() const { return data.begin(); }
  
  iterator end() { return data.end(); }
  const_iterator end() const { return data.end(); }
  
  void set(K key, const V& value) { data[static_cast<size_t>(key)] = value; }
  void set(K key, V&& value) { data[static_cast<size_t>(key)] = value; }

  V& operator[](K key) { return data[static_cast<size_t>(key)]; }
  const V& operator[](K key) const { return data[static_cast<size_t>(key)]; }
  dummy_pair find(K key) const { return { key, operator[](key) }; }
};

using color_d = u32;
struct Color
{
  union
  {
    struct { u8 b,g,r,a; };
    color_d data;
  };
  
  Color() = default;
  constexpr Color(u8 r, u8 g, u8 b, u8 a = 0xFF) : b(b), g(g), r(r), a(a) { }
  constexpr Color(color_d data) : data(data) { }
  
  Color blend(Color dst) const { return blend(dst, dst.a); }
  
  Color blend(Color dst, float dstAlpha) const {
    float da = dstAlpha, sa = 1.0f - dstAlpha;
    return Color(
	    static_cast<u8>(r*sa + dst.r*da),
      static_cast<u8>(g*sa + dst.g*da),
	    static_cast<u8>(b*sa + dst.b*da)
	  );
  }

  Color blend(Color dst, u8 dstAlpha) const {
    float sa = (255 - dstAlpha) / 255.0f;
    float da = dstAlpha / 255.0f;
    return Color(
	    static_cast<u8>(r*sa + dst.r*da),
	    static_cast<u8>(g*sa + dst.g*da),
	    static_cast<u8>(b*sa + dst.b*da)
	  );
  }
  operator color_d() const { return data; }
  
  static const Color BLACK, WHITE, NONE;
  
  struct hash
  {
    size_t operator()(const Color& color) const { return std::hash<color_d>()(color.data); }
  };
};

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

#define LBXI(x, y) (0x80000000 | (static_cast<u32>(LBXID::x) << 16) | (y))
#define LBXF(x, y, f) (0x80000000 | (static_cast<u32>(f) << 24) | (static_cast<u32>(LBXID::x) << 16) | (y))
#define LBXU(x) SpriteInfo(x)
#define LSI(x, y) SpriteInfo(LBXID::x, y)
#define LSI_PLACEHOLD SpriteInfo(LBXID::COUNT, 0)
#define TSI(x, y, z) SpriteInfo(TextureID::x, y, z)
#define TSIS(x, y) SpriteInfo(TextureID::x, y)


struct DiscreteTimer
{
private:
  count_t base;
  const count_t ticksPerTick;
  count_t tickCounter;
  count_t counter;
  
public:
  DiscreteTimer(count_t ticksPerTick) :
  ticksPerTick(ticksPerTick), counter(0), base(0) { }
  
  void mark(count_t base) { this->base = base; }
  
  void set(count_t value)
  {
    tick(value - base);
    base = value;
  }
  
  void tick(count_t delta)
  {
    tickCounter += delta;
    if (tickCounter >= ticksPerTick)
      counter += (tickCounter / ticksPerTick);
    tickCounter %= ticksPerTick;
  }
  
  void tick() { tick(1); }
  
  count_t value() const { return counter; }
  operator count_t() const { return counter; }
  
  void reset()
  {
    tickCounter = 0;
    counter = 0;
  }
  
  void resetCounter()
  {
    counter = 0;
  }
};

constexpr size_t LBX_COUNT = static_cast<size_t>(LBXID::COUNT);

using KeyboardKey = SDL_Keycode;
using KeyboardCode = SDL_Scancode;
using KeyboardMod = SDL_Keymod;
using sdl_text_input = const char (&)[32];

struct Position
{
  int_type x, y;
  Plane plane;
  
  Position() { }
  Position(int_type x, int_type y, Plane plane) : x(x), y(y), plane(plane) { }
  Position(Point pt, Plane plane) : x(pt.x), y(pt.y), plane(plane) { }
  
  Position relative(int_type ox, int_type oy) const { return Position(x+ox, y+oy, plane); }
  Position dx(int_type ox, int_type oy) const { return relative(ox, oy); }
  
  inline bool wrapAndCheckValidity(u16 w, u16 h)
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
  
  bool isValid(u16 w, u16 h) const
  {
    return y >= 0 || y < h;
  }
  
  bool same(const Position& p) const { return p.x == x && p.y == y && p.plane == plane; }
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
  
  N = NORTH,
  NE = NORTH_EAST,
  E = EAST,
  SE = SOUTH_EAST,
  S = SOUTH,
  SW = SOUTH_WEST,
  W = WEST,
  NW = NORTH_WEST,
  
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
  
  N  = 0x01, NE = 0x02, E  = 0x04, SE = 0x08,
  S  = 0x10, SW = 0x20, W  = 0x40, NW = 0x80,
  
  /* orthogonal for map */
  
  HORIZONTAL = W | E,
  VERTICAL = N | S,
  
  OCORNER_NE = N | E,
  OCORNER_NW = N | W,
  OCORNER_SE = S | E,
  OCORNER_SW = S | W,
  
  CORNER_NW = N | NW | W,
  CORNER_SW = S | SW | W,
  CORNER_NE = N | NE | E,
  CORNER_SE = S | SE | E,
  
  OCROSS = N | E | W | S,
  
  TCROSS_N = N | W | E,
  TCROSS_S = S | W | E,
  TCROSS_E = N | E | S,
  TCROSS_W = N | W | S,
  
  EDGE_N = NW | N | NE,
  EDGE_S = SW | S | SE,
  EDGE_W = NW | W | SW,
  EDGE_E = NE | E | SE,
  
  EDGES_NS = EDGE_N | EDGE_S,
  EDGES_WE = EDGE_W | EDGE_E,
  
  
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
  
  ALL = 0xFF,
  
  ALL_NO_N = ALL - N,
  ALL_NO_S = ALL - S,
  ALL_NO_W = ALL - W,
  ALL_NO_E = ALL - E,
};

struct DirMask
{
private:
  using ut = std::underlying_type<DirJoin>::type;
  static constexpr u32 COUNT = 8;
  static constexpr ut MASK = 0xFF;
  ut mask;
  ut value() const { return (ut)mask; }
  
public:
  constexpr DirMask() : DirMask(0) { }
  constexpr DirMask(ut mask) : mask(mask) { }
  constexpr DirMask(const DirMask& other) : mask(other.mask) { }
  constexpr DirMask(DirJoin join) : mask((ut)join) { }
  constexpr DirMask(DirJoin j1, DirJoin j2) : mask((ut)j1 | (ut)j2) { }
  
  inline bool is(DirJoin dir) const { return (mask & static_cast<ut>(dir)) != 0; }
  inline bool isJust(DirJoin dir) const { return mask == (static_cast<ut>(dir)); }
  
  inline bool operator&&(DirJoin dir) const { return is(dir); }
  inline bool operator==(const DirJoin dir) const { return isJust(dir); }
  inline bool operator!=(const DirJoin dir) const { return !isJust(dir); }
  
  inline bool operator==(const DirMask mask) const { return this->mask == mask.mask; }
  inline bool operator!=(const DirMask mask) const { return this->mask != mask.mask; }
  
  void unset(DirMask mask) { this->mask &= ~mask.value(); this->mask &= MASK; }
  void unset(DirJoin dir) { mask &= ~DirMask(dir).value(); }
  
  DirMask toOrthogonal() const { return DirMask((ut)mask & (ut)DirJoin::OCROSS); }
  DirMask toDiagonal() const { return DirMask((ut)mask & (ut)DirJoin::CROSS); }
  DirMask pruneDiagonal() const { return toOrthogonal(); }
  DirMask pruneDiagonalIfIsolated() const
  {
    DirMask mask = DirMask(*this);
    
    if (mask.is(DirJoin::N)) { mask.unset(DirMask(DirJoin::NE, DirJoin::NW)); }
    if (mask.is(DirJoin::E)) { mask.unset(DirMask(DirJoin::NE, DirJoin::SE)); }
    if (mask.is(DirJoin::S)) { mask.unset(DirMask(DirJoin::SE, DirJoin::SW)); }
    if (mask.is(DirJoin::W)) { mask.unset(DirMask(DirJoin::NW, DirJoin::SW)); }
    
    return mask;
  }
  
  struct iterator
  {
  private:
    const DirMask& mask;
    size_t position;
    
  public:
    iterator(const DirMask& mask, size_t pos = 0) : mask(mask), position(pos)
    {
      while (position < COUNT && !(mask.mask & (1 << position)))
        ++position;
    }
    
    inline bool operator!=(const iterator& other) const { return mask != other.mask || position != other.position; }
    inline const iterator& operator++()
    {
      ++position;
      while (position < COUNT && !(mask.mask & (1 << position)))
        ++position;
      
      return *this;
    }
    
    operator bool() const { return position < COUNT; }
    
    DirJoin operator*() const { return static_cast<DirJoin>(1 << position); }
  };
  
  iterator begin() const { return iterator(*this); }
  iterator end() const { return iterator(*this, COUNT); }
  
};

struct PositionOffset
{
  s8 x;
  s8 y;
  
  static PositionOffset forDirJoin(DirJoin d)
  {
    switch (d)
    {
      case DirJoin::N: return {0, -1};
      case DirJoin::NE: return {-1, -1};
      case DirJoin::E: return {1, 0};
      case DirJoin::SE: return {1, 1};
      case DirJoin::S: return {0, 1};
      case DirJoin::SW: return {-1, 1};
      case DirJoin::W: return {-1, 0};
      case DirJoin::NW: return {-1, -1};
      default: break;
    }
   
    assert(false);
    return {0,0};
  }
};


/* these utility functions are used to work with enum class through standard
 boolean operators
 */
inline DirJoin operator~(const DirJoin& lhs)
{
  using utype_t = std::underlying_type<DirJoin>::type;
  return static_cast<DirJoin>(~static_cast<utype_t>(lhs) & 0xFF);
}

inline DirJoin operator&(const DirJoin& lhs, const DirJoin& rhs) {
  using utype_t = std::underlying_type<DirJoin>::type;
  return static_cast<DirJoin>(static_cast<utype_t>(lhs) & static_cast<utype_t>(rhs));
}

inline DirJoin operator|(const DirJoin& lhs, const DirJoin& rhs) {
  using utype_t = std::underlying_type<DirJoin>::type;
  return static_cast<DirJoin>(static_cast<utype_t>(lhs) | static_cast<utype_t>(rhs));
}

inline DirJoin operator-(const DirJoin& lhs, const DirJoin& rhs) {
  using utype_t = std::underlying_type<DirJoin>::type;
  return static_cast<DirJoin>(static_cast<utype_t>(lhs) - static_cast<utype_t>(rhs));
}

inline void operator|=(DirJoin& lhs, const DirJoin& rhs) {
  using utype_t = std::underlying_type<DirJoin>::type;
  lhs = static_cast<DirJoin>(static_cast<utype_t>(lhs) | static_cast<utype_t>(rhs));
}

/* shift and rotate CW */

inline DirJoin operator<<(DirJoin lhs, u32 v) {
  using utype_t = std::underlying_type<DirJoin>::type;
  constexpr size_t bits = 8;
  v %= bits;
  
  utype_t keep = static_cast<utype_t>(lhs) << v;
  utype_t rotate = static_cast<utype_t>(lhs) >> (bits - v);
  
  return static_cast<DirJoin>((keep | rotate) & 0xFF);
}
/* shift and rotate CCW */

inline DirJoin operator>>(DirJoin lhs, u32 v) {
  using utype_t = std::underlying_type<DirJoin>::type;
  constexpr size_t bits = 8;
  v %= bits;
  
  utype_t keep = static_cast<utype_t>(lhs) >> v;
  utype_t rotate = static_cast<utype_t>(lhs) << (bits - v);
  
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
class IndexedPalette;
class ColorMap;
using ColorFilter = ColorMap;

class SpriteSheet
{
public:
  virtual u32 at(index_t x, index_t y, index_t c = 0, index_t r = 0) const { return 0x00000000; }
  virtual void set(index_t x, index_t y, Color c) { }
  virtual ~SpriteSheet() { }
  virtual index_t tw() const = 0;
  virtual index_t th() const = 0;
  
  virtual index_t sw(index_t r, index_t c) const = 0;
  virtual index_t sh(index_t r, index_t c) const = 0;
  
  virtual void lock() const { }
  virtual void unlock() const { }
  
  virtual const Palette* getPalette() const { return nullptr; }
};

/*
 This struct is used to refer to a specific graphic sprite by packing its attributes in
 a single value, the struct can hold a LBX reference or a real texture reference according to mode.
 
 LBX MODE
  0x0000FFFF : mask for sprite number
  0x00FF0000 : mask for LBX index (max 256 LBX files)
  0x7F000000 : which frame of LBX entry (0-127)
  0x80000000 : flag for LBX entry type, must be set
 
 TEXTURE MODE
  0x000000FF : mask for colum in spritesheet
  0x0000FF00 : mask for row in spritesheet
  0x00FF0000 : mask for texture ID (max 256 textures)
 
 By extending with 4 additional bytes we could specify palette and other attributes useful in a single number,
 this would require a large refactor but it maybe worth to simplify a lot of code around.
 
 The graphics engine doesn't care about which kind you use since the implementation automatically manages this.
 */
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

#if PACKED_STRUCTS_REQUIRE_SAME_TYPE
    struct { u16 _index : 16; u16 _lbx : 8; u16 _frame : 7; u16 __is_lbx : 1; } as_lbx;
    struct { u16 _y : 8; u16 _x : 8; u16 _tex : 15; u16 _is_lbx : 1; } as_texture;
#else
    struct { u16 _index : 16; LBXID _lbx : 8; u8 _frame : 7; bool __is_lbx : 1; } as_lbx;
    struct { u16 _y : 8; u16 _x : 8; TextureID _tex : 15; bool _is_lbx : 1; } as_texture;
#endif
  };
  
public:
  constexpr SpriteInfo() : SpriteInfo(0) { }
  constexpr SpriteInfo(data_type data) : data(data) { }
  
  SpriteInfo& operator=(const SpriteInfo& o) { this->data = o.data; return *this; }
  
  explicit constexpr SpriteInfo(TextureID texture, s8 x, s8 y) : data((static_cast<u32>(texture) << texture_id_shift) | (x << texture_x_shift) | (y << texture_y_shift)) { assert(x >= 0 && y >= 0); }
  explicit SpriteInfo(TextureID texture, u16 i);
  
  explicit constexpr SpriteInfo(LBXID lbx, u16 index) : data(lbx_flag_mask | (static_cast<u32>(lbx) << lbx_id_shift) | (index << lbx_index_shift)) { }
  explicit constexpr SpriteInfo(LBXID lbx, u16 index, u8 frame) : data(lbx_flag_mask | (frame << lbx_frame_shift) | (static_cast<u32>(lbx) << lbx_id_shift) | (index << lbx_index_shift)) { assert(frame <= 128); }

  constexpr bool isLBX() const { return data & lbx_flag_mask; }
  
  constexpr TextureID texture() const { return static_cast<TextureID>((data & texture_id_mask) >> texture_id_shift); }
  
  constexpr u16 x() const { return isLBX() ? 0 : (data & texture_x_mask) >> texture_x_shift; }
  constexpr u16 y() const { return isLBX() ? frame() : (data & texture_y_mask) >> texture_y_shift; }
  
  constexpr LBXID lbx() const { return static_cast<LBXID>((data & lbx_id_mask) >> lbx_id_shift); }
  constexpr u16 index() const { return (data & lbx_index_mask) >> lbx_index_shift; }
  constexpr u8 frame() const { return (data & lbx_frame_mask) >> lbx_frame_shift; }
  
  u16 sw() const { return sheet()->sw(x(), y()); }
  u16 sh() const { return sheet()->sh(x(), y()); }
  
  const Palette* palette() const;
  
  u16 count() const;
  
  inline constexpr SpriteInfo relative(s16 offset) const { assert(isLBX()); return SpriteInfo(lbx(), index() + offset); }
  inline constexpr SpriteInfo frame(s16 offset, u8 f) const { assert(isLBX()); return SpriteInfo(lbx(), index() + offset, f); }
  inline constexpr SpriteInfo frame(u8 f) const { assert(isLBX()); return SpriteInfo(lbx(), index(), f); }
  inline constexpr SpriteInfo lbx(LBXID l) const { assert(isLBX()); return SpriteInfo(l, index(), frame()); }

  inline constexpr SpriteInfo x(u16 x) const { assert(!isLBX()); return SpriteInfo(texture(), x, y()); }
  inline constexpr SpriteInfo y(u16 y) const { assert(!isLBX()); return SpriteInfo(texture(), x(), y); }
  inline constexpr SpriteInfo xy(u16 x, u16 y) const { assert(!isLBX()); return SpriteInfo(texture(), x, y); }


  constexpr bool isValid() const { return lbx() < LBXID::COUNT; }
  
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
  
  mutable value_t gold, mana, food;
  
  Upkeep(value_t gold = 0, value_t mana = 16, value_t food = 0) : gold(gold), mana(mana), food(food) { }

  value_t operator[](Type type) const
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

#include <vector>
#include <set>
#include <string>

class Spell;
enum class SpellRarity : u32;

template<typename T> using spell_rarity_map = enum_simple_map<SpellRarity, T, 4>;
template<typename T> using spell_enum_map = enum_simple_map<School, enum_simple_map<SpellRarity, T, 4>, 6>; //TODO: 6th element is necessary for arcane which is at index 0
template<typename T> using school_map = enum_simple_map<School, T, SCHOOL_COUNT>;
using school_value_map = school_map<value_t>;

template <typename T>
struct school_value
{
  School school;
  T value;
};

struct Retort
{
  struct Requirement
  {
    enum class Type : u8
    {
      ANY_SCHOOL_AT_LEAST,
      SPECIFIC_SCHOOL_AT_LEAST
    };
    
    Type type;
    School school;
    u8 count;
    u8 times;
    
    bool isSatisfied(const school_value_map& books) const
    {
      switch (type)
      {
        case Type::SPECIFIC_SCHOOL_AT_LEAST: return books[school] >= count;
        case Type::ANY_SCHOOL_AT_LEAST:
        {
          u32 valid = 0;
          for (const auto& c : books)
            valid += c >= count ? 1 : 0;
          
          return valid >= times;
        }
      }

      return false;
    }
  };
  
  const std::string identifier;
  const u16 cost;
  std::vector<Requirement> requirements;
  I18 i18n; //TODO: should be moved in GfxData?
  
  Retort(const std::string& identifier, u16 cost) : identifier(identifier), cost(cost) { }
  Retort(const std::string& identifier, u16 cost, const std::vector<Requirement>& requirements) : identifier(identifier), cost(cost), requirements(requirements) { }
  
  bool canBePicked(u16 availablePicks, const school_value_map& books) const
  {
    if (availablePicks < cost)
      return false;
    
    return std::all_of(requirements.begin(), requirements.end(), [&books](const Requirement& req) { return req.isSatisfied(books); });
  }

  bool operator==(const std::string& identifier) const { return this->identifier == identifier; }
};

struct Wizard
{
  school_value_map defaultBooks;
  std::set<const Retort*> defaultRetorts;
  
  Wizard() : defaultBooks(0) { }
};

struct PlayerSetupInfo
{
  const Wizard* portrait;
  const Race* race;
  std::string name;
  school_value_map books;
  spell_enum_map<std::set<const Spell*>> spells;
  std::set<const Retort*> retorts;
  PlayerColor color;
};

#endif
