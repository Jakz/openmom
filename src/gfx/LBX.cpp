//
//  LBX.cpp
//  OpenMoM
//
//  Created by Jack on 7/30/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "LBX.h"

#include "Common.h"
#include "Gfx.h"

#include <iostream>

#include <cstdio>
#include <string>
#include <dirent.h>
#include <vector>

#include <SDL2/SDL.h>

using namespace std;

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

const u16 GLYPH_COUNT = 96;

class SpriteSheet
{
  
public:
  virtual const u8* dataAt(u16 x = 0, u16 y = 0) const = 0;
  virtual u16 rows() const = 0;
  virtual u16 columns() const = 0;
  
  virtual u16 w(u16 x = 0) const = 0;
  virtual u16 h(u16 x = 0) const = 0;
};

class FontData : public SpriteSheet
{
private:
  u8* data[96];
  u8 widths[96];
  const u8 height;
  
public:
  FontData(FontType type, u8 height, u8 colorCount) : type(type), height(height)
  {
    //for (int i = 0; i < GLYPH_COUNT; ++i)
    //  data[i] = new u8[width*height];
  }
  
  ~FontData()
  {
    for (int i = 0; i < GLYPH_COUNT; ++i)
      delete [] data[i];
  }
  
  void setGlyph(u8 index, u8 width, u8* data)
  {
    this->data[index] = data;
    this->widths[index] = width;
  }
  
  const u8* dataAt(u16 x, u16 y) const override { return data[x]; }
  u16 rows() const override { return 1; }
  u16 columns() const override { return GLYPH_COUNT; }
  u16 w(u16 x) const override { return widths[x]; }
  u16 h(u16 y) const override { return height; }
  
  
  const FontType type;
  
  static constexpr const u8 LIGHT_STROKE_VALUE = 0x80;
  static constexpr const u8 DARK_STROKE_VALUE = 0x81;
  
  static FontData* fonts[FONT_TYPE_COUNT];
};

FontData* FontData::fonts[FONT_TYPE_COUNT] = {nullptr};



const Color BLACK_ALPHA = RGB(0, 255, 0);
const Color TRANSPARENT = RGB(255, 0, 255);

static const Color PALETTE[256] =
{
  RGB(0x0, 0x0, 0x0),
  RGB(0x8, 0x4, 0x4),
  RGB(0x24, 0x1c, 0x18),
  RGB(0x38, 0x30, 0x2c),
  RGB(0x48, 0x40, 0x3c),
  RGB(0x58, 0x50, 0x4c),
  RGB(0x68, 0x60, 0x5c),
  RGB(0x7c, 0x74, 0x70),
  RGB(0x8c, 0x84, 0x80),
  RGB(0x9c, 0x94, 0x90),
  RGB(0xac, 0xa4, 0xa0),
  RGB(0xc0, 0xb8, 0xb4),
  RGB(0xd0, 0xc8, 0xc4),
  RGB(0xe0, 0xd8, 0xd4),
  RGB(0xf0, 0xe8, 0xe4),
  RGB(0xfc, 0xfc, 0xfc),
  RGB(0x38, 0x20, 0x1c),
  RGB(0x40, 0x2c, 0x24),
  RGB(0x48, 0x34, 0x2c),
  RGB(0x50, 0x3c, 0x30),
  RGB(0x58, 0x40, 0x34),
  RGB(0x5c, 0x44, 0x38),
  RGB(0x60, 0x48, 0x3c),
  RGB(0x64, 0x4c, 0x3c),
  RGB(0x68, 0x50, 0x40),
  RGB(0x70, 0x54, 0x44),
  RGB(0x78, 0x5c, 0x4c),
  RGB(0x80, 0x64, 0x50),
  RGB(0x8c, 0x70, 0x58),
  RGB(0x94, 0x74, 0x5c),
  RGB(0x9c, 0x7c, 0x64),
  RGB(0xa4, 0x84, 0x68),
  RGB(0xec, 0xc0, 0xd4),
  RGB(0xd4, 0x98, 0xb4),
  RGB(0xbc, 0x74, 0x94),
  RGB(0xa4, 0x54, 0x7c),
  RGB(0x8c, 0x38, 0x60),
  RGB(0x74, 0x24, 0x4c),
  RGB(0x5c, 0x10, 0x34),
  RGB(0x44, 0x4, 0x20),
  RGB(0xec, 0xc0, 0xc0),
  RGB(0xd4, 0x94, 0x94),
  RGB(0xbc, 0x74, 0x74),
  RGB(0xa4, 0x54, 0x54),
  RGB(0x8c, 0x38, 0x38),
  RGB(0x74, 0x24, 0x24),
  RGB(0x5c, 0x10, 0x10),
  RGB(0x44, 0x4, 0x4),
  RGB(0xec, 0xd4, 0xc0),
  RGB(0xd4, 0xb4, 0x98),
  RGB(0xbc, 0x98, 0x74),
  RGB(0xa4, 0x7c, 0x54),
  RGB(0x8c, 0x60, 0x38),
  RGB(0x74, 0x4c, 0x24),
  RGB(0x5c, 0x34, 0x10),
  RGB(0x44, 0x24, 0x4),
  RGB(0xec, 0xec, 0xc0),
  RGB(0xd4, 0xd4, 0x94),
  RGB(0xbc, 0xbc, 0x74),
  RGB(0xa4, 0xa4, 0x54),
  RGB(0x8c, 0x8c, 0x38),
  RGB(0x74, 0x74, 0x24),
  RGB(0x5c, 0x5c, 0x10),
  RGB(0x44, 0x44, 0x4),
  RGB(0xd4, 0xec, 0xbc),
  RGB(0xb8, 0xd4, 0x98),
  RGB(0x98, 0xbc, 0x74),
  RGB(0x7c, 0xa4, 0x54),
  RGB(0x60, 0x8c, 0x38),
  RGB(0x4c, 0x74, 0x24),
  RGB(0x38, 0x5c, 0x10),
  RGB(0x24, 0x44, 0x4),
  RGB(0xc0, 0xec, 0xc0),
  RGB(0x98, 0xd4, 0x98),
  RGB(0x74, 0xbc, 0x74),
  RGB(0x54, 0xa4, 0x54),
  RGB(0x38, 0x8c, 0x38),
  RGB(0x24, 0x74, 0x24),
  RGB(0x10, 0x5c, 0x10),
  RGB(0x4, 0x44, 0x4),
  RGB(0xc0, 0xec, 0xd8),
  RGB(0x98, 0xd4, 0xb8),
  RGB(0x74, 0xbc, 0x98),
  RGB(0x54, 0xa4, 0x7c),
  RGB(0x38, 0x8c, 0x60),
  RGB(0x24, 0x74, 0x4c),
  RGB(0x10, 0x5c, 0x38),
  RGB(0x4, 0x44, 0x24),
  RGB(0xf4, 0xc0, 0xa0),
  RGB(0xe0, 0xa0, 0x84),
  RGB(0xcc, 0x84, 0x6c),
  RGB(0xc8, 0x8c, 0x68),
  RGB(0xa8, 0x78, 0x54),
  RGB(0x98, 0x68, 0x4c),
  RGB(0x8c, 0x60, 0x44),
  RGB(0x7c, 0x50, 0x3c),
  RGB(0xc0, 0xd8, 0xec),
  RGB(0x94, 0xb4, 0xd4),
  RGB(0x70, 0x98, 0xbc),
  RGB(0x54, 0x7c, 0xa4),
  RGB(0x38, 0x64, 0x8c),
  RGB(0x24, 0x4c, 0x74),
  RGB(0x10, 0x38, 0x5c),
  RGB(0x4, 0x24, 0x44),
  RGB(0xc0, 0xc0, 0xec),
  RGB(0x98, 0x98, 0xd4),
  RGB(0x74, 0x74, 0xbc),
  RGB(0x54, 0x54, 0xa4),
  RGB(0x3c, 0x38, 0x8c),
  RGB(0x24, 0x24, 0x74),
  RGB(0x10, 0x10, 0x5c),
  RGB(0x4, 0x4, 0x44),
  RGB(0xd8, 0xc0, 0xec),
  RGB(0xb8, 0x98, 0xd4),
  RGB(0x98, 0x74, 0xbc),
  RGB(0x7c, 0x54, 0xa4),
  RGB(0x60, 0x38, 0x8c),
  RGB(0x4c, 0x24, 0x74),
  RGB(0x38, 0x10, 0x5c),
  RGB(0x24, 0x4, 0x44),
  RGB(0xec, 0xc0, 0xec),
  RGB(0xd4, 0x98, 0xd4),
  RGB(0xbc, 0x74, 0xbc),
  RGB(0xa4, 0x54, 0xa4),
  RGB(0x8c, 0x38, 0x8c),
  RGB(0x74, 0x24, 0x74),
  RGB(0x5c, 0x10, 0x5c),
  RGB(0x44, 0x4, 0x44),
  RGB(0xd8, 0xd0, 0xd0),
  RGB(0xc0, 0xb0, 0xb0),
  RGB(0xa4, 0x90, 0x90),
  RGB(0x8c, 0x74, 0x74),
  RGB(0x78, 0x5c, 0x5c),
  RGB(0x68, 0x4c, 0x4c),
  RGB(0x5c, 0x3c, 0x3c),
  RGB(0x48, 0x2c, 0x2c),
  RGB(0xd0, 0xd8, 0xd0),
  RGB(0xb0, 0xc0, 0xb0),
  RGB(0x90, 0xa4, 0x90),
  RGB(0x74, 0x8c, 0x74),
  RGB(0x5c, 0x78, 0x5c),
  RGB(0x4c, 0x68, 0x4c),
  RGB(0x3c, 0x5c, 0x3c),
  RGB(0x2c, 0x48, 0x2c),
  RGB(0xc8, 0xc8, 0xd4),
  RGB(0xb0, 0xb0, 0xc0),
  RGB(0x90, 0x90, 0xa4),
  RGB(0x74, 0x74, 0x8c),
  RGB(0x5c, 0x5c, 0x78),
  RGB(0x4c, 0x4c, 0x68),
  RGB(0x3c, 0x3c, 0x5c),
  RGB(0x2c, 0x2c, 0x48),
  RGB(0xd8, 0xdc, 0xec),
  RGB(0xc8, 0xcc, 0xdc),
  RGB(0xb8, 0xc0, 0xd4),
  RGB(0xa8, 0xb8, 0xcc),
  RGB(0x9c, 0xb0, 0xcc),
  RGB(0x94, 0xac, 0xcc),
  RGB(0x88, 0xa4, 0xcc),
  RGB(0x88, 0x94, 0xdc),
  RGB(0xfc, 0xf0, 0x90),
  RGB(0xfc, 0xe4, 0x60),
  RGB(0xfc, 0xc8, 0x24),
  RGB(0xfc, 0xac, 0xc),
  RGB(0xfc, 0x78, 0x10),
  RGB(0xd0, 0x1c, 0x0),
  RGB(0x98, 0x0, 0x0),
  RGB(0x58, 0x0, 0x0),
  RGB(0x90, 0xf0, 0xfc),
  RGB(0x60, 0xe4, 0xfc),
  RGB(0x24, 0xc8, 0xfc),
  RGB(0xc, 0xac, 0xfc),
  RGB(0x10, 0x78, 0xfc),
  RGB(0x0, 0x1c, 0xd0),
  RGB(0x0, 0x0, 0x98),
  RGB(0x0, 0x0, 0x58),
  RGB(0xfc, 0xc8, 0x64),
  RGB(0xfc, 0xb4, 0x2c),
  RGB(0xec, 0xa4, 0x24),
  RGB(0xdc, 0x94, 0x1c),
  RGB(0xcc, 0x88, 0x18),
  RGB(0xbc, 0x7c, 0x14),
  RGB(0xa4, 0x6c, 0x1c),
  RGB(0x8c, 0x60, 0x24),
  RGB(0x78, 0x54, 0x24),
  RGB(0x60, 0x44, 0x24),
  RGB(0x48, 0x38, 0x24),
  RGB(0x34, 0x28, 0x1c),
  RGB(0x90, 0x68, 0x34),
  RGB(0x90, 0x64, 0x2c),
  RGB(0x94, 0x6c, 0x34),
  RGB(0x94, 0x70, 0x40),
  RGB(0x8c, 0x5c, 0x24),
  RGB(0x90, 0x64, 0x2c),
  RGB(0x90, 0x68, 0x30),
  RGB(0x98, 0x78, 0x4c),
  RGB(0x60, 0x3c, 0x2c),
  RGB(0x54, 0xa4, 0xa4),
  RGB(0xc0, 0x0, 0x0),
  RGB(0xfc, 0x88, 0xe0),
  RGB(0xfc, 0x58, 0x84),
  RGB(0xf4, 0x0, 0xc),
  RGB(0xd4, 0x0, 0x0),
  RGB(0xac, 0x0, 0x0),
  RGB(0xe8, 0xa8, 0xfc),
  RGB(0xe0, 0x7c, 0xfc),
  RGB(0xd0, 0x3c, 0xfc),
  RGB(0xc4, 0x0, 0xfc),
  RGB(0x90, 0x0, 0xbc),
  RGB(0xfc, 0xf4, 0x7c),
  RGB(0xfc, 0xe4, 0x0),
  RGB(0xe4, 0xd0, 0x0),
  RGB(0xa4, 0x98, 0x0),
  RGB(0x64, 0x58, 0x0),
  RGB(0xac, 0xfc, 0xa8),
  RGB(0x74, 0xe4, 0x70),
  RGB(0x0, 0xbc, 0x0),
  RGB(0x0, 0xa4, 0x0),
  RGB(0x0, 0x7c, 0x0),
  RGB(0xac, 0xa8, 0xfc),
  RGB(0x80, 0x7c, 0xfc),
  RGB(0x0, 0x0, 0xfc),
  RGB(0x0, 0x0, 0xbc),
  RGB(0x0, 0x0, 0x7c),
  RGB(0x30, 0x30, 0x50),
  RGB(0x28, 0x28, 0x48),
  RGB(0x24, 0x24, 0x40),
  RGB(0x20, 0x1c, 0x38),
  RGB(0x1c, 0x18, 0x34),
  RGB(0x18, 0x14, 0x2c),
  RGB(0x14, 0x10, 0x24),
  RGB(0x10, 0xc, 0x20),
  RGB(0xa0, 0xa0, 0xb4),
  RGB(0x88, 0x88, 0xa4),
  RGB(0x74, 0x74, 0x90),
  RGB(0x60, 0x60, 0x80),
  RGB(0x50, 0x4c, 0x70),
  RGB(0x40, 0x3c, 0x60),
  RGB(0x30, 0x2c, 0x50),
  RGB(0x24, 0x20, 0x40),
  RGB(0x18, 0x14, 0x30),
  RGB(0x10, 0xc, 0x20),
  RGB(0x14, 0xc, 0x8),
  RGB(0x18, 0x10, 0xc),
  RGB(0x0, 0x0, 0x0),
  RGB(0x0, 0x0, 0x0),
  RGB(0x0, 0x0, 0x0),
  RGB(0x0, 0x0, 0x0),
  RGB(0x0, 0x0, 0x0),
  RGB(0x0, 0x0, 0x0),
  RGB(0x0, 0x0, 0x0),
  RGB(0x0, 0x0, 0x0),
  RGB(0x0, 0x0, 0x0),
  RGB(0x0, 0x0, 0x0),
  RGB(0x0, 0x0, 0x0),
  RGB(0x0, 0x0, 0x0)
};

const vector<string> findFiles(string path, const char *ext)
{
  vector<string> packs;
  
  
  
  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir (path.c_str())) != NULL) {
    while ((ent = readdir (dir)) != NULL) {
      if (strncmp(ent->d_name + strlen(ent->d_name) - 4, ext, 4) == 0)
      {
        packs.push_back(ent->d_name);
        //        printf ("%s\n", ent->d_name);
        
      }
    }
    closedir (dir);
  } else {
    /* could not open directory */
    //perror ("");
  }
  
  return packs;
}



struct LBXArray
{
  u16 count;
  u16 size;
} __attribute__((__packed__));

/*
void scanArray(LBXOffset offset, LBXArray& info, FILE *in)
{
  fseek(in, offset, SEEK_SET);
  
  char *buffer = new char[info.size];
  
  for (int i = 0; i < info.count; ++i)
  {
    fread(buffer, 1, info.size, in);
    printf("  > %s\n",buffer);
  }
}*/

/*
void scanArrayFile(LBXHeader& header, LBXOffset* offsets, FILE *in)
{
  LBXArray* arrays = new LBXArray[header.count];
  for (int i = 0; i < header.count; ++i)
  {
    fseek(in, offsets[i], SEEK_SET);
    fread(&arrays[i], sizeof(LBXArray), 1, in);
  }
  
  for (int i = 0; i < header.count; ++i)
  {
    LBXOffset startArrayOffset = offsets[i]+arrays[i].size*(arrays[i].count)+sizeof(LBXArray);
    u32 nextEntryOffset = offsets[i+1];
    
    printf("Array at 0x%08X, %4d elements, %4d size, ends at 0x%08X (next at: 0x%08X)\n",
           offsets[i],arrays[i].count,arrays[i].size,startArrayOffset,nextEntryOffset);
    
    if (startArrayOffset != nextEntryOffset) cout << "ERROR! OFFSET DIFFERS!" << endl;
    else
    {
      scanArray(offsets[i]+sizeof(LBXArray), arrays[i], in);
    }
  }
}*/

struct LBXFileName
{
  char folder[8];
  char padding;
  char name[22];
  char padding2;
} __attribute__((__packed__));

struct LBXGfxHeader
{
  u16 width;
  u16 height;
  u16 unknown1;
  u16 count;
  u16 unknown2[3];
  u16 paletteOffset;
  u16 unknown3;
} __attribute__((__packed__));

struct LBXPaletteHeader
{
  u16 offset;
  u16 firstIndex;
  u16 count;
  u16 unknown;
  
} __attribute__((__packed__));

struct LBXPaletteEntry
{
  u8 r;
  u8 g;
  u8 b;
} __attribute__((__packed__));

void scanGfxFrame(LBXGfxHeader& header, LBXPaletteHeader &pheader, u16 index, Color* palette, Color* image, u8* data, u32 dataLength)
{
  s32 i = 0, x = 0;
  const u32 w = header.width, h = header.height;
  s32 y = 0;
  
  //printf(">>>>>>>>>>>>>>>>>> FRAME %d\n", index);
  
  if (index > 0 && data[i] == 1)
  {
    //printf("CLEARING FRAME %d\n", index);
    for (int k = 0; k < header.width*header.height; ++k)
      static_cast<u32*>(image)[k] = TRANSPARENT;
  }
  
  ++i;
  
  u32 RLE_val = 0;
  u32 n_r = 0;
  u32 longData = 0;
  u32 lastPos = 0;
  u32 nextCtl = 0;
  u32 rleLength = 0;
  u32 rleCounter = 0;
  
  while (x < w && i < dataLength)
  {
    y = 0;
    
    if (data[i] == 0xFF)
    {
      ++i;
      RLE_val = pheader.firstIndex + pheader.count;
    }
    else
    {
      longData = data[i+2];
      nextCtl = i + data[i+1] + 2;
      
      if (data[i] == 0x00) RLE_val = pheader.firstIndex + pheader.count;
      else if (data[i] == 0x80) RLE_val = 0xE0;
      //else printf("UNRECOGNIZED RLE VALUE: %u\n", data[i]);
      
      //printf("%d,%d..%d COLUMN SKIPPED\n", x, y, data[i+3]-1);
      y = data[i + 3];
      i += 4;
      
      n_r = i;
      
      while (n_r < nextCtl)
      {
        while (n_r < i + longData && x < w)
        {
          if (data[n_r] >= RLE_val)
          {

            lastPos = n_r + 1;
            rleLength = data[n_r] - RLE_val + 1;
            rleCounter = 0;
            
            //printf("%d,%d..%d RLE -> %d\n", x, y, y+rleCounter, data[lastPos]);
            
            while (rleCounter < rleLength && y < h)
            {
              if (x >= 0 && x < w && y >= 0 && y < h)
              {
                image[x + y*w] = palette[data[lastPos]];
              }
              //else printf("RLE OVERRUN: %d,%d\n",x,y);
              
              ++y;
              ++rleCounter;
            }

            n_r += 2;
          }
          else
          {
            if (x >= 0 && x < w && y >= 0 && y < h)
            {
              image[x + y*w] = palette[data[n_r]];
              //printf("%d,%d SINGLE -> %d\n", x, y, data[n_r]);

            }
            
            ++n_r;
            ++y;
          }
        }
        
        if (n_r < nextCtl)
        {
          //printf("ANOTHER RLE\n");
          y += data[n_r + 1];
          i = n_r + 2;
          longData = data[n_r];
          n_r += 2;
        }
      }
      
      i = nextCtl;
    }
    
    /*if (y != h)
      printf("%d,%d..%d -> EMPTY COLUMN\n", x, y, h-1);*/
    
    ++x;
  }
  
}

void LBX::scanGfx(LBXHeader& header, LBXOffset offset, FILE *in)
{
  fseek(in, offset, SEEK_SET);
  LBXGfxHeader gfxHeader;
  fread(&gfxHeader, sizeof(LBXGfxHeader), 1, in);
  printf("    WxH: %dx%d, COUNT: %d, PALETTE: %d\n", gfxHeader.width, gfxHeader.height, gfxHeader.count, gfxHeader.paletteOffset);
  
  LBXOffset *frameOffsets = new LBXOffset[gfxHeader.count+1];
  fread(frameOffsets, sizeof(LBXOffset), gfxHeader.count+1, in);
  
  // create palette from standard palette
  Color* palette = new Color[256];
  memcpy(palette, PALETTE, sizeof(Color)*256);

  LBXPaletteHeader paletteHeader = {0,0,256};
  
  // there is a palette
  if (gfxHeader.paletteOffset > 0)
  {
    fseek(in, offset+gfxHeader.paletteOffset, SEEK_SET);
    
    fread(&paletteHeader, sizeof(LBXPaletteHeader), 1, in);
    printf("    PALETTE AT OFFSET %d, COUNT %d STARTING AT %d\n", paletteHeader.offset, paletteHeader.count, paletteHeader.firstIndex);

    fseek(in, offset+paletteHeader.offset, SEEK_SET);
    for (int i = 0; i < paletteHeader.count; ++i)
    {
      LBXPaletteEntry paletteEntry;
      fread(&paletteEntry, sizeof(LBXPaletteEntry), 1, in);
      
      palette[paletteHeader.firstIndex + i] = (0xFF << 24) | (paletteEntry.r << 18) | (paletteEntry.g << 10) | (paletteEntry.b << 2);
    }
  }
  
  // fix palette colors
  for (int i = 0; i < 256; ++i)
  {
    if (palette[i] == 0xFFA0A0B4 || palette[i] == 0xFF8888A4)
      palette[i] = BLACK_ALPHA; //palette[i] = 0x80000000;
    else if (i == 0)
      palette[i] = TRANSPARENT; //palette[i] = 0x00000000;
  }
  
  SDL_Surface *image = Gfx::createSurface(gfxHeader.width, gfxHeader.height);
  Gfx::lock(image);
  SDL_FillRect(image, nullptr, TRANSPARENT);
  
  for (int i = 0; i < gfxHeader.count; ++i)
  {
    u32 dataSize = frameOffsets[i+1] - frameOffsets[i];
    //Color* image = new Color[gfxHeader.width*gfxHeader.height];
    
    fseek(in, offset+frameOffsets[i], SEEK_SET);
    u8* data = new u8[dataSize];
    fread(data, sizeof(u8), dataSize, in);
    printf("      FRAME %d AT OFFSET %d (SIZE: %d)\n", i, frameOffsets[i], dataSize);
    
    
    scanGfxFrame(gfxHeader, paletteHeader, i, palette, static_cast<Color*>(image->pixels), data, dataSize);
    Gfx::unlock(image);
    
    SDL_SaveBMP(image, (to_string(offset)+" "+to_string(i)+".bmp").c_str());
    
    
    delete [] data;
  }
  
  SDL_FreeSurface(image);
  
  delete [] palette;
}

void LBX::scanFileNames(LBXHeader& header, offset_list& offsets, FILE *in)
{
  fseek(in, offsets[0] - sizeof(LBXFileName)*header.count, SEEK_SET);
  
  LBXFileName *names = new LBXFileName[header.count];
  
  fread(names, sizeof(LBXFileName), header.count, in);
  
  //int i = 25;
  for (int i = 0; i < header.count; ++i)
  {
    printf("> (%d) %s/%s (%u %08X)\n", i, names[i].folder, names[i].name, offsets[i], offsets[i]);
    //scanGfx(header, offsets[i], in);
  }
}

static std::string path;

bool LBX::loadHeader(LBXHeader& header, vector<LBXOffset>& offsets, FILE *in)
{
  fread(&header, sizeof(header), 1, in);
  
  if (header.magic == 0x0000FEAD)
  {
    offsets.resize(header.count);
    fread(&offsets[0], sizeof(LBXOffset), header.count+1, in);
    
    return true;
  }
  
  return false;
}

static const u16 FONT_NUM = 8;
static const u16 FONT_CHAR_NUM = 0x5E;

void LBX::loadFonts(LBXHeader& header, vector<LBXOffset>& offsets, FILE *in)
{
  // position to start of character heights resource
  fseek(in, offsets[0] + 0x16A, SEEK_SET);

  u16 *heights = new u16[8];
  fread(heights, FONT_NUM, sizeof(u16), in);
  
  // position to start of character widths
  fseek(in, offsets[0] + 0x19A, SEEK_SET);
  
  u8 **widths = new u8*[FONT_NUM];
  
  // read widths
  for (int i = 0; i < FONT_NUM; ++i)
  {
    // read 5E bytes for widths of current font
    widths[i] = new u8[FONT_CHAR_NUM]; // 94
    fread(widths[i], 1, FONT_CHAR_NUM, in);

    // skip 2 control characters
    u8 padding[2];
    fread(padding, 1, 2, in);
    if (padding[0] != 0 || padding[1] != 0)
      printf("ERROR!\n");
  }
  
  
  // position to start of character offsets
  fseek(in, offsets[0] + 0x49A, SEEK_SET);
  
  u16 **foffsets = new u16*[FONT_NUM];
  
  for (int i = 0; i < FONT_NUM; ++i)
  {
    foffsets[i] = new u16[FONT_CHAR_NUM];
    
    fread(foffsets[i], sizeof(u16), FONT_CHAR_NUM, in);
    
    u16 padding[2];
    fread(&padding, sizeof(u16), 2, in);
  }
  
  //int i = 0;
  for (int i = 0; i < FONT_NUM; ++i)
  {
    // allocate storage for all glyphs
    u8** glyphs = new u8*[GLYPH_COUNT];
    for (int j = 0; j < GLYPH_COUNT; ++j)
      glyphs[j] = new u8[(heights[i]+2)*(widths[i][j]+2)]; // add 2 pixels by side for precomputed stroke

    
    /*u8 width = 0;
    for (int j = 0; j < FONT_CHAR_NUM; ++j) width = std::max(widths[i][j], width);
    width += 2;*/
    
    //SDL_Surface *image = Gfx::createSurface(FONT_CHAR_NUM*(width), heights[i]+2);
    //Gfx::lock(image);
    //u32* pixels = static_cast<u32*>(image->pixels);

    
    for (int j = 0; j < FONT_CHAR_NUM; ++j)
    {
      u8* glyph = glyphs[j];
      u8 width = widths[i][j]+2;
      
      printf("      CHAR %d (%c)\n",j,j+32);
      
      fseek(in, offsets[0] + foffsets[i][j], SEEK_SET);

      u16 bx = 1;
      u16 by = 1;
      u16 x = 0, y = 0;
      u8 data;
      u8 maxColor = 0;
      
      while (x < widths[i][j])
      {
        fread(&data, 1, 1, in);
        
        u8 high = (data >> 4) & 0x0F;
        u8 low = data & 0x0F;
        
        u8 color;
        u8 strain = 0;
        
        if (high == 0x08)
        {
          color = 0;
          strain = low;
          
          if (strain > 0)
            printf("%d,%d..%d STRAIN TRANSPARENT\n", x, y, y+strain-1);
          else
            printf("%d,%d..%d SKIPPING\n", x, y, heights[i]-1);
        }
        else
        {
          printf(">>>>>>>>>>>>>>>>>>>>>>>> LOW %d\n", low);
          color = low;
          maxColor = std::max(maxColor, color);
          strain = high;
          
          if (strain > 0)
            printf("%d,%d..%d STRAIN %8X\n", x, y, y+strain-1, color);
          else
            printf("%d,%d..%d SKIPPING\n", x, y, heights[i]-1);
        }
        
        if (strain == 0)
        {
          ++x;
          y = 0;
        }
        else
        {
          for (int k = 0; k < strain; ++k)
          {
            if (color > 0)
              glyph[bx+x + (by+y)*width] = color;
            ++y;
          }
        }
      }

      s8 dirs[8][2] = {{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1}};
      
      // precompute font stroke
      for (int x = 0; x < width; ++x)
        for (int y = 0; y < heights[i]+2; ++y)
        {
          bool hasLightStroke = false, hasDarkStroke = false;
          
          for (int d = 0; d < 8; ++d)
          {
            int dx = x + dirs[d][0];
            int dy = y + dirs[d][1];
            
            if (dx >= 0 && dy >= 0 && dx < width && dy < heights[i]+2)
            {
              u8 pixel = glyph[dx + dy*width];
              
              if (pixel <= maxColor && glyph[x + y*width] == 0)
              {
                hasLightStroke |= (d == 1 || d == 2 || d == 3 || d == 4 || d == 5 || d == 6);
                hasDarkStroke |= (d == 0 || ((d == 7 || d == 6) && (pixel > 0)) );
              }
            }
          }
          
          if (hasDarkStroke)
            glyph[x + y*width] = FontData::DARK_STROKE_VALUE;
          else if (hasLightStroke)
            glyph[x + y*width] = FontData::LIGHT_STROKE_VALUE;
          
        }
      
      
      // pass the glyph data to the specific font
      FontData::fonts[i] = new FontData(static_cast<FontType>(i), heights[i], maxColor);
      for (int j = 0; j < GLYPH_COUNT; ++j)
        FontData::fonts[i]->setGlyph(j, widths[i][j]+2, glyphs[j]);

    }
    
    delete [] glyphs;
  }
  
  
  
  /*for (int i = 0; i < FONT_NUM; ++i)
  {
    printf("Font %d, height: %d\n  ", i, heights[i]);
    for (int j = 0; j < 0x5E; ++j)
      printf("%d(%04X) ", widths[i][j], foffsets[i][j]);
    printf("\n");
  }*/
  
}


void LBX::load()
{
  path = string(getenv("PWD")) + "/OpenMoM.app/Contents/Resources/data/lbx/";
  
  auto files = findFiles(path, ".lbx");
  
  for (auto f : files)
  {
    if (f == "fonts.lbx")
    {
      auto filePath = path + f;
      
      LBXHeader header;
      offset_list offsets;
      
      FILE *in = fopen(filePath.c_str(), "rb");
      
      loadHeader(header, offsets, in);
      loadFonts(header, offsets, in);
      
      fclose(in);
    }
  }
  
  
}

