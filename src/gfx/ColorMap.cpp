//
//  ColorMap.cpp
//  OpenMoM
//
//  Created by Jack on 7/18/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "ColorMap.h"

#include "gfx/Gfx.h"
#include "gfx/SDLHelper.h"

using namespace std;


u32 BlinkMap::get(u32 k) const
{
  if (set.find(k) == set.end()) return 0x00000000;
  else
  {
    s16 phase = Gfx::ticks % ticks;
    
    if (updatedForTick != phase)
    {
      float progress = phase / (float) ticks;
      
      if (progress <= 0.5f) progress *= 2.0f;
      else progress = (0.5f - (progress - 0.5f)) * 2.0f;
      
      u8 red = s[0] + (u8)(d[0]*progress);
      u8 green = s[1] + (u8)(d[1]*progress);
      u8 blue = s[2] + (u8)(d[2]*progress);
      
      return Gfx::color(red,green,blue);
    }
    
    return 0x00000000;
  }
}

const MiscMaps::GrayscaleMap MiscMaps::GRAYSCALE = MiscMaps::GrayscaleMap();

const HashColorMap MiscMaps::FLAG_COLORS_MAP[6] = {
  HashColorMap({}),
  HashColorMap({
    RGB(0, 188, 0), RGB(150,182,215),
    RGB(0, 164, 0), RGB(113,154,190),
    RGB(0, 124, 0), RGB(85,125,166)
  }),
  HashColorMap({
    RGB(0, 188, 0), RGB(215,0,0),
    RGB(0, 164, 0), RGB(211,28,0),
    RGB(0, 124, 0), RGB(142,56,56)
  }),
  HashColorMap({
    RGB(0, 188, 0), RGB(211,60,255),
    RGB(0, 164, 0), RGB(199,0,255),
    RGB(0, 124, 0), RGB(146,0,190)
  }),
  HashColorMap({
    RGB(0, 188, 0), RGB(255,231,0),
    RGB(0, 164, 0), RGB(231,211,0),
    RGB(0, 124, 0), RGB(166,154,0)
  }),
  HashColorMap({
    RGB(0, 188, 0), RGB(117,77,36),
    RGB(0, 164, 0), RGB(93,52,16),
    RGB(0, 124, 0), RGB(69,36,4)
  })
};

const Color MiscMaps::SCHOOL_GLOW_COLORS[][5] = {
  {0},
  // chaos
  {
    RGB(215,150,150),
    RGB(190,117,117),
    RGB(166,85,85),
    RGB(142,56,56),
    RGB(117,36,36)
  },
  // death
  {
    RGB(239,195,215),
    RGB(215,154,182),
    RGB(190,117,150),
    RGB(166,85,125),
    RGB(142,56,97)
  },
  // life
  {
    RGB(255,255,255),
    RGB(243,235,231),
    RGB(227,219,215),
    RGB(211,203,199),
    RGB(195,186,182)
  },
  // nature
  {
    RGB(215,239,190),
    RGB(186,215,154),
    RGB(154,190,117),
    RGB(125,166,85),
    RGB(97,142,92)
  },
  // sorcery
  {
    RGB(195,219,239),
    RGB(150,182,215),
    RGB(113,154,190),
    RGB(85,125,166),
    RGB(56,101,142)
  }
};


Color BlinkingPalette::get(u8 index) const
{
  if (indices.find(index) == indices.end()) return 0x00000000;
  else
  {
    s16 phase = Gfx::ticks % ticks;
    
    if (updatedForTick != phase)
    {
      float progress = phase / (float) ticks;
      
      if (progress <= 0.5f) progress *= 2.0f;
      else progress = (0.5f - (progress - 0.5f)) * 2.0f;
      
      u8 red = s[0] + (u8)(d[0]*progress);
      u8 green = s[1] + (u8)(d[1]*progress);
      u8 blue = s[2] + (u8)(d[2]*progress);
      
      return Gfx::color(red,green,blue);
    }
    
    return 0x00000000;
  }
}
