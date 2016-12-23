//
//  Font.h
//  OpenMoM
//
//  Created by Jack on 18/07/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _FONT_H_
#define _FONT_H_

#include "Texture.h"

#include <string>
#include <vector>
#include <unordered_map>

enum FontType
{
  FONT_TINY = 0,
  FONT_SMALL,
  FONT_MEDIUM,
  FONT_MEDIUM_THICK,
  FONT_SERIF,
  FONT_HUGE,
  FONT_TINY_CRYPT,
  FONT_SERIF_CRYPT,
  
  FONT_TYPE_COUNT
};


enum TextAlign : u8
{
  ALIGN_RIGHT = 0,
  ALIGN_CENTER,
  ALIGN_LEFT
};


const u16 GLYPH_COUNT = 96;

class SpriteRawData
{
  
public:
  virtual u8* dataAt(u16 x, u16 y) const = 0;
  virtual u16 rows() const = 0;
  virtual u16 columns() const = 0;
  
  virtual u16 tw() const = 0;
  
  virtual u16 w(u16 x = 0) const = 0;
  virtual u16 h(u16 x = 0) const = 0;
};

/* glyph data is stored in a monodimensional array in which all glyphs are stored contiguously, eg
   XXXYYYZZZ
   XXXYYYZZZ becomes XXXYYYZZZXXXYYYZZZXXXYYYZZZ
   XXXYYYZZZ
 */
class FontData : public SpriteRawData
{
private:
  u8* data;
  s8 glyphWidth[GLYPH_COUNT];
  const u8 width;
  const u8 height;
  
public:
  FontData(FontType type, u8 height, u8 width) : type(type), height(height), width(width), data(new u8[width*height*GLYPH_COUNT])
  {
    std::fill(data, data + width*height*GLYPH_COUNT, 0);
  }
  
  ~FontData()
  {
    delete [] data;
  }

  void setGlyphWidth(u8 index, s8 width) {  glyphWidth[index] = width; }
  s8 getGlyphWidth(u8 index) const { return glyphWidth[index - ' ']; }
  
  u8* dataAt(u16 x = 0, u16 y = 0) const override { return &data[y*width]; }
  u16 rows() const override { return 1; }
  u16 columns() const override { return GLYPH_COUNT; }
  u16 w(u16 x = 0) const override { return width; }
  u16 h(u16 x = 0) const override { return height; }
  u16 tw() const override { return width*GLYPH_COUNT; }
  
  const FontType type;
  
  static constexpr const u8 LIGHT_STROKE_VALUE = 0x01;
  static constexpr const u8 DARK_STROKE_VALUE = 0x02;
  
  static FontData* fonts[FONT_TYPE_COUNT];
};

class FontSpriteSheet : public SpriteSheet
{
private:
  FontData *rawData;
  const Palette* palette;
  
  FontSpriteSheet(const FontSpriteSheet&) = delete;

public:
  FontSpriteSheet(FontData *data, const Palette* palette, s8 hor, s8 ver) : rawData(data), palette(palette), hor(hor), ver(ver) { }
  FontSpriteSheet(FontData *data, color_list palette, s8 hor, s8 ver) : rawData(data), palette(new IndexedPalette(palette)), hor(hor), ver(ver) { }

  ~FontSpriteSheet() { delete palette; }
  
  Color at(u16 x, u16 y, u16 r = 0, u16 c = 0) const override
  {
    const u8* data = rawData->dataAt(r,c);
    u8 value = data[x + y*rawData->tw()];
    
    //return RGB(255,0,0);
    
    return value;
  }
  
  u16 tw() const override { return rawData->tw(); }
  u16 th() const override { return rawData->h(); }
  
  u16 sw(u16 r, u16 c = 0) const override { return rawData->w(); }
  u16 sh(u16 r = 0, u16 c = 0) const override { return rawData->h(); }
  
  const s8 hor, ver;
  
  const s8 charWidth(s8 c) const { return rawData->getGlyphWidth(c); }
  
  u16 stringWidth(const std::string& str, s8 hor, s8 spaceAdj) const
  {
    u16 l = 0;
    for (s8 c : str)
    {
      if (c == '^') continue;
      else if (c == ' ') l += charWidth(c) + hor - spaceAdj;
      else l += charWidth(c) + hor;
    }
    
    return l;
  }
  
  const Palette* getPalette() const override { return palette; }
};



class FontFaces
{
public:
  class Tiny {
  public:
    const static FontSpriteSheet *WHITE, *WHITE_STROKE, *YELLOW_STROKE, *RED_STROKE, *BROWN;
  };
  
  class Small {
  public:
    const static FontSpriteSheet *YELLOW, *BLUE_MAGIC, *WHITE_PALE, *YELLOW_PALE, *RED_PALE, *WHITE, *TEAL, *BROWN, *GREENW, *BLUEW, *REDW, *PURPLEW, *YELLOWW, *GRAY_ITEM_CRAFT, *BLINK_WHITE_GREY;
  };
  
  class Medium {
  public:
    const static FontSpriteSheet *TEAL, *TEAL_STROKE, *BLACK, *TEAL_BRIGHT, *BLUE_MAGIC;
  };
  
  class MediumBold {
  public:
    const static FontSpriteSheet *BROWN_START;
  };
  
  class Serif {
  public:
    const static FontSpriteSheet *TEAL, *BROWN, *YELLOW_SHADOW, *GOLD_SHADOW, *GOLD, *GOLD_ERROR_MESSAGE, *SILVER_SHADOW, *WHITE_SURVEY, *DARK_BROWN;
  };
  
  class Crypt {
  public:
    const static FontSpriteSheet *SERIF_BROWN, *TINY_BROWN;
  };
  
  class Huge {
  public:
    const static FontSpriteSheet *GOLD;
  };
  
  
  class Palettes {
  public:
    const static BlinkingPalette *BLINK_WHITE_BLUE;
    const static Palette *SMALL_WHITE_PALE, *SMALL_YELLOW_PALE;
    const static Palette *WHITE_PRODUCTION;
  };
  
  static void buildFonts();
};

class Fonts
{
private:
  static const FontSpriteSheet* font;
  static s16 vSpace, hSpace;
  static s16 spaceCharAdj;
  static const Palette *palette, *opalette;
  static std::unordered_map<char, const Palette*> fontColors;
  
public:
  static std::string format(const char* str, ...);
  
  static inline u16 stringWidth(const FontSpriteSheet* face, const std::string string) { return face->stringWidth(string, hSpace, spaceCharAdj); }
  static inline u16 stringHeight() { return 0; }
  
  static inline const std::string join(const std::vector<const std::string>& tokens, s16 s, s16 e);
  static inline void split(std::string string, std::vector<const std::string>& tokens, s8 delim);
  
  static inline void setFace(const FontSpriteSheet* face, s16 v, s16 h) { setFace(face); setVerHorSpace(v,h); }
  static inline void setFace(const FontSpriteSheet* face, const Palette* palette, s16 v, s16 h) { setFace(face,v,h); setMap(palette); }
  
  static inline void setFace(const FontSpriteSheet* face)
  {
    font = face;
    hSpace = font->hor;
    vSpace = font->ver;
    palette = font->getPalette();
    opalette = palette;
    spaceCharAdj = 0;
  }
  
  static inline void setFace(const FontSpriteSheet* face, Palette* palette, s16 v, s16 h) { setFace(face); vSpace = v; hSpace = h; }
  static inline void setHorSpace(s16 h) { hSpace = h; }
  static inline void setVerSpace(s16 v) { vSpace = v; }
  static inline void setSpaceAdj(s16 s) { spaceCharAdj = s; }
  static inline void setVerHorSpace(s16 v, s16 h) { vSpace = v; hSpace = h; }
  static inline void setMap(const Palette *p) { palette = p; opalette = p; }
  
  static u16 drawString(const std::string& string, u16 x, u16 y, TextAlign align, const Palette *palette)
  {
    setMap(palette);
    u16 r = drawString(string,x,y,align);
    return r;
  }
  
  static u16 drawString(const std::string& string, const FontSpriteSheet* face, u16 x, u16 y, TextAlign align, const Palette *palette)
  {
    setFace(face);
    return drawString(string, x, y, align, palette);
  }
  
  static u16 drawString(const std::string& string, const FontSpriteSheet* face, u16 x, u16 y, TextAlign align)
  {
    setFace(face);
    return drawString(string, x, y, align);
  }
  
  static u16 drawString(const std::string& string, u16 x, u16 y, TextAlign align);
  static u16 drawStringContext(const std::string& string, u16 x, u16 y, TextAlign align);
  
  static u16 drawStringBounded(const std::string& string, const FontSpriteSheet* face, u16 x, u16 y, s16 bound, TextAlign align, const Palette* palette = nullptr)
  {
    setFace(face);
    
    if (palette)
      setMap(palette);
    
    return drawStringBounded(string, x, y, bound, align);
  }
  
  static u16 drawStringBounded(const std::string& string, const int x, int y, int bound, TextAlign align, const Palette* palette = nullptr);
  
  static const Palette* paletteFor(char c) { return fontColors[c]; }
  
  static std::string groupDigits(u32 value);
  
  static const FontSpriteSheet* fontForColor(PlayerColor color)
  {
    switch (color)
    {
      case GREEN: return FontFaces::Small::GREENW;
      case BLUE: return FontFaces::Small::BLUEW;
      case RED: return FontFaces::Small::REDW;
      case PURPLE: return FontFaces::Small::PURPLEW;
      case YELLOW: return FontFaces::Small::YELLOWW;
      default: return FontFaces::Small::WHITE;
    }
  }
};

#endif
