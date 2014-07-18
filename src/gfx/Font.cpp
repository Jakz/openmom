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
ColorMap* Fonts::map = nullptr;
ColorMap* Fonts::omap = nullptr;

unordered_map<s8, ColorMap*> Fonts::fontColors = {
  
};

Font Fonts::fonts[] = {
  TinyFont(FONT_TINY)
};


Font* Fonts::font = &fonts[0];



string Fonts::format(const string fmt_str, ...) {
  int final_n, n = ((int)fmt_str.size()) * 2; /* reserve 2 times as much as the length of the fmt_str */
  string str;
  unique_ptr<char[]> formatted;
  va_list ap;
  while(1) {
    formatted.reset(new char[n]); /* wrap the plain char array into the unique_ptr */
    strcpy(&formatted[0], fmt_str.c_str());
    va_start(ap, fmt_str);
    final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
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

u16 Fonts::drawString(const std::string string, u16 x, u16 y, TextAlign align)
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