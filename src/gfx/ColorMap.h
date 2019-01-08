//
//  ColorMap.h
//  OpenMoM
//
//  Created by Jack on 7/18/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _COLOR_MAP_H_
#define _COLOR_MAP_H_

#include "common/Common.h"

#include <initializer_list>
#include <unordered_set>
#include <unordered_map>

#include <set>

class ColorMap
{
  public:
    virtual u32 get(u32 k) const = 0;
};

using ColorFilter = ColorMap;

using color_list = std::initializer_list<Color>;
using color_array = std::vector<Color>;
using color_map = std::unordered_map<Color, Color, Color::hash>;

class BlinkMap : public ColorMap
{
  private:
    const s16 s[3];
    const s16 d[3];
    const u16 ticks, updatedForTick = -1;
  
    std::unordered_set<Color, Color::hash> set;
    //Color color;
  
  
  public:
    BlinkMap(color_list colors, u8 r1, u8 g1, u8 b1, u8 r2, u8 g2, u8 b2, u16 ticks) :
      set(colors), s{r1,g1,b1}, d{static_cast<s16>(r2-r1),static_cast<s16>(g2-g1),static_cast<s16>(b2-b1)}, ticks(ticks) { }
  
    u32 get(u32 k) const override;
};

namespace rgb_filters
{
  class ConstantColor : public ColorFilter
  {
  private:
    const Color color;
  public:
    ConstantColor(Color color) : color(color) { }
    u32 get(u32 k) const override { return color; }
  };
}

class HashColorMap : public ColorMap
{
  private:
    color_map map;
  
  public:
    HashColorMap() { }
    HashColorMap(color_list data)
    {
      color_list::iterator it = data.begin();
      
      while (it != data.end())
      {
        Color src = *it++;
        Color dst = *it++;
        map[src] = dst;
      }
    }
  
    HashColorMap(color_list from, color_list to)
    {
      color_list::iterator it = from.begin(), it2 = to.begin();
      while (it != from.end())
        map[*it++] = *it2++;
    }

    u32 get(u32 k) const override
    {
      if (k != 0)
      {
        //printf("%d %d %d %d\n", k >> 24, (k>>16)&0xff, (k>>8)&0xff, k&0xff);
      
        color_map::const_iterator it = map.find(k);
      
        return it != map.end() ? it->second.data : k;
      }
      
      return k;
    }
};

class MiscMaps
{
public:
  static const enum_simple_map<School, std::array<Color, 5>, SCHOOL_COUNT> SCHOOL_GLOW_COLORS;
  static const enum_simple_map<PlayerColor, HashColorMap, 6> FLAG_COLORS_MAP;
  static const enum_simple_map<PlayerColor, HashColorMap, 6> UNIT_COLORS_MAP;
  
  class GrayscaleMap : public ColorMap
  {
    public:
      GrayscaleMap() { }
      
      u32 get(u32 k) const override
      {
        if (k & 0xFF000000)
        {
          u8 r = (k >> 16) & 0xFF;
          u8 g = (k >> 8) & 0xFF;
          u8 b = k & 0xFF;
          
          u8 v = static_cast<u8>(0.21f*r + 0.72f*g + 0.07*b);
          return 0xFF000000 | (v << 16) | (v << 8) | v;
        }
        else
          return k;
        
      };
  };
  
  
  static const GrayscaleMap GRAYSCALE;
};

class Palette
{
public:
  virtual Color get(u8 index) const = 0;
  Color operator[](u8 index) const { return get(index); }
  virtual ~Palette() { }
};

class IndexedPalette : public Palette
{
private:
  Color *colors;
public:
  IndexedPalette(Color* colors) : colors(colors) { }
  IndexedPalette(size_t size) : colors(new Color[size]) { }
  
  template<size_t S> IndexedPalette(const std::array<Color,S>& colors) : colors(new Color[colors.size()])
  {
    std::copy(colors.begin(), colors.end(), this->colors);
  }
  
  IndexedPalette(const color_array& colors) : colors(new Color[colors.size()]) { for (size_t i = 0; i < colors.size(); ++i) this->colors[i] = colors[i]; }
  IndexedPalette(color_list colors) : colors(new Color[colors.size()]) { setPalette(colors); }
  
  void setPalette(color_list colors) const
  {
    for (size_t i = 0; i < colors.size(); ++i)
      this->colors[i] = *std::next(colors.begin(), i);
  }
  
  const Color* raw() const { return colors; }
  
  void set(u8 index, Color color) { colors[index] = color; }
  Color get(u8 index) const override { return colors[index]; }
    
  ~IndexedPalette() { delete [] colors; }
};

class SharedPalette : public Palette
{
private:
  const Color* colors;
public:
  SharedPalette(const IndexedPalette* palette) : colors(palette->raw()) { }
  SharedPalette(const Color* colors) : colors(colors) { }
  
  Color get(u8 index) const override { return colors[index]; }
};

class BlinkingPalette : public Palette
{
private:
  const s16 s[3];
  const s16 d[3];
  const u16 ticks, updatedForTick = -1;
  
  std::set<u8> indices;
  
public:
  BlinkingPalette(std::initializer_list<u8> indices, u8 r1, u8 g1, u8 b1, u8 r2, u8 g2, u8 b2, u16 ticks) : indices(indices), s{r1,g1,b1}, d{static_cast<s16>(r2-r1),static_cast<s16>(g2-g1),static_cast<s16>(b2-b1)}, ticks(ticks)
  {

  }

  Color get(u8 index) const override;
};

class DerivedPalette : public Palette
{
private:
  const Palette* palette;
  Color* colors;
  size_t start;
  size_t end;
public:
  DerivedPalette(const Palette* palette, size_t start, size_t length) : palette(palette), colors(new Color[length]), start(start), end(start+length)
  { }
  
  DerivedPalette(const Palette* palette, size_t start, size_t length, Color color) : DerivedPalette(palette, start, length)
  {
    std::fill(colors, colors+length, color);
  }
  
  DerivedPalette(const Palette* palette, size_t start, size_t length, color_list colors) : DerivedPalette(palette, start, length)
  {
    assert(colors.size() == length);
    for (size_t i = 0; i < colors.size(); ++i)
      this->colors[i] = *std::next(colors.begin(), i);
  }
  
  void set(u8 index, Color color) { assert(index >= start && index < end); colors[index - start] = color; }
  void setPalette(const Palette* palette) { this->palette = palette; }
  
  Color get(u8 index) const override { return index >= start && index < end ? colors[index - start] : palette->get(index); }
};
    
class GrayscalePalette : public Palette
{
private:
  const Palette* palette;
  
public:
  GrayscalePalette(const Palette* palette) : palette(palette) { }
  Color get(u8 index) const override
  {
    Color c = palette->get(index);
    u8 v = static_cast<u8>(0.21f*c.r + 0.72f*c.g + 0.07*c.b);
    return Color(v, v, v);
  }
};

#endif
