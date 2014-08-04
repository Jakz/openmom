//
//  Font.cpp
//  OpenMoM
//
//  Created by Jack on 18/07/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "Font.h"

#include "Gfx.h"

using namespace std;


FontData* FontData::fonts[FONT_TYPE_COUNT] = {nullptr};

const FontSpriteSheet* FontFaces::Tiny::WHITE = nullptr;
const FontSpriteSheet* FontFaces::Tiny::WHITE_STROKE = nullptr;
const FontSpriteSheet* FontFaces::Tiny::YELLOW_STROKE = nullptr;
const FontSpriteSheet* FontFaces::Tiny::RED_STROKE = nullptr;
const FontSpriteSheet* FontFaces::Tiny::BROWN = nullptr;

const FontSpriteSheet* FontFaces::Small::YELLOW = nullptr;
const FontSpriteSheet* FontFaces::Small::BLUE_MAGIC = nullptr;
const FontSpriteSheet* FontFaces::Small::WHITE_PALE = nullptr;
const FontSpriteSheet* FontFaces::Small::YELLOW_PALE = nullptr;
const FontSpriteSheet* FontFaces::Small::RED_PALE = nullptr;
const FontSpriteSheet* FontFaces::Small::WHITE = nullptr;
const FontSpriteSheet* FontFaces::Small::TEAL = nullptr;
const FontSpriteSheet* FontFaces::Small::BROWN = nullptr;
const FontSpriteSheet* FontFaces::Small::GREENW = nullptr;
const FontSpriteSheet* FontFaces::Small::BLUEW = nullptr;
const FontSpriteSheet* FontFaces::Small::REDW = nullptr;
const FontSpriteSheet* FontFaces::Small::PURPLEW = nullptr;
const FontSpriteSheet* FontFaces::Small::YELLOWW = nullptr;
const FontSpriteSheet* FontFaces::Small::GRAY_ITEM_CRAFT = nullptr;
const FontSpriteSheet* FontFaces::Small::BLINK_WHITE_GREY = nullptr;


const FontSpriteSheet* FontFaces::Medium::TEAL = nullptr;
const FontSpriteSheet* FontFaces::Medium::TEAL_STROKE = nullptr;
const FontSpriteSheet* FontFaces::Medium::TEAL_BRIGHT = nullptr;
const FontSpriteSheet* FontFaces::Medium::BLACK = nullptr;
const FontSpriteSheet* FontFaces::Medium::BLUE_MAGIC = nullptr;


const FontSpriteSheet* FontFaces::Serif::TEAL = nullptr;
const FontSpriteSheet* FontFaces::Serif::BROWN = nullptr;
const FontSpriteSheet* FontFaces::Serif::YELLOW_SHADOW = nullptr;
const FontSpriteSheet* FontFaces::Serif::GOLD_SHADOW = nullptr;
const FontSpriteSheet* FontFaces::Serif::SILVER_SHADOW = nullptr;
const FontSpriteSheet* FontFaces::Serif::WHITE_SURVEY = nullptr;
const FontSpriteSheet* FontFaces::Serif::DARK_BROWN = nullptr;

const FontSpriteSheet* FontFaces::Crypt::SERIF_BROWN = nullptr;
const FontSpriteSheet* FontFaces::Crypt::TINY_BROWN = nullptr;

const BlinkingPalette* FontFaces::Palettes::BLINK_WHITE_BLUE = nullptr;

const FontSpriteSheet* buildTiny(color_list colors) { return new FontSpriteSheet(FontData::fonts[FONT_TINY], colors, 1, -2); }
const FontSpriteSheet* buildTinyCrypt(color_list colors) { return new FontSpriteSheet(FontData::fonts[FONT_TINY_CRYPT], colors, 1, -1); }
const FontSpriteSheet* buildSmall(color_list colors) { return new FontSpriteSheet(FontData::fonts[FONT_SMALL], colors, 1, 2); }
const FontSpriteSheet* buildMedium(color_list colors) { return new FontSpriteSheet(FontData::fonts[FONT_MEDIUM], colors, 1, 3); }
const FontSpriteSheet* buildSerif(color_list colors) { return new FontSpriteSheet(FontData::fonts[FONT_SERIF], colors, 1, -1); }
const FontSpriteSheet* buildSerifCrypt(color_list colors) { return new FontSpriteSheet(FontData::fonts[FONT_SERIF_CRYPT], colors, 1, -3); }

const FontSpriteSheet* buildSmall(BlinkingPalette *palette) { return new FontSpriteSheet(FontData::fonts[FONT_SMALL], palette, 1, 2); }


// tiny change SX = SX - 1 compared to old medium
// medium should change SX = SX - 1 compared to old medium
// serif crypt change SX = SX - 1 compared to old medium
// small SX = SX - 1, SY = SY - 1

void FontFaces::buildFonts()
{
  FontData::fonts[FONT_MEDIUM]->setGlyphWidth(' '-' ', 1);
  FontData::fonts[FONT_SERIF_CRYPT]->setGlyphWidth(' '-' ', 3);

  Tiny::WHITE = buildTiny({0, 0, RGB(0,0,0), RGB(143, 133, 130), RGB(255, 255, 255)});
  Tiny::WHITE_STROKE = buildTiny({0, RGB(0,0,0), RGB(0,0,0), RGB(143, 133, 130), RGB(255, 255, 255)});
  Tiny::YELLOW_STROKE = buildTiny({0, RGB(0,0,0), RGB(0,0,0), RGB(124,82,36), RGB(213,133,27)});
  Tiny::RED_STROKE = buildTiny({0, RGB(0,0,0), RGB(0,0,0), RGB(128,0,0), RGB(255,0,0)});
  Tiny::BROWN = buildTiny({0, 0, 0, RGB(121,85,36), RGB(97,69,36)});
  
  Small::YELLOW = buildSmall({0,0,RGB(81,60,48),RGB(150,109,52),RGB(223,150,28)});
  Small::BLUE_MAGIC = buildSmall({0,0,RGB(40,40,65),RGB(97,97,125),RGB(146,146,166)});
  Small::WHITE_PALE = buildSmall({0,0,RGB(93,93,121),RGB(142,134,130),RGB(255,255,255)});
  Small::YELLOW_PALE = buildSmall({0,0,RGB(93,93,121),RGB(142,134,130),RGB(249,232,67)});
  Small::RED_PALE = buildSmall({0,0,RGB(16,12,32),RGB(81,77,113),RGB(195,178,178)});
  Small::WHITE = buildSmall({0,0,RGB(0,0,0),RGB(143,133,130),RGB(255,255,255)});
  Small::TEAL = buildSmall({0,0,RGB(20,69,69),RGB(85,166,166),RGB(190,239,239)});
  Small::BROWN = buildSmall({0,0,0,RGB(119,85,23),RGB(51,40,26)});
  Small::GREENW = buildSmall({0,0,RGB(0,0,0),RGB(21,71,45),RGB(42,141,97)});
  Small::BLUEW = buildSmall({0,0,RGB(0,0,0),RGB(24,79,116),RGB(78,127,166)});
  Small::REDW = buildSmall({0,0,RGB(0,0,0),RGB(128,0,4),RGB(255,0,8)});
  Small::PURPLEW = buildSmall({0,0,RGB(0,0,0),RGB(95,20,92),RGB(145,59,141)});
  Small::YELLOWW = buildSmall({0,0,RGB(0,0,0),RGB(117,103,8),RGB(235,207,17)});
  Small::GRAY_ITEM_CRAFT = buildSmall({0,0,0,0,RGB(158,150,146)});
  Small::BLINK_WHITE_GREY = buildSmall(new BlinkingPalette({3,4}, 200, 180, 180, 172, 148, 130, 600));
  
  Medium::TEAL = buildMedium({0, RGB(0,121,123), RGB(0,68,68), RGB(57,166,166), RGB(180,240,240)});
  Medium::TEAL_STROKE = buildMedium({0, RGB(0,68,68), RGB(0,68,68), RGB(57,166,166), RGB(180,240,240)});
  Medium::TEAL_BRIGHT = buildMedium({0, 0, RGB(22,97,97), RGB(90,166,166), RGB(185,240,240)});
  Medium::BLACK = buildMedium({0, 0, RGB(90,154,154), RGB(6,69,69), RGB(6,2,2)});
  Medium::BLUE_MAGIC = buildMedium({0, 0, RGB(81,60,48), RGB(97,69,36), RGB(146,146,166)});
  
  Serif::TEAL = buildSerif({0, RGB(24,68,68), RGB(24,68,68), RGB(58,166,166), RGB(243,235,231), RGB(188,238,218), RGB(197,239,217), RGB(193,239,240)});
  Serif::BROWN = buildSerif({0, 0, 0, RGB(120,74,36), RGB(96,8,14), RGB(96,8,14), RGB(96,8,14), RGB(96,8,14)});
  Serif::YELLOW_SHADOW = buildSerif({0, 0, RGB(15,49,56), RGB(115,84,69), RGB(245,161,39), RGB(229,145,31), RGB(213,133,27), RGB(213,133,27)});
  Serif::GOLD_SHADOW = buildSerif({0,  0, RGB(67,43,36),RGB(74,51,44), RGB(213,133,27), RGB(245,161,39), RGB(255,199,103), RGB(255,243,127)});
  Serif::SILVER_SHADOW = buildSerif({0, 0, RGB(67,43,36), RGB(106,97,93), RGB(159,150,146), RGB(196,186,182), RGB(228,219,215), RGB(255,255,255)});
  Serif::WHITE_SURVEY = buildSerif({0, 0, RGB(93,93,121), RGB(142,134,130), RGB(255,255,255), RGB(255,255,255), RGB(255,255,215), RGB(255,255,255)});
  Serif::DARK_BROWN = buildSerif({0, 0, 0, RGB(73, 56, 36), RGB(73, 56, 36), RGB(73, 56, 36), RGB(73, 56, 36), RGB(73, 56, 36)});
  
  Crypt::SERIF_BROWN = buildSerifCrypt({0,0,0,RGB(73, 56, 36), RGB(73, 56, 36)}); // TODO: single pixels are of same brown?
  Crypt::TINY_BROWN = buildTinyCrypt({0,0,0,RGB(97,69,36), RGB(97,69,36)}); // TODO: don't kno why main pixels are on second palette index, not first
  
  Palettes::BLINK_WHITE_BLUE = new BlinkingPalette({3,4}, 194, 228, 255, 140, 174, 255, 600);
}


s16 Fonts::vSpace = 0;
s16 Fonts::hSpace = 0;
const ColorMap* Fonts::map = nullptr;
const ColorMap* Fonts::omap = nullptr;

unordered_map<char, const ColorMap*> Fonts::fontColors = {
  {'s', &FontMap::Serif::SILVER_SHADOW},
  {'y', &FontMap::Small::YELLOW_PALE},
  {'w', &FontMap::Small::WHITE_PALE}
};

Font Fonts::fonts[] = {
  TinyFont(TEXTURE_FONT_TINY, &FontMap::Tiny::WHITE),
  TinyFont(TEXTURE_FONT_TINY, &FontMap::Tiny::WHITE_STROKE),
  TinyFont(TEXTURE_FONT_TINY, &FontMap::Tiny::YELLOW_STROKE),
  TinyFont(TEXTURE_FONT_TINY, &FontMap::Tiny::RED_STROKE),
  
  TinyCompactFont(TEXTURE_FONT_TINY_COMPACT),
  
  TinyCompactFont(TEXTURE_FONT_TINY_COMPACT_CRYPT, &FontMap::Misc::TINY_BROWN_CRYPT),
  
  SmallFont(TEXTURE_FONT_YELLOW_SMALL),
  SmallFont(TEXTURE_FONT_YELLOW_SMALL, &FontMap::Small::WHITE),
  SmallFont(TEXTURE_FONT_YELLOW_SMALL, &FontMap::Small::TEAL),
  SmallFont(TEXTURE_FONT_YELLOW_SMALL, &FontMap::Small::BROWN),
    
  SmallFont(TEXTURE_FONT_YELLOW_SMALL, &FontMap::Small::GREEN),
  SmallFont(TEXTURE_FONT_YELLOW_SMALL, &FontMap::Small::BLUE),
  SmallFont(TEXTURE_FONT_YELLOW_SMALL, &FontMap::Small::RED),
  SmallFont(TEXTURE_FONT_YELLOW_SMALL, &FontMap::Small::PURPLE),
  SmallFont(TEXTURE_FONT_YELLOW_SMALL, &FontMap::Small::YELLOW),
  
  SerifFont(TEXTURE_FONT_SERIF, &FontMap::Serif::TEAL_STROKE),
  SerifFont(TEXTURE_FONT_SERIF, &FontMap::Serif::YELLOW_SHADOW),
  SerifFont(TEXTURE_FONT_SERIF, &FontMap::Serif::GOLD_SHADOW),
  SerifFont(TEXTURE_FONT_SERIF, &FontMap::Serif::SILVER_SHADOW),
  SerifFont(TEXTURE_FONT_SERIF, &FontMap::Serif::WHITE_SURVEY),
  SerifFont(TEXTURE_FONT_SERIF, &FontMap::Serif::BROWN),
  SerifFont(TEXTURE_FONT_SERIF, &FontMap::Serif::DARK_BROWN),
  
  SerifCryptFont(TEXTURE_FONT_SERIF_CRYPT, &FontMap::Misc::SERIF_CRYPT_BROWN),
  
  MediumFont(TEXTURE_FONT_TEAL_MEDIUM),
  MediumFont(TEXTURE_FONT_TEAL_MEDIUM, &FontMap::Medium::TEAL_STROKE),
  MediumFont(TEXTURE_FONT_TEAL_MEDIUM, &FontMap::Medium::TEAL_BRIGHT),
  MediumFont(TEXTURE_FONT_TEAL_MEDIUM, &FontMap::Medium::BLACK),
  
  MediumBoldFont(TEXTURE_FONT_MEDIUM_BOLD),
  
  HugeSerifFont(TEXTURE_FONT_HUGE_SERIF)
};




Font* Fonts::font = &fonts[0];



string Fonts::format(const char *fmt_str, ...) {
  int final_n, n = static_cast<int>(strlen(fmt_str)) * 2; /* reserve 2 times as much as the length of the fmt_str */
  string str;
  unique_ptr<char[]> formatted;
  va_list ap;
  while(1) {
    formatted.reset(new char[n]); /* wrap the plain char array into the unique_ptr */
    strcpy(&formatted[0], fmt_str);
    va_start(ap, fmt_str);
    final_n = vsnprintf(&formatted[0], n, fmt_str, ap);
    va_end(ap);
    if (final_n < 0 || final_n >= n)
    {
      int r = final_n - n + 1;
      n += r > 0 ? r : -r;
    }
    else
      break;
  }
  return string(formatted.get());
}

const string Fonts::join(vector<const string>& tokens, s16 s, s16 e)
{
  string result = tokens[s];
  
  for (int i = s+1; i <= e; ++i)
    result += " "+tokens[i];

  return result;
}

void Fonts::split(string s, vector<const std::string>& tokens, s8 delim)
{
  size_t pos = 0;
  std::string token;
  while ((pos = s.find(delim)) != std::string::npos) {
    token = s.substr(0, pos);
    tokens.push_back(token);
    s.erase(0, pos + 1);
  }
  tokens.push_back(s);
}

u16 Fonts::drawString(const string string, u16 x, u16 y, TextAlign align)
{
  Gfx::bindColorMap(map);
  
  if (align != ALIGN_CENTER)
  {
    u16 w = drawStringContext(string, x, y, align);
    
    if (map) Gfx::unbindColorMap();
    
    return w;
  }
  else
  {
    Gfx::resetBuffer();
    Gfx::bindBuffer();
    u16 w = drawStringContext(string, 4, 0, ALIGN_LEFT);
    Gfx::bindCanvas();
    Gfx::mergeBuffer(1, 0, x - w/2, y, w+4, 20);
    
    if (map) Gfx::unbindColorMap();
    
    return w;
  }
}

u16 Fonts::drawStringContext(const string string, u16 x, u16 y, TextAlign align)
{
  s16 sx = align == ALIGN_RIGHT ? 0 : x, sy = y;
  u16 length = string.length();
  bool switchingColor = false;
  
  for (int i = 0; i < length; ++i)
  {
    s8 c = string[align == ALIGN_RIGHT ? length - i - 1: i];

    if (switchingColor)
    {
      if (c == '^')
      {
        if (omap)
        {
          map = omap;
          Gfx::bindColorMap(omap);
        }
        else
        {
          map = nullptr;
          Gfx::unbindColorMap();
        }
      }
      else
      {
        map = fontColors.find(c)->second;
        Gfx::bindColorMap(map);
      }
      
      switchingColor = false;
    }
    else if (c == ' ')
    {
      sx += hSpace + font->hor;
    }
    else if (c == '^')
    {
      switchingColor = true;
    }
    else
    {
      s8 cw = font->charWidth(c);
      s8 r = c - ' ';
      s8 d = align == ALIGN_RIGHT ? (s8)ceilf((font->w - cw) / 2.0f) : (font->w - cw) / 2;
            
      sx -= d;
      Gfx::draw(font->texture, r/32, r%32, align == ALIGN_RIGHT ? x - sx : sx, sy);
      sx += cw + font->hor + d;
    }
  }
  
  return sx - 1 - (align == ALIGN_RIGHT ? 0 : x);
}

u16 Fonts::drawStringBounded(const string str, int x, int y, int bound, TextAlign align, const ColorMap* map)
{
  setMap(map);
  vector<const string> lines;
  split(str, lines, '\n');
  
  if (lines.size() > 1)
  {
    int dy = 0;
    
    for (const string& line : lines)
    {
      int ndy = drawStringBounded(line, x, y+dy, bound, align) - y;
      
      if (dy == 0) dy = vSpace;
      dy += ndy;
    }
    
    return y+dy;
  }
  else
  {
    vector<const string> words;
    split(str, words, ' ');

    int s = 0, e = 0;
    
    while (e < words.size() - 1)
    {
      ++e;
      int w = font->stringWidth(join(words,s,e),hSpace);
      
      if (w > bound)
      {
        drawString(join(words,s,e-1), x, y, align);
        s = e;
        y += vSpace + font->h;
      }
    }
    
    if (s != e || e < words.size())
      drawString(join(words,s,e), x, y, align);
    
    return y + font->h;
  }
}


s16 fnts::Fonts::vSpace = 0;
s16 fnts::Fonts::hSpace = 0;
const Palette* fnts::Fonts::palette = nullptr;
const Palette* fnts::Fonts::opalette = nullptr;
const FontSpriteSheet* fnts::Fonts::font = nullptr;
unordered_map<char, const Palette*> fnts::Fonts::fontColors = {

};


string fnts::Fonts::format(const char *fmt_str, ...) {
  int final_n, n = static_cast<int>(strlen(fmt_str)) * 2; /* reserve 2 times as much as the length of the fmt_str */
  string str;
  unique_ptr<char[]> formatted;
  va_list ap;
  while(1) {
    formatted.reset(new char[n]); /* wrap the plain char array into the unique_ptr */
    strcpy(&formatted[0], fmt_str);
    va_start(ap, fmt_str);
    final_n = vsnprintf(&formatted[0], n, fmt_str, ap);
    va_end(ap);
    if (final_n < 0 || final_n >= n)
    {
      int r = final_n - n + 1;
      n += r > 0 ? r : -r;
    }
    else
      break;
  }
  return string(formatted.get());
}

const string fnts::Fonts::join(vector<const string>& tokens, s16 s, s16 e)
{
  string result = tokens[s];
  
  for (int i = s+1; i <= e; ++i)
    result += " "+tokens[i];
  
  return result;
}

void fnts::Fonts::split(string s, vector<const std::string>& tokens, s8 delim)
{
  size_t pos = 0;
  std::string token;
  while ((pos = s.find(delim)) != std::string::npos) {
    token = s.substr(0, pos);
    tokens.push_back(token);
    s.erase(0, pos + 1);
  }
  tokens.push_back(s);
}

u16 fnts::Fonts::drawString(const string string, u16 x, u16 y, TextAlign align)
{
  Gfx::bindPalette(palette);
  
  if (align != ALIGN_CENTER)
  {
    u16 w = drawStringContext(string, x, y, align);
    
    if (palette) Gfx::unbindPalette();
    
    return w;
  }
  else
  {
    Gfx::resetBuffer();
    Gfx::bindBuffer();
    u16 w = drawStringContext(string, 4, 0, ALIGN_LEFT);
    Gfx::bindCanvas();
    Gfx::mergeBuffer(1, 0, x - w/2, y, w+4, 20);
    
    if (palette) Gfx::unbindPalette();
    
    return w;
  }
}

u16 fnts::Fonts::drawStringContext(const string string, u16 x, u16 y, TextAlign align)
{
  s16 sx = align == ALIGN_RIGHT ? x - font->stringWidth(string, hSpace) : x, sy = y;
  u16 length = string.length();
  bool switchingColor = false;
  
  for (int i = 0; i < length; ++i)
  {
    s8 c = string[i];
    
    if (switchingColor)
    {
      if (c == '^')
      {
        if (opalette)
        {
          palette = opalette;
          Gfx::bindPalette(opalette);
        }
        else
        {
          palette = nullptr;
          Gfx::unbindPalette();
        }
      }
      else
      {
        palette = fontColors.find(c)->second;
        Gfx::bindPalette(palette);
      }
      
      switchingColor = false;
    }
    else if (c == ' ')
    {
      sx += font->hor + font->charWidth(' ');
    }
    else if (c == '^')
    {
      switchingColor = true;
    }
    else
    {
      s8 cw = font->charWidth(c);
      s8 r = c - ' ';
      Gfx::draw(font, 0, r, sx, sy);
      sx += cw + font->hor;
    }
  }
  
  return sx - 1 - x;
}

u16 fnts::Fonts::drawStringBounded(const string str, int x, int y, int bound, TextAlign align, const Palette* palette)
{
  if (palette)
    setMap(palette);
  
  vector<const string> lines;
  split(str, lines, '\n');
  
  if (lines.size() > 1)
  {
    int dy = 0;
    
    for (const string& line : lines)
    {
      int ndy = drawStringBounded(line, x, y+dy, bound, align) - y;
      
      if (dy == 0) dy = vSpace;
      dy += ndy;
    }
    
    return y+dy;
  }
  else
  {
    vector<const string> words;
    split(str, words, ' ');
    
    int s = 0, e = 0;
    
    while (e < words.size() - 1)
    {
      ++e;
      int w = font->stringWidth(join(words,s,e),hSpace);
      
      if (w > bound)
      {
        drawString(join(words,s,e-1), x, y, align);
        s = e;
        y += vSpace + font->sh();
      }
    }
    
    if (s != e || e < words.size())
      drawString(join(words,s,e), x, y, align);
    
    return y + font->sh();
  }
}