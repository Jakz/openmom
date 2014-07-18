//
//  ColorMap.cpp
//  OpenMoM
//
//  Created by Jack on 7/18/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "ColorMap.h"

#include "Gfx.h"

using namespace std;


u32 BlinkMap::get(u32 k)
{
  if (set.find(k) == set.end()) return 0x00000000;
  else
  {
    s16 phase = Gfx::getRealTicks() % ticks;
    
    if (updatedForTick != phase)
    {
      float progress = phase / (float) ticks;
      
      if (progress <= 0.5f) progress *= 2.0f;
      else progress = (0.5f - (progress - 0.5f)) * 2.0f;
      
      u8 red = s[0] + (u8)(d[0]*progress);
      u8 green = s[1] + (u8)(d[1]*progress);
      u8 blue = s[2] + (u8)(d[2]*progress);
      
      color = Gfx::color(red,green,blue);
    }
    
    return color;
  }
}

#define RGB(r,g,b) Gfx::color(r,g,b)
#define RGBA(r,g,b,a) Gfx::colora(r,g,b,a)

const HashColorMap smallGrayItemCraftName = HashColorMap({
  RGB(81, 60, 48), RGBA(0, 0, 0, 0),
  RGB(223, 150, 28), RGB(158, 150, 146),
  RGB(150, 109, 52), RGBA(0, 0, 0, 0)
});

const HashColorMap whiteSmallFontMap = HashColorMap({
	RGB(81, 60, 48), RGB(0, 0, 0),
	RGB(223, 150, 28), RGB(255, 255, 255),
	RGB(150, 109, 52), RGB(143, 133, 130)
});

const HashColorMap tealSmallFontMap = HashColorMap({
	RGB(81, 60, 48), RGB(20, 69, 69),
	RGB(223, 150, 28), RGB(190, 239, 239),
	RGB(150, 109, 52), RGB(85, 166, 166)
});

const HashColorMap greenColorSmall = HashColorMap({
  RGB(81, 60, 48), RGB(0, 0, 0),
  RGB(223, 150, 28), RGB(42, 141, 97),
  RGB(150, 109, 52), RGB(21, 71, 45)
});

const HashColorMap blueColorSmall = HashColorMap({
  RGB(81, 60, 48), RGB(0, 0, 0),
  RGB(223, 150, 28), RGB(78, 127, 166),
  RGB(150, 109, 52), RGB(24, 79, 116)
});

const HashColorMap redColorSmall = HashColorMap({
  RGB(81, 60, 48), RGB(0, 0, 0),
  RGB(223, 150, 28), RGB(255, 0, 8),
  RGB(150, 109, 52), RGB(128, 0, 4)
});

const HashColorMap purpleColorSmall = HashColorMap({
  RGB(81, 60, 48), RGB(0, 0, 0),
  RGB(223, 150, 28), RGB(145, 59, 141),
  RGB(150, 109, 52), RGB(95, 20, 92)
});

const HashColorMap yellowColorsmall = HashColorMap({
  RGB(81, 60, 48), RGB(0, 0, 0),
  RGB(223, 150, 28), RGB(235, 207, 17),
  RGB(150, 109, 52), RGB(117, 103, 8)
});

const HashColorMap darkBrownFont = HashColorMap({
	RGB(81, 60, 48), 0x00000000,
	RGB(223, 150, 28), RGB(51, 40, 26),
	RGB(150, 109, 52), RGB(119, 85, 23)
});

const HashColorMap strokeTealMediumFont = HashColorMap({
	RGB(0, 121, 123), RGB(0, 68, 69)
});

const HashColorMap brightTealMediumFont = HashColorMap({
	RGB(180, 240, 240), RGB(185, 240, 240),
	RGB(57, 166, 166), RGB(90, 166, 166),
	RGB(0, 121, 123), 0x00000000,
	RGB(0, 69, 69), RGB(22, 97, 97),
});

const HashColorMap blackMediumFont = HashColorMap({
  RGB(180, 240, 240), RGB(6, 2, 2),
  RGB(57, 166, 166), RGB(6, 69, 69),
  RGB(0, 121, 123), 0x00000000,
  RGB(0, 69, 69), RGB(90, 155, 154),
});

const HashColorMap whiteTiny = HashColorMap({ RGB(91, 91, 91), 0x00000000});
const HashColorMap whiteStrokeTiny = HashColorMap({ RGB(91, 91, 91), RGB(0,0,0)});

const HashColorMap yellowStrokeTiny = HashColorMap({
  RGB(91, 91, 91), RGB(0,0,0),
  RGB(255,255,255),RGB(213,133,27),
  RGB(143,133,130),RGB(124,82,36)
});

const HashColorMap redStrokeTiny = HashColorMap({
  RGB(91, 91, 91), RGB(0,0,0),
  RGB(255,255,255),RGB(255,0,0),
  RGB(143,133,130),RGB(128,0,0)
});

const HashColorMap tinyBrown = HashColorMap({
  RGB(91, 91, 91), 0x00000000,
  RGB(0,0,0), 0x00000000,
  RGB(255,255,255),RGB(97,69,36),
  RGB(143,133,130),RGB(121,85,36)
});