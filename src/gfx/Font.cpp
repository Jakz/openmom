//
//  Font.cpp
//  OpenMoM
//
//  Created by Jack on 18/07/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "Font.h"

#include "Gfx.h"
#include "common/mystrings.h"

using namespace std;


FontData* FontData::fonts[FONT_TYPE_COUNT] = {nullptr};

const FontFaces::Spacings& FontFaces::defaultSpacingForType(FontType type)
{
  const static Spacings spacings[FONT_TYPE_COUNT] = {
    { 1, -2 },
    { 1,  2 },
    { 1,  3 },
    { 1,  2 },
    { 1, -1 },
    { 1,  0 },
    { 1, -1 },
    { 1,  3 }
  };
  
  return spacings[type];
}

const FontSpriteSheet* FontFaces::Tiny::WHITE = nullptr;
const FontSpriteSheet* FontFaces::Tiny::WHITE_STROKE = nullptr;
const FontSpriteSheet* FontFaces::Tiny::YELLOW_STROKE = nullptr;
const FontSpriteSheet* FontFaces::Tiny::RED_STROKE = nullptr;
const FontSpriteSheet* FontFaces::Tiny::BROWN = nullptr;
const FontSpriteSheet* FontFaces::Tiny::GOLD_COMBAT = nullptr;
const FontSpriteSheet* FontFaces::Tiny::BLACK_COMBAT = nullptr;

const FontSpriteSheet* FontFaces::Small::YELLOW = nullptr;
const FontSpriteSheet* FontFaces::Small::BLUE_MAGIC = nullptr;
const FontSpriteSheet* FontFaces::Small::WHITE_PALE = nullptr;
const FontSpriteSheet* FontFaces::Small::YELLOW_PALE = nullptr;
const FontSpriteSheet* FontFaces::Small::RED_PALE = nullptr;
const FontSpriteSheet* FontFaces::Small::WHITE = nullptr;
const FontSpriteSheet* FontFaces::Small::TEAL = nullptr;
const FontSpriteSheet* FontFaces::Small::TEAL_DARK = nullptr;
const FontSpriteSheet* FontFaces::Small::BROWN = nullptr;
const FontSpriteSheet* FontFaces::Small::BROWN_HELP = nullptr;
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

const FontSpriteSheet* FontFaces::MediumBold::BROWN_ITEM_CRAFT = nullptr;
const FontSpriteSheet* FontFaces::MediumBold::GRAY_ITEM_CRAFT = nullptr;
const FontSpriteSheet* FontFaces::MediumBold::GOLD_ITEM_CRAFT = nullptr;


const FontSpriteSheet* FontFaces::Serif::TEAL = nullptr;
const FontSpriteSheet* FontFaces::Serif::BROWN = nullptr;
const FontSpriteSheet* FontFaces::Serif::BROWN_HELP = nullptr;
const FontSpriteSheet* FontFaces::Serif::YELLOW_SHADOW = nullptr;
const FontSpriteSheet* FontFaces::Serif::GOLD_SHADOW = nullptr;
const FontSpriteSheet* FontFaces::Serif::SILVER_SHADOW = nullptr;
const FontSpriteSheet* FontFaces::Serif::WHITE_SURVEY = nullptr;
const FontSpriteSheet* FontFaces::Serif::DARK_BROWN = nullptr;

const FontSpriteSheet* FontFaces::Serif::BLACK_INFO_MENU = nullptr;
const FontSpriteSheet* FontFaces::Serif::BLACK_INFO_MENU_HOVER = nullptr;

const FontSpriteSheet* FontFaces::Crypt::SERIF_BROWN = nullptr;
const FontSpriteSheet* FontFaces::Crypt::TINY_BROWN = nullptr;

const FontSpriteSheet* FontFaces::Huge::GOLD = nullptr;
const FontSpriteSheet* FontFaces::Huge::BROWN_CARTOGRAPHER = nullptr;

const BlinkingPalette* FontFaces::Palettes::BLINK_WHITE_BLUE = nullptr;

const Palette* FontFaces::Palettes::SMALL_WHITE_PALE = nullptr;
const Palette* FontFaces::Palettes::SMALL_YELLOW_PALE = nullptr;
const Palette* FontFaces::Palettes::WHITE_PRODUCTION = nullptr;


/*FONT_TINY = 0,
FONT_SMALL,
FONT_MEDIUM,
FONT_MEDIUM_THICK,
FONT_SERIF,
FONT_HUGE,
FONT_TINY_CRYPT,
FONT_SERIF_CRYPT,*/

static constexpr int vspacings[FONT_TYPE_COUNT] = {-2, 2, 3, 2, -1, 0, -1, -3 };

const FontSpriteSheet* build(FontType type, const Palette* palette) { return new FontSpriteSheet(FontData::fonts[type], palette, 1, vspacings[type]); }

const FontSpriteSheet* buildTiny(color_list colors) { return new FontSpriteSheet(FontData::fonts[FONT_TINY], colors, 1, -2); }
const FontSpriteSheet* buildTinyCrypt(color_list colors) { return new FontSpriteSheet(FontData::fonts[FONT_TINY_CRYPT], colors, 1, -1); }
const FontSpriteSheet* buildSmall(color_list colors) { return new FontSpriteSheet(FontData::fonts[FONT_SMALL], colors, 1, 2); }
const FontSpriteSheet* buildMedium(color_list colors) { return new FontSpriteSheet(FontData::fonts[FONT_MEDIUM], colors, 1, 3); }
const FontSpriteSheet* buildMediumBold(color_list colors) { return new FontSpriteSheet(FontData::fonts[FONT_MEDIUM_THICK], colors, 1, 2); }

const FontSpriteSheet* buildSerif(color_list colors) { return new FontSpriteSheet(FontData::fonts[FONT_SERIF], colors, 1, -1); }
const FontSpriteSheet* buildSerifCrypt(color_list colors) { return new FontSpriteSheet(FontData::fonts[FONT_SERIF_CRYPT], colors, 1, -3); }

const FontSpriteSheet* buildSmall(const Palette *palette) { return new FontSpriteSheet(FontData::fonts[FONT_SMALL], palette, 1, 2); }


namespace fonts
{
  template<FontType FONT, typename TYPE>
  SpecificFontFace<FONT, TYPE>::SpecificFontFace(const Palette* palette) : FontSpriteSheet(FontData::fonts[FONT], palette, 1, vspacings[FONT]) { }
  
  /* color indices: background, high shadow, edge shadow, low shadow, unused?, main color, middle dots, single pixels */
  MediumBoldFont::MediumBoldFont(Color color) : MediumBoldFont(color, 0) { }
  //TODO: should single shadow pixels considered part of low shadow?
  MediumBoldFont::MediumBoldFont(Color color, Color shadow) : SpecificFontFace<FONT_MEDIUM_THICK, MediumBoldFont>(new IndexedPalette({0, 0, 0, shadow, 0, color, color, color})) { }
  
  /* color indices: background, high shadow, edge shadow, low shadow, single pixels, stripes x 4 (low to high) */
  SerifFont::SerifFont(const std::array<Color,9>& colors) : SerifFont(new IndexedPalette(colors)) { }
  SerifFont::SerifFont(Color color) : SerifFont(new IndexedPalette({0, 0, 0, 0, color, color, color, color, color})) { }
  SerifFont::SerifFont(Color color, Color shadow) : SerifFont(new IndexedPalette({0, 0, 0, shadow, color, color, color, color, color})) { }
  
  const SerifFont* SerifFont::withStripes(const std::array<Color, 4>& stripes, Color single) { return new SerifFont(new IndexedPalette({0, 0, 0, 0, single, stripes[0], stripes[1], stripes[2], stripes[3]})); }
  
  const SerifFont* SerifFont::withShadow(const SerifFont* font, Color shadow) { return withShadowAndSingle(font, font->getPalette()->get(4), shadow); }
  const SerifFont* SerifFont::withShadowAndSingle(const SerifFont* font, Color single, Color shadow) {
    const Palette* p = font->getPalette();
    return new SerifFont(new IndexedPalette({0, 0, 0, shadow, single, p->get(5), p->get(6), p->get(7), p->get(8)}));
  }


  
  /* color indices: background, high shadow, edge stroke, low shadow, single pixels, main */
  const TinyFont* TinyFont::of(Color color, Color single, Color shadow) { return new TinyFont(new IndexedPalette({0, 0, 0, shadow, single, color})); }
  
  
  
  
  
  
  const SerifFont* base::SERIF_GOLD = nullptr;
  
}


/*
//              small, tiny, medium, serif, tiny crypt, serif crypt
s8 xadjust[] = {  -1,   -1,    -1,    0,      -1,         -1};
s8 yadjust[] = {  -1,    0,     0,    0,       0,          0};
*/

enum color_indices
{
  font_background = 0,
  font_light_stroke = 1,
  font_edge_stroke = 2,
  font_dark_stroke = 3,
  font_first_free = font_dark_stroke + 1,
  
  tiny_single = font_first_free,
  tiny_main = font_first_free + 1,
};

namespace FontPalettes
{
  // bg, light stroke, edge stroke, dark stroke, single, main
  static const Palette* tiny(Color main) { return new IndexedPalette({0,0,0,0, main, main}); }
  static const Palette* tiny(Color main, Color single) { return new IndexedPalette({0,0,0,0, single, main}); }
  //TODO: should edge stroke be part of shadow?
  static const Palette* tinyWithShadow(Color main, Color single, Color shadow) { return new IndexedPalette({0,0,0, shadow, single, main}); }
  static const Palette* tinyWithStroke(Color main, Color single, Color stroke) { return new IndexedPalette({0, stroke, stroke, stroke, single, main}); }

  /* color indices: bg, light stroke, edge stroke, dark stroke, stripes x 4 (low to high) */
  static const Palette* serif(Color main) { return new IndexedPalette({0,0,0,0, main, main, main, main, main} ); }
  static const Palette* serif(Color main, Color single) { return new IndexedPalette({0,0,0,0, single, main, main, main, main} ); }
  static const Palette* serifWithShadow(Color main, Color single, Color shadow) { return new IndexedPalette({0,0,0, shadow, single, main, main, main, main} ); }

};

void FontFaces::buildFonts()
{
  
  Palettes::SMALL_WHITE_PALE = new IndexedPalette({0,0,0, {93,93,121}, {142,134,130}, {255,255,255}});
  Palettes::SMALL_YELLOW_PALE = new IndexedPalette({0,0,0,{93,93,121},{142,134,130},{249,232,67}});
  Palettes::WHITE_PRODUCTION = new IndexedPalette({0,0, /*{73,65,60}*/0, 0, {121,93,77}, {255,255,255}}); // TODO: bugged, stroke is different
  
  FontData::fonts[FONT_MEDIUM]->setGlyphWidth(' '-' ', 1);
  FontData::fonts[FONT_SERIF_CRYPT]->setGlyphWidth(' '-' ', 3);

  using namespace FontPalettes;
  
  /* color indices: background, high shadow, low shadow, single pixels, main */
  /*Tiny::WHITE = build(FONT_TINY, tinyWithShadow(Color::WHITE, {143,133,130}, Color::BLACK));
  Tiny::WHITE_STROKE = build(FONT_TINY, tinyWithStroke(Color::WHITE, {144,133,130}, Color::BLACK));
  Tiny::YELLOW_STROKE = build(FONT_TINY, tinyWithStroke({213,133,27}, {124,82,36}, Color::BLACK));*/
  
  
  Tiny::WHITE = buildTiny({0, 0, 0, {0,0,0}, {143,133,130}, {255,255,255}});
  Tiny::WHITE_STROKE = buildTiny({0, {0,0,0}, 0, {0,0,0}, {143,133,130}, {255,255,255}});
  Tiny::YELLOW_STROKE = buildTiny({0, {0,0,0}, 0, {0,0,0}, {124,82,36}, {213,133,27}});
  Tiny::RED_STROKE = buildTiny({0, {0,0,0}, 0, {0,0,0}, {128,0,0}, {255,0,0}});
  Tiny::BROWN = buildTiny({0, 0, 0, 0, {121,85,36}, {97,69,36}});
  Tiny::GOLD_COMBAT = buildTiny({0, 0, 0, {39,27,24}, {185,105,7}, {255,176,6}});
  Tiny::BLACK_COMBAT = buildTiny({0, 0, 0, {120,132,148},{120,132,148},{32,32,36}});

  
  Small::YELLOW = buildSmall({0,0,0,{81,60,48},{150,109,52},{223,150,28}});
  Small::BLUE_MAGIC = buildSmall({0,0,0,{40,40,65},{97,97,125},{146,146,166}});
  Small::WHITE_PALE = buildSmall(Palettes::SMALL_WHITE_PALE);
  Small::YELLOW_PALE = buildSmall(Palettes::SMALL_YELLOW_PALE);
  Small::RED_PALE = buildSmall({0,0,0,{16,12,32},{81,77,113},{195,178,178}});
  Small::WHITE = buildSmall({0,0,0,{0,0,0},{143,133,130},{255,255,255}});
  Small::TEAL = buildSmall({0,0,0,{20,69,69},{85,166,166},{190,239,239}});
  Small::TEAL_DARK = buildSmall({0,0,0,0,{39,77,77},{31,60,60}});
  Small::BROWN = buildSmall({0,0,0,0,{119,85,23},{51,40,26}});
  Small::BROWN_HELP = new FontSpriteSheet(FontData::fonts[FONT_SMALL], {0,0,0,0,{121,85,36},{69,36,4}}, 1, 1);
  Small::GREENW = buildSmall({0,0,0,{0,0,0},{21,71,45},{42,141,97}});
  Small::BLUEW = buildSmall({0,0,0,{0,0,0},{24,79,116},{78,127,166}});
  Small::REDW = buildSmall({0,0,0,{0,0,0},{128,0,4},{255,0,8}});
  Small::PURPLEW = buildSmall({0,0,0,{0,0,0},{95,20,92},{145,59,141}});
  Small::YELLOWW = buildSmall({0,0,0,{0,0,0},{117,103,8},{235,207,17}});
  Small::GRAY_ITEM_CRAFT = buildSmall({0,0,0,0,0,{158,150,146}});
  Small::BLINK_WHITE_GREY = buildSmall(new BlinkingPalette({3,4}, 200, 180, 180, 172, 148, 130, 600));
  
  Medium::TEAL = buildMedium({0, {0,121,123}, 0,{0,68,68}, {57,166,166}, {180,240,240}});
  Medium::TEAL_STROKE = buildMedium({0, {0,68,68}, 0,{0,68,68}, {57,166,166}, {180,240,240}});
  Medium::TEAL_BRIGHT = buildMedium({0, 0, 0, {22,97,97}, {90,166,166}, {185,240,240}});
  Medium::BLACK = buildMedium({0, 0, 0, {90,154,154}, {6,69,69}, {6,2,2}});
  Medium::BLUE_MAGIC = buildMedium({0, 0, 0, {81,60,48}, {97,69,36}, {146,146,166}});
  
  /* color indices: background, high shadow, edge shadow, low shadow, unused?, main color, middle dots, single pixels */
  MediumBold::BROWN_ITEM_CRAFT = buildMediumBold({0, {73,52,44}, 0, {56,32,28}, 0, {166,134,105}, {150,117,93}, {142,113,89}});
  MediumBold::GOLD_ITEM_CRAFT = buildMediumBold({0, {73,52,44}, 0, {56,32,28}, 0, {255,182,43}, {239,166,35}, {223,150,27}});
  MediumBold::GRAY_ITEM_CRAFT = buildMediumBold({0, 0, 0, {65,43,35}, 0, {158,150,146}, {142,134,130}, {142,134,130}});

  
  /* color indices: background, high shadow, edge shadow, low shadow, single pixels, stripes x 4 (low to high) */
  
  Serif::TEAL = buildSerif({0, {24,68,68}, 0, {24,68,68}, {58,166,166}, {243,235,231}, {188,238,218}, {197,239,217}, {193,239,240}});
  Serif::BROWN = buildSerif({0, 0, 0, 0, {120,74,36}, {96,8,14}, {96,8,14}, {96,8,14}, {96,8,14}});
  Serif::BROWN_HELP = buildSerif({0, 0, 0, 0, {97,69,36}, {69,4,4}, {69,4,4}, {69,4,4}, {69,4,4}});
  Serif::YELLOW_SHADOW = buildSerif({0, 0, 0, {15,49,56}, {115,84,69}, {245,161,39}, {229,145,31}, {213,133,27}, {213,133,27}});
  
  fonts::base::SERIF_GOLD = fonts::SerifFont::withStripes({{{213,133,27}, {245,161,39}, {255,199,103}, {255,243,127}}}, {255,174,12});

  Serif::GOLD_SHADOW = buildSerif({0, 0, 0, {67,43,36}, {74,51,44}, {213,133,27}, {245,161,39}, {255,199,103}, {255,243,127}});
  Serif::SILVER_SHADOW = buildSerif({0, 0, 0, {67,43,36}, {106,97,93}, {159,150,146}, {196,186,182}, {228,219,215}, {255,255,255}});  // TODO: take from fontColors map
  Serif::WHITE_SURVEY = buildSerif({0, 0, 0, {93,93,121}, {142,134,130}, {255,255,255}, {255,255,255}, {255,255,255}, {255,255,255}});
  Serif::DARK_BROWN = buildSerif({0, 0, 0, 0, {73,56,36}, {73,56,36}, {73,56,36}, {73,56,36}, {73,56,36}});
  
  Serif::BLACK_INFO_MENU = build(FONT_SERIF, serifWithShadow({8,4,4}, {117,77,36}, {166, 109, 28}));
  Serif::BLACK_INFO_MENU_HOVER = build(FONT_SERIF, serifWithShadow({8,4,4}, {166,109,28}, {239, 166, 36}));
  
  
  Crypt::SERIF_BROWN = buildSerifCrypt({0,0,0,0,{73,56,36}, {73,56,36}}); // TODO: single pixels are of same brown?
  Crypt::TINY_BROWN = buildTinyCrypt({0,0,0,0,{97,69,36}, {97,69,36}}); // TODO: don't kno why main pixels are on second palette index, not first
  
  color_list hugeGoldPalette = {
    0, 0, 0, {0,0,0}, 0,
    {215,182,154},
    {255,203,101},
    {255,203,36},
    {239,166,36},
    {223,159,28},
    {166,125,85},
    {207,138,24},
    {166,109,28},
    {121,85,36},
    {97,69,36},
    {101,89,0},
    {73,56,36}
  };
  
  Huge::GOLD = new FontSpriteSheet(FontData::fonts[FONT_HUGE], hugeGoldPalette, 1, 0);

  
  color_list brownCartographer = {
    0, {105, 73, 35}, {105, 73, 35}, {105, 73, 35}, 0,
    {6, 6, 2},
    {6, 6, 2},
    {6, 6, 2},
    {6, 6, 2},
    {6, 6, 2},
    {6, 6, 2},
    {6, 6, 2},
    {6, 6, 2},
    {6, 6, 2},
    {6, 6, 2},
    {6, 6, 2},
    {6, 6, 2},
  };
  
  // 105 73 35
  Huge::BROWN_CARTOGRAPHER = new FontSpriteSheet(FontData::fonts[FONT_HUGE], brownCartographer, 1, 0);

  
  
  Palettes::BLINK_WHITE_BLUE = new BlinkingPalette({3,4}, 194, 228, 255, 140, 174, 255, 600);
}


s16 Fonts::vSpace = 0;
s16 Fonts::hSpace = 0;
s16 Fonts::spaceCharAdj = 0;
const Palette* Fonts::palette = nullptr;
const Palette* Fonts::opalette = nullptr;
const FontSpriteSheet* Fonts::font = nullptr;
unordered_map<char, const Palette*> Fonts::fontColors = {
  {'s', new IndexedPalette({0, 0, {67,43,36}, {106,97,93}, {159,150,146}, {196,186,182}, {228,219,215}, {255,255,255}})},
  {'w', FontFaces::Palettes::SMALL_WHITE_PALE},
  {'y', FontFaces::Palettes::SMALL_YELLOW_PALE}
};


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

u16 Fonts::drawString(const string& string, u16 x, u16 y, TextAlign align)
{
  Gfx::bindPalette(palette);
  
  if (align == ALIGN_CENTER)
    x -= stringWidth(font,string)/2;
  
  //if (align != ALIGN_CENTER)
  {
    u16 w = drawStringContext(string, x, y, align);
    
    if (palette) Gfx::unbindPalette();
    
    return w;
  }
  /*else
  {
    Gfx::resetBuffer();
    Gfx::bindBuffer();
    u16 w = drawStringContext(string, 4, 0, ALIGN_LEFT);
    Gfx::bindCanvas();
    Gfx::mergeBuffer(1, 0, x - w/2, y, w+4, 20);
    
    if (palette) Gfx::unbindPalette();
    
    return w;
  }*/
}

u16 Fonts::drawStringContext(const string& string, u16 x, u16 y, TextAlign align)
{
  u16 plength = stringWidth(font, string);
  s16 sx = align == ALIGN_RIGHT ? x - plength : x, sy = y;
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
      sx += font->hor + font->charWidth(' ') + spaceCharAdj;
    }
    else if (c == '^')
    {
      switchingColor = true;
    }
    else
    {
      s8 cw = font->charWidth(c);
      s8 r = c - ' ';
      Gfx::draw(font, sx, sy, 0, r);
      sx += cw + font->hor;
    }
  }
  
  return plength;
}

/* TODO: these bounded string draw functions must be radically optimized to avoid splitting joining and such, possibly
   by working on individual characters */

u16 Fonts::drawStringBounded(const string& str, int x, int y, int bound, TextAlign align, const Palette* palette)
{
  if (palette)
    setPalette(palette);
  
  vector<string> lines;
  strings::split(str, lines, '\n');
  
  /* if there are multiple lines then for each line we have a separated procedure since a newline
     resets the position they can be considered as indipendent */
  if (lines.size() > 1)
  {
    int dy = y;
    
    for (const string& line : lines)
    {
      int ndy = drawStringBounded(line, x, dy, bound, align);
      
      dy = ndy;
    }
    
    return dy;
  }
  else
  {
    vector<string> words;
    strings::split(str, words, ' ');
    
    int s = 0, e = 0;
    
    while (e < words.size() - 1)
    {
      ++e;
      int w = stringWidth(font,strings::join(words,s,e));
      
      if (w > bound)
      {
        drawString(strings::join(words,s,e-1), x, y, align);
        s = e;
        y += vSpace + font->sh();
      }
    }
    
    if (s != e || e < words.size())
      drawString(strings::join(words,s,e), x, y, align);
    
    return y + vSpace + font->sh();
  }
}

std::pair<std::string, u16> Fonts::drawStringBounded(const std::string& text, Point p, int maxWidth, int maxLines, TextAlign align)
{  
  /* TODO: \n are ignored and collapsed as delimiters, maybe this should enhanced */
  vector<string> words;
  strings::split(text, words, " \n");
  
  int s = 0, e = 0;
  int l = 0;
  
  while (e < words.size() - 1)
  {
    if (l >= maxLines)
      return std::make_pair(strings::join(words, s, words.size()-1), p.y);
    
    ++e;
    /* try to add a new word and check the total line length */
    int w = stringWidth(font, strings::join(words, s, e));
    
    /* we go over bound by adding the word, then print current line */
    if (w > maxWidth)
    {
      drawString(strings::join(words, s, e-1), p, align);
      s = e;
      p.y += vSpace + font->sh();
      ++l;
    }
  }
  
  if (l < maxLines && (s != e || e < words.size()))
  {
    drawString(strings::join(words,s,e), p, align);
    p.y += vSpace + font->sh();
    s = e;
  }
  
  return std::make_pair(strings::join(words, s, words.size()-1), p.y);
}
