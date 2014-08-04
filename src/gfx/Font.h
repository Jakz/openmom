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




enum FontFace : u16
{
  WHITE_TINY = 0,
  WHITE_TINY_STROKE,
  YELLOW_TINY_STROKE,
  RED_TINY_STROKE,
  
  TINY_COMPACT,
  
  TINY_COMPACT_CRYPT_BROWN,
  
  YELLOW_SMALL,
  WHITE_SMALL,
  TEAL_SMALL,
  BROWN_SMALL,
    
  GREEN_SMALLW,
  BLUE_SMALLW,
  RED_SMALLW,
  PURPLE_SMALLW,
  YELLOW_SMALLW,
  
  SURVEY_SERIF,
  BROWN_SERIF,
  DARK_BROWN_SERIF,
  
  SERIF_CRYPT_BROWN,
  
  TEAL_MEDIUM,
  TEAL_MEDIUM_STROKE,
  BRIGHT_TEAL_MEDIUM,
  BLACK_MEDIUM,
  
  BROWN_MEDIUM_BOLD_SHADOW,
  
  HUGE_SERIF
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
    
  }
  
  ~FontData()
  {
    for (int i = 0; i < GLYPH_COUNT; ++i)
      delete data;
  }
  
  void setData(u8* data)
  {
    this->data = data;
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

public:
  FontSpriteSheet(FontData *data, const Palette* palette, s8 hor, s8 ver) : rawData(data), palette(palette), hor(hor), ver(ver) { }
  FontSpriteSheet(FontData *data, color_list palette, s8 hor, s8 ver) : rawData(data), palette(new IndexedPalette(palette)), hor(hor), ver(ver) { }

  ~FontSpriteSheet() { delete [] palette; }
  
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
  
  u16 stringWidth(const std::string& str, s8 hor) const
  {
    u16 l = 0;
    for (s8 c : str)
    {
      if (c == '^') continue;
      else l += charWidth(c) + hor;
    }
    
    return l;
  }
  
  const Palette* getPalette() const override { return palette; }
};



class FontFaces
{
public:
  class Tiny { public:
    const static FontSpriteSheet *WHITE, *WHITE_STROKE, *YELLOW_STROKE, *RED_STROKE, *BROWN;
  };
  
  class Small { public:
    const static FontSpriteSheet *YELLOW, *BLUE_MAGIC, *WHITE_PALE, *YELLOW_PALE, *RED_PALE, *WHITE, *TEAL, *BROWN, *GREENW, *BLUEW, *REDW, *PURPLEW, *YELLOWW, *GRAY_ITEM_CRAFT, *BLINK_WHITE_GREY;
  };
  
  class Medium { public:
    const static FontSpriteSheet *TEAL, *TEAL_STROKE, *BLACK, *TEAL_BRIGHT, *BLUE_MAGIC;
  };
  
  class Serif { public:
    const static FontSpriteSheet *TEAL, *BROWN, *YELLOW_SHADOW, *GOLD_SHADOW, *SILVER_SHADOW, *WHITE_SURVEY, *DARK_BROWN;
  };
  
  class Crypt { public:
    const static FontSpriteSheet *SERIF_BROWN, *TINY_BROWN;
  };
  
  class Palettes { public:
    const static BlinkingPalette *BLINK_WHITE_BLUE;
  };
  
  static void buildFonts();
};

class Font
{
  const TextureID texture;
  //const Texture& texture;
  s8 widths[128];
  const ColorMap *map;
  
  public:

  const s16 w, h, hor, ver, space;
  const s8 charWidth(s8 c) { return widths[c]; }
  
  u16 stringWidth(const std::string& str, s8 hSpace)
  {
    bool first = true;
    u16 l = 0;
    for (s8 c : str)
    {
      if (first) first = false;
      else l += hor;
      
      if (c == ' ') l += hSpace;
      else if (c == '^') continue;
      else l += charWidth(c);
    }
    
    return l;
  }
  
  u16 stringWidth(const std::vector<const std::string>& tokens, s8 hSpace, u16 s, u16 e)
  {
    u16 l = 0;
    
    for (u16 i = s; i <= e; ++i)
      l += stringWidth(tokens[i], hSpace);

    l += (e-s)*hSpace;
    
    return l;
  }
  
  protected:
    Font(TextureID tex, s16 w, s16 h, s16 hor, s16 ver, s16 space, const ColorMap* map = nullptr) :
      texture(tex), w(w), h(h), hor(hor), ver(ver), space(space), map(map)
    {
      std::fill(std::begin(widths), std::end(widths), -1);
    }
  
  
    void setWidth(const std::string&& str, s8 width) { for (const s8 i : str) widths[i] = width; }
    void setWidth(s8 c, s8 width) { widths[c] = width; }
    void fillWidth(s8 upperWidth, s8 lowerWidth, u8 otherWidth)
    {
      for (u8 i = 0; i < 128; ++i)
      {
        if (widths[i] == -1)
        {
          if (isupper(i)) widths[i] = upperWidth;
          else if (islower(i)) widths[i] = lowerWidth;
          else widths[i] = otherWidth;
        }
      }
    }
  
  friend class Fonts;
};

class TinyFont : public Font
{
  public:
  TinyFont(TextureID tex, const ColorMap *map = nullptr) : Font(tex,7,8,1,-2,2, map)
  {
    setWidth("ilI1.:\'",1);
		setWidth(",()",2);
		setWidth("023456789BACDFGHMPRSTUWabcdefghknpqorstuwxy+-",3);
		setWidth("m/",5);
		fillWidth(4,4,4);
  }
};

class TinyCompactFont : public Font
{
  public:
  TinyCompactFont(TextureID tex, const ColorMap *map = nullptr) : Font(tex,7,8,1,-2,2, map)
  {
    setWidth("ilI1.:\'",1);
		setWidth(",()",2);
		setWidth("023456789BACDEFGHMPRSTUWabcdefghkmnpqorstuvwxy+-",3);
		setWidth("/",5);
		fillWidth(4,4,4);
  }
};

class TinyCompactCryptFont : public Font
{
  public:
  TinyCompactCryptFont(TextureID tex, const ColorMap *map = nullptr) : Font(tex,7,8,1,-2,2, map)
  {
    setWidth("iI123-.:\'",1);
		setWidth(",()l",2);
		setWidth("023456789BACDEFGHMPRSTUWabcdefghkmnpqorstuvwxyz+-",3);
		setWidth("/",5);
		fillWidth(4,4,4);
  }
  
  // l + 1
	// z = 3
	// 3 = 1
	// 2 = 1
	// - = invisibile, largo 1
};

class SmallFont : public Font
{
  public:
  SmallFont(TextureID tex, const ColorMap *map = nullptr) : Font(tex,7,7,1,3,3,map)
  {
    setWidth("il1.:\'",1);
		setWidth(",()",2);
		setWidth("jtfxI+-",3);
		setWidth("VTMmWYw/",5);		
		fillWidth(4,4,4);
  }
};

class MediumFont : public Font
{
  public:
  MediumFont(TextureID tex, const ColorMap *map = nullptr) : Font(tex,9,11,1,3,1,map)
  {
    setWidth("il.:\'",1);
		setWidth(",1",2);
		setWidth("()txI+-",3);
		setWidth("4JTXMvVmWwY/",5);
		fillWidth(4,4,4);
  }
};

class MediumBoldFont : public Font
{
  public:
  MediumBoldFont(TextureID tex, const ColorMap *map = nullptr) : Font(tex, 10, 10, 1, 3, 4, map)
  {
    setWidth("Iil'.1",2);
		setWidth("j",3);
		setWidth("Jft",4);
		setWidth("abcdeghknopqrsuvxyz023456789",5);
    setWidth("ABCDEFGHKLNOPRSTUVXYZ",6);
    setWidth("Q",7);
    setWidth("MWmw/",8);
		fillWidth(4,4,4);
  }
};

class SerifFont : public Font
{
  public:
  SerifFont(TextureID tex, const ColorMap *map = nullptr) : Font(tex, 11, 12, 1, -1, 3, map)
  {
    setWidth("'",2);
		setWidth("il",3);
		setWidth("1",4);
		setWidth(".%+0bcegoksty",5);
    setWidth("?245IShdfapurv",6);
    setWidth("CJLOTnx",7);
    setWidth("BDEFGPQmw",8);
		fillWidth(11,9,11);
  }
};

class SerifCryptFont : public Font
{
  public:
  SerifCryptFont(TextureID tex, const ColorMap *map = nullptr) : Font(tex, 10, 10, 1, -1, 3, map)
  {
    setWidth("bcegijl",3);
		setWidth("bcegkosty",5); // TODO: bug b repeated
		setWidth("ISadfhpruv",6);
		setWidth("CLTVnx",7);
    setWidth("BDEFGPmw",8);
    setWidth("AMNRUW",9);
    setWidth("H",10);
		fillWidth(9,11,20);
  }
  
  // TODO: check character M and N
};

class HugeSerifFont : public Font
{
  public:
  HugeSerifFont(TextureID tex, const ColorMap *map = nullptr) : Font(tex, 17, 17, 1, -1, 11, map)
  {
    setWidth("cegilort",6);
		setWidth("afs",7);
		setWidth("dnpuyz",8);
		setWidth("G",11);
    setWidth("mD",12);
    setWidth("LOS",13);
    setWidth("MW",14);
		fillWidth(9,11,20);
  }
};

class Fonts
{
  private:
    static Font* font;
    static s16 vSpace, hSpace;
    static const ColorMap *map, *omap;
    static Font fonts[];
    static std::unordered_map<char, const ColorMap*> fontColors;
  
  public:
    static std::string format(const char* str, ...);

    static inline u16 stringWidth(FontFace face, const std::string string) { return fonts[face].stringWidth(string, hSpace); }
    static inline u16 stringHeight() { return 0; }
      
    static inline const std::string join(std::vector<const std::string>& tokens, s16 s, s16 e);
    static inline void split(std::string string, std::vector<const std::string>& tokens, s8 delim);
  
    static inline void setFace(FontFace face, s16 v, s16 h) { setFace(face); setVerHorSpace(v,h); }
    static inline void setFace(FontFace face, const ColorMap* map, s16 v, s16 h) { setFace(face,v,h); setMap(map); }
  
    static inline void setFace(FontFace face)
    {
      font = &fonts[face];
      hSpace = font->space;
      vSpace = font->ver;
      map = font->map;
      omap = map;
    }
  
    static inline void setFace(FontFace face, ColorMap* map, s16 v, s16 h) { setFace(face); vSpace = v; hSpace = h; }
    static inline void setHorSpace(s16 h) { hSpace = h; }
    static inline void setVerSpace(s16 v) { vSpace = v; }
    static inline void setVerHorSpace(s16 v, s16 h) { vSpace = v; hSpace = h; }
    static inline void setMap(const ColorMap *m) { map = m; omap = m; }
  
    static u16 drawString(const std::string string, u16 x, u16 y, TextAlign align, const ColorMap *map)
    {
      setMap(map);
      u16 r = drawString(string,x,y,align);
      return r;
    }
  
    static u16 drawString(const std::string string, FontFace face, u16 x, u16 y, TextAlign align, const ColorMap *map)
    {
      setFace(face);
      return drawString(string, x, y, align, map);
    }
  
    static u16 drawString(const std::string string, FontFace face, u16 x, u16 y, TextAlign align)
    {
      setFace(face);
      return drawString(string, x, y, align);
    }
  
    static u16 drawString(const std::string string, u16 x, u16 y, TextAlign align);
    static u16 drawStringContext(const std::string string, u16 x, u16 y, TextAlign align);
  
    static u16 drawStringBounded(const std::string string, FontFace face, u16 x, u16 y, s16 bound, TextAlign align, const ColorMap* map = nullptr)
    {
      setFace(face);
      setMap(map);
      return drawStringBounded(string, x, y, bound, align);
    }
  
    static u16 drawStringBounded(const std::string string, int x, int y, int bound, TextAlign align, const ColorMap* map = nullptr);
  
    static FontFace fontForColor(PlayerColor color)
    {
      switch (color)
      {
        case GREEN: return GREEN_SMALLW;
        case BLUE: return BLUE_SMALLW;
        case RED: return RED_SMALLW;
        case PURPLE: return PURPLE_SMALLW;
        case YELLOW: return YELLOW_SMALLW;
        default: return WHITE_SMALL;
      }
    }
  
    static const Font& fontFor(FontFace face) { return fonts[face]; }

};

namespace fnts
{
  class Fonts
  {
  private:
    static const FontSpriteSheet* font;
    static s16 vSpace, hSpace;
    static const Palette *palette, *opalette;
    static std::unordered_map<char, const Palette*> fontColors;
    
  public:
    static std::string format(const char* str, ...);
    
    static inline u16 stringWidth(const FontSpriteSheet* face, const std::string string) { return face->stringWidth(string, hSpace); }
    static inline u16 stringHeight() { return 0; }
    
    static inline const std::string join(std::vector<const std::string>& tokens, s16 s, s16 e);
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
    }
    
    static inline void setFace(const FontSpriteSheet* face, Palette* palette, s16 v, s16 h) { setFace(face); vSpace = v; hSpace = h; }
    static inline void setHorSpace(s16 h) { hSpace = h; }
    static inline void setVerSpace(s16 v) { vSpace = v; }
    static inline void setVerHorSpace(s16 v, s16 h) { vSpace = v; hSpace = h; }
    static inline void setMap(const Palette *p) { palette = p; opalette = p; }
    
    static u16 drawString(const std::string string, u16 x, u16 y, TextAlign align, const Palette *palette)
    {
      setMap(palette);
      u16 r = drawString(string,x,y,align);
      return r;
    }
    
    static u16 drawString(const std::string string, const FontSpriteSheet* face, u16 x, u16 y, TextAlign align, const Palette *palette)
    {
      setFace(face);
      return drawString(string, x, y, align, palette);
    }
    
    static u16 drawString(const std::string string, const FontSpriteSheet* face, u16 x, u16 y, TextAlign align)
    {
      setFace(face);
      return drawString(string, x, y, align);
    }
    
    static u16 drawString(const std::string string, u16 x, u16 y, TextAlign align);
    static u16 drawStringContext(const std::string string, u16 x, u16 y, TextAlign align);
    
    static u16 drawStringBounded(const std::string string, const FontSpriteSheet* face, u16 x, u16 y, s16 bound, TextAlign align, const Palette* palette = nullptr)
    {
      setFace(face);
      
      if (palette)
        setMap(palette);
      
      return drawStringBounded(string, x, y, bound, align);
    }
    
    static u16 drawStringBounded(const std::string string, int x, int y, int bound, TextAlign align, const Palette* palette = nullptr);
    
    static const Palette* paletteFor(char c) { return fontColors[c]; }
    
    static FontFace fontForColor(PlayerColor color)
    {
      switch (color)
      {
        case GREEN: return GREEN_SMALLW;
        case BLUE: return BLUE_SMALLW;
        case RED: return RED_SMALLW;
        case PURPLE: return PURPLE_SMALLW;
        case YELLOW: return YELLOW_SMALLW;
        default: return WHITE_SMALL;
      }
    }
  };
}

#endif
