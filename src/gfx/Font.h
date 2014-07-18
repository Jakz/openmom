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

#include <vector>

class Font
{
  
  const Texture& texture;
  s8 widths[128];
  ColorMap *map;
  
  public:

  const u16 w, h, hor, ver, space;
  const s8 charWidth(s8 c) { return widths[c]; }
  
  u16 stringWidth(std::string& str, s8 hSpace)
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
    Font(TextureID tex, u16 w, u16 h, u16 hor, u16 ver, u16 space, ColorMap* map = nullptr) :
      texture(Texture::get(tex)), w(w), h(h), hor(hor), ver(ver), space(space), map(map), widths{-1} { }
  
  
    void setWidth(std::string str, s8 width) { for (const s8 i : str) widths[i] = width; }
    void setWidth(s8 c, s8 width) { widths[c] = width; }
    void fillWidth(s8 upperWidth, s8 lowerWidth, u8 otherWidth)
    {
      for (u8 i = 0; i < 128; ++i)
      {
        if (isupper(i)) widths[i] = upperWidth;
        else if (islower(i)) widths[i] = lowerWidth;
        else widths[i] = otherWidth;
      }
    }
  
};

class SmallFont : public Font
{
  SmallFont(TextureID tex, ColorMap *map) : Font(tex,7,7,1,3,3,map)
  {
    setWidth("il1.:\'",1);
		setWidth(",()",2);
		setWidth("jtfxI+-",3);
		setWidth("VTMmWYw/",5);		
		fillWidth(4,4,4);
  }
};

#endif
