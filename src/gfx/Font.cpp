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

s16 Fonts::vSpace = 0;
s16 Fonts::hSpace = 0;
const ColorMap* Fonts::map = nullptr;
const ColorMap* Fonts::omap = nullptr;

unordered_map<s8, ColorMap*> Fonts::fontColors = {
  
};

Font Fonts::fonts[] = {
  TinyFont(FONT_TINY, &FontMap::Tiny::WHITE),
  TinyFont(FONT_TINY, &FontMap::Tiny::WHITE_STROKE),
  TinyFont(FONT_TINY, &FontMap::Tiny::YELLOW_STROKE),
  TinyFont(FONT_TINY, &FontMap::Tiny::RED_STROKE),
  
  TinyCompactFont(FONT_TINY_COMPACT),
  
  TinyCompactFont(FONT_TINY_COMPACT_CRYPT, &FontMap::Misc::TINY_BROWN_CRYPT),
  
  SmallFont(FONT_YELLOW_SMALL),
  SmallFont(FONT_YELLOW_SMALL, &FontMap::Small::WHITE),
  SmallFont(FONT_YELLOW_SMALL, &FontMap::Small::TEAL),
  SmallFont(FONT_YELLOW_SMALL, &FontMap::Small::BROWN),
  
  SmallShorterFont(FONT_YELLOW_SMALL),
  
  SmallFont(FONT_YELLOW_SMALL, &FontMap::Small::GREEN),
  SmallFont(FONT_YELLOW_SMALL, &FontMap::Small::BLUE),
  SmallFont(FONT_YELLOW_SMALL, &FontMap::Small::RED),
  SmallFont(FONT_YELLOW_SMALL, &FontMap::Small::PURPLE),
  SmallFont(FONT_YELLOW_SMALL, &FontMap::Small::YELLOW),
  
  SerifFont(FONT_SERIF, &FontMap::Serif::TEAL_STROKE),
  SerifFont(FONT_SERIF, &FontMap::Serif::YELLOW_SHADOW),
  SerifFont(FONT_SERIF, &FontMap::Serif::GOLD_SHADOW),
  SerifFont(FONT_SERIF, &FontMap::Serif::SILVER_SHADOW),
  SerifFont(FONT_SERIF, &FontMap::Serif::WHITE_SURVEY),
  SerifFont(FONT_SERIF, &FontMap::Serif::BROWN),
  SerifFont(FONT_SERIF, &FontMap::Serif::DARK_BROWN),
  
  SerifCryptFont(FONT_SERIF_CRYPT, &FontMap::Misc::SERIF_CRYPT_BROWN),
  
  MediumFont(FONT_TEAL_MEDIUM),
  MediumFont(FONT_TEAL_MEDIUM, &FontMap::Medium::TEAL_STROKE),
  MediumFont(FONT_TEAL_MEDIUM, &FontMap::Medium::BLACK),
  MediumFont(FONT_TEAL_MEDIUM, &FontMap::Medium::TEAL_BRIGHT),
  
  MediumBoldFont(FONT_MEDIUM_BOLD),
  
  HugeSerifFont(FONT_HUGE_SERIF)
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
  
  for (int i = s; i <= e; ++i)
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
      int w = font->stringWidth(join(words,s,e), hSpace);
      
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