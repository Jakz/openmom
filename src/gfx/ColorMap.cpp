//
//  ColorMap.cpp
//  OpenMoM
//
//  Created by Jack on 7/18/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "ColorMap.h"

#include "Gfx.h"
#include "SDL.h"

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

/*
 static const color_list SMALL_COLORS = {RGB(222, 148, 34), RGB(149, 108, 55), RGB(82, 62, 51)};
 static const color_list TINY_COLORS = {RGB(255, 255, 255), RGB(142, 132, 129), RGB(91, 91, 91)};

*/

namespace FontMap
{
  static const color_list SMALL_COLORS = {RGB(223, 150, 28), RGB(150, 109, 52), RGB(81, 60, 48)};
  static const color_list TINY_COLORS = {RGB(255, 255, 255), RGB(143, 133, 130), RGB(91, 91, 91)};
  static const color_list TINY_COMPACT_COLORS = {RGB(255, 255, 255), RGB(150, 150, 150), RGB(90, 90, 90), RGB(0,0,0)};
  static const color_list MEDIUM_COLORS = {RGB(180, 240, 240), RGB(57, 166, 166), RGB(0, 121, 123), RGB(0, 69, 69)};
  static const color_list SERIF_COLORS = {RGB(24, 69, 69),RGB(49, 129, 129),RGB(57, 166, 166),RGB(193, 239, 240),RGB(197, 239, 217),RGB(218, 246, 235),RGB(243, 235, 231)};
  
  const HashColorMap buildSmall(color_list data) { return HashColorMap(SMALL_COLORS, data); };
  const HashColorMap buildTiny(color_list data) { return HashColorMap(TINY_COLORS, data); };
  const HashColorMap buildTinyCompact(color_list data) { return HashColorMap(TINY_COMPACT_COLORS, data); };
  const HashColorMap buildMedium(color_list data) { return HashColorMap(MEDIUM_COLORS, data); };
  const HashColorMap buildSerif(color_list data) { return HashColorMap(SERIF_COLORS, data); };


  const BlinkMap Small::WHITE_GREY_BLINK = BlinkMap({SMALL_COLORS.begin()[0],SMALL_COLORS.begin()[1]}, 200, 180, 180, 172, 148, 130, 600);
  const HashColorMap Small::BLUE_MAGIC = buildSmall({RGB(146,146,166),RGB(97,97,125),RGB(40,40,65)});
  const HashColorMap Small::WHITE_PALE = buildSmall({RGB(255, 255, 255),RGB(142, 134, 130),RGB(93, 93, 121)});
  const HashColorMap Small::YELLOW_PALE = buildSmall({RGB(249, 232, 67),RGB(142, 134, 130),RGB(93, 93, 121)});
  const HashColorMap Small::RED_PALE = buildSmall({RGB(195, 178, 178),RGB(81, 77, 113),RGB(16, 12, 32)});
  const HashColorMap Small::WHITE = buildSmall({RGB(255, 255, 255),RGB(143, 133, 130),RGB(0, 0, 0)});
  const HashColorMap Small::TEAL = buildSmall({RGB(190, 239, 239),RGB(85, 166, 166), RGB(20, 69, 69)});
  const HashColorMap Small::BROWN = buildSmall({RGB(51, 40, 26),RGB(119, 85, 23), 0x00000000});
  const HashColorMap Small::GREEN = buildSmall({RGB(42, 141, 97),RGB(21, 71, 45), RGB(0, 0, 0)});
  const HashColorMap Small::BLUE = buildSmall({RGB(78, 127, 166),RGB(24, 79, 116), RGB(0, 0, 0)});
  const HashColorMap Small::RED = buildSmall({RGB(255, 0, 8),RGB(128, 0, 4), RGB(0, 0, 0)});
  const HashColorMap Small::PURPLE = buildSmall({RGB(145, 59, 141),RGB(95, 20, 92), RGB(0, 0, 0)});
  const HashColorMap Small::YELLOW = buildSmall({RGB(235, 207, 17),RGB(117, 103, 8), RGB(0, 0, 0)});
  const HashColorMap Small::GRAY_ITEM_CRAFT = buildSmall({RGB(158, 150, 146),0x00000000,0x00000000});
  
  const HashColorMap Tiny::WHITE = buildTiny({RGB(255, 255, 255), RGB(143, 133, 130), 0x00000000});
  const HashColorMap Tiny::WHITE_STROKE = buildTiny({RGB(255, 255, 255), RGB(143, 133, 130), RGB(0,0,0)});
  const HashColorMap Tiny::YELLOW_STROKE = buildTiny({RGB(213,133,27), RGB(124,82,36), RGB(0,0,0)});
  const HashColorMap Tiny::RED_STROKE = buildTiny({RGB(255,0,0), RGB(128,0,0), RGB(0,0,0)});
  
  const BlinkMap TinyCompact::WHITE_BLUE_BLINK = BlinkMap({TINY_COMPACT_COLORS.begin()[0],TINY_COMPACT_COLORS.begin()[1]}, 194, 228, 255, 140, 174, 255, 600);
  const HashColorMap TinyCompact::BROWN = buildTinyCompact({RGB(97,69,36), RGB(121,85,36), 0x00000000, 0x00000000});
  
  const HashColorMap Medium::BLUE_MAGIC = buildMedium({RGB(146,146,166),RGB(97,69,36),0x00000000,RGB(81,60,48)});
  const HashColorMap Medium::TEAL_STROKE = buildMedium({RGB(180, 240, 240),RGB(57, 166, 166),RGB(0, 68, 69),RGB(0, 69, 69)});
  const HashColorMap Medium::TEAL_BRIGHT = buildMedium({RGB(185, 240, 240),RGB(90, 166, 166),0x00000000,RGB(22, 97, 97)});
  const HashColorMap Medium::BLACK = buildMedium({RGB(6, 2, 2),RGB(6, 69, 69),0x00000000,RGB(90, 155, 154)});

  const HashColorMap Serif::TEAL_STROKE = HashColorMap({SERIF_COLORS.begin()[1], SERIF_COLORS.begin()[5]}, {RGB(24,69,69),RGB(187,238,219)});
  const HashColorMap Serif::BROWN = buildSerif({0x00000000, 0x00000000, RGB(120,74,36), RGB(96,8,14), RGB(96,8,14), RGB(96,8,14), RGB(96,8,14), });
  const HashColorMap Serif::YELLOW_SHADOW = buildSerif({RGB(15,49,56), 0x00000000, RGB(115,84,69), RGB(213,133,27), RGB(213,133,27), RGB(229,145,31), RGB(245,161,39), });
  const HashColorMap Serif::GOLD_SHADOW = buildSerif({RGB(67,43,36), 0x00000000, RGB(74,51,44), RGB(255,243,127), RGB(255,199,103), RGB(245,161,39), RGB(213,133,27), });
  const HashColorMap Serif::SILVER_SHADOW = buildSerif({RGB(67,43,36), 0x00000000, RGB(106,97,93), RGB(255,255,255), RGB(228,219,215), RGB(196,186,182), RGB(159,150,146), });
  const HashColorMap Serif::WHITE_SURVEY = buildSerif({RGB(93,93,121), 0x00000000, RGB(142,134,130), RGB(255,255,255), RGB(255,255,215), RGB(255,255,255), RGB(255,255,255), });
  const HashColorMap Serif::DARK_BROWN = buildSerif({0x00000000, 0x00000000, RGB(73, 56, 36), RGB(73, 56, 36), RGB(73, 56, 36), RGB(73, 56, 36), RGB(73, 56, 36), });
  
  const HashColorMap Misc::SERIF_CRYPT_BROWN = HashColorMap({RGB(255, 255, 255), RGB(73, 56, 36)});
  const HashColorMap Misc::TINY_BROWN_CRYPT = HashColorMap({RGB(91, 91, 91), 0x00000000, RGB(0,0,0), 0x00000000, RGB(255,255,255),RGB(97,69,36), RGB(143,133,130),RGB(121,85,36)});
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


