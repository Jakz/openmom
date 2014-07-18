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
  
  SMALL_SHORTER,
  
  GREEN_SMALLW,
  BLUE_SMALLW,
  RED_SMALLW,
  PURPLE_SMALLW,
  YELLOW_SMALLW,
  
  TEAL_SERIF,
  YELLOW_SERIF_SHADOW,
  GOLD_SERIF_SHADOW,
  SILVER_SERIF_SHADOW,
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

class SmallShorterFont : public Font
{
  public:
  SmallShorterFont(TextureID tex, const ColorMap *map = nullptr) : Font(tex, 7, 7, 1, 2, 3, map)
  {
    setWidth("il1.:\'", 1);
    setWidth(",()", 2);
    setWidth("jtfxI+-", 3);
    setWidth("VTMmWYw/", 5);
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
    static std::unordered_map<s8, ColorMap*> fontColors;
  
  public:
    static std::string format(const std::string, ...);
  
    static inline u16 stringWidth(FontFace face, const std::string string) { return fonts[face].stringWidth(string, hSpace); }
    static inline u16 stringHeight() { return 0; }
      
    static inline const std::string join(std::vector<const std::string>& tokens, s16 s, s16 e);
    static inline void split(std::string string, std::vector<const std::string>& tokens, s8 delim);
  
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
    static inline void setMap(ColorMap *m) { map = m; omap = m; }
  
    static u16 drawString(const std::string string, u16 x, u16 y, TextAlign align, ColorMap *map)
    {
      setMap(map);
      u16 r = drawString(string,x,y,align);
      return r;
    }
  
    static u16 drawString(const std::string string, FontFace face, u16 x, u16 y, TextAlign align, ColorMap *map)
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
  
    static u16 drawStringBounded(const std::string string, FontFace face, u16 x, u16 y, s16 bound, TextAlign align, ColorMap* map = nullptr)
    {
      setFace(face);
      setMap(map);
      return drawStringBounded(string, x, y, bound, align);
    }
  
    static u16 drawStringBounded(const std::string string, int x, int y, int bound, TextAlign align, ColorMap* map = nullptr);
  
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

#endif
