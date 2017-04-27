//
//  LBX.cpp
//  OpenMoM
//
//  Created by Jack on 7/30/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "LBX.h"

#include "Buildings.h"
#include "Localization.h"
#include "Help.h"

#include "Font.h"
#include "Common.h"
#include "Gfx.h"

#include <iostream>

#include <cstdio>
#include <string>
#include <dirent.h>
#include <vector>

#include "SDL.h"
#include "platform.h"

/* FORMAT */

/* HEADER
 
 2 bytes element count
 4 bytes magic number (0x0000FEAD)
 2 bytes type
 
 */

struct LBXTerrainInfo
{
  const char* name;
  size_t count;
};

using namespace std;
using namespace lbx;

const Color BLACK_ALPHA = {0,255,0};
const Color TRANSPARENT = {255,0,255};

constexpr size_t TERRAIN_COUNT_PER_PLANE = 0x2FA;
constexpr size_t TERRAIN_BASE = 768;
constexpr size_t TERRAIN_ENTRY_LENGTH = 384;
constexpr size_t TERRAIN_ENTRY_HEADER = 16;

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



LBXArrayData* LBX::loadArray(LBXOffset offset, FILE *in)
{
  LBXArray arrayHeader;
  
  fseek(in, offset, SEEK_SET);
  fread(&arrayHeader, sizeof(LBXArray), 1, in);
 
  LBXArrayData* data = new LBXArrayData(arrayHeader.count, arrayHeader.size);
  
  for (size_t i = 0; i < data->count; ++i)
    fread(data->data[i], 1, data->size, in);

  return data;
}


void LBX::loadArray(LBXOffset offset, LBXArray& info, const TextFiller& inserter, FILE *in)
{
  fseek(in, offset, SEEK_SET);
  
  char* buffer = new char[info.size];
  
  for (int i = 0; i < info.count; ++i)
  {
    fread(buffer, 1, info.size, in);
    //printf("  > %s\n",buffer);
    inserter.push(string(buffer));
  }
  
  delete [] buffer;
}


void LBX::loadArrayFile(const FileInfo& info, std::vector<TextFiller>& inserters, FILE *in)
{
  const auto& header = info.header;
  const auto& offsets = info.offsets;
  
  LBXArray* arrays = new LBXArray[header.count];
  for (int i = 0; i < header.count; ++i)
  {
    fseek(in, offsets[i], SEEK_SET);
    fread(&arrays[i], sizeof(LBXArray), 1, in);
  }
  
  for (int i = 0; i < header.count; ++i)
  {
    LBXOffset computedNextArrayOffset = offsets[i]+arrays[i].size*(arrays[i].count)+sizeof(LBXArray);
    u32 nextEntryOffset = offsets[i+1];
    
    /*printf("Array at 0x%08X, %4d elements, %4d size, ends at 0x%08X (next at: 0x%08X)\n",
     offsets[i],arrays[i].count,arrays[i].size,startArrayOffset,nextEntryOffset);*/
    
    if (computedNextArrayOffset != nextEntryOffset) cout << "ERROR! OFFSET DIFFERS!" << endl;
    else
    {
      loadArray(offsets[i]+sizeof(LBXArray), arrays[i], inserters[i], in);
    }
  }
  
  delete [] arrays;
}


void LBX::scanGfxFrame(const LBXGfxHeader& header, const LBXPaletteHeader& pheader, u16 index, u8* image, u8* data, u32 dataLength)
{
  s32 i = 0, x = 0;
  const u32 w = header.width, h = header.height;
  s32 y = 0;
  
  /* first byte is used to tell if a successive frame should be cleared
     or if it's a copy of the previous: 1 means clear, 0 means copy
   */
  if (index > 0 && data[i] == 1)
    std::fill(image, image + header.width*header.height, 0);

  ++i;
  
  u32 RLE_val = 0;
  
  while (x < w && i < dataLength)
  {
    y = 0;

    /* if current data is the special 0xFF control character
     we reset RLE_val and we skip until the end of column
     */
    if (data[i] == 0xFF)
    {
      RLE_val = pheader.firstIndex + pheader.count;
      ++i;
      ++x;
      continue;
    }
    /* each control structure is made by 2 bytes in the following way:
       r rle value type (start from palette or start from default 0xE0 index)
       c distance to next control sequence (add 2 since it starts after the subsequence structure always present)
    */
    if (data[i] == 0x00)
      RLE_val = pheader.firstIndex + pheader.count;
    else if (data[i] == 0x80)
      RLE_val = 0xE0;

    else printf("UNRECOGNIZED RLE VALUE: %u\n", data[i]);
    
    u32 columnDataLength = data[i + 1];
    u32 nextColumnPosition = i + columnDataLength + 2;
    
    //printf("%d,%d..%d COLUMN SKIPPED\n", x, y, data[i+3]-1);
    i += 2;
    
    /* when we arrive here n_r starts from the next byte of data,
       while we're before the next control sequence
     */
    while (i < nextColumnPosition)
    {
      /* each subsequence is made by 2 bytes in the following way:
         l length of the subsequence of data values
         y a delta adjustment to y used to skip pixels in the column
       */
      u32 subsequenceEnd = i + data[i] + 2;
      y += data[i + 1];
      i += 2;

      /* while we are before the end of the subsequence */
      while (i < subsequenceEnd && x < w)
      {
        /* if we have an RLE value */
        if (data[i] >= RLE_val)
        {
          /* RLE structure is made by two bytes: first byte is the length,
           second byte is the color index to repeat */
          u32 rleLength = data[i] - RLE_val + 1;
          u8 colorIndex = data[i + 1];
          u32 rleCounter = 0;
          
          //printf("%d,%d..%d RLE -> %d\n", x, y, y+rleCounter, data[lastPos]);
          
          /* mind that the RLE sequence can't go over to next column */
          while (rleCounter < rleLength && y < h)
          {
            assert(x >= 0 && x < w && y >= 0 && y < h);
            image[x + y*w] = colorIndex;
            
            ++y;
            ++rleCounter;
          }
          
          i += 2;
        }
        /* if value found wasn't an RLE value then we're just setting
           single pixels */
        else
        {
          assert(x >= 0 && x < w && y >= 0 && y < h);
          image[x + y*w] = data[i];
          //printf("%d,%d SINGLE -> %d\n", x, y, data[n_r]);
          ++i;
          ++y;
        }
      }
    }
    
    /*if (y != h)
     printf("%d,%d..%d -> EMPTY COLUMN\n", x, y, h-1);*/
    
    ++x;
  }
  
}

LBXSpriteData* LBX::scanGfx(const LBXHeader& header, LBXOffset offset, FILE *in)
{
  fseek(in, offset, SEEK_SET);
  LBXGfxHeader gfxHeader;
  fread(&gfxHeader, sizeof(LBXGfxHeader), 1, in);
  assert(gfxHeader.alwaysZero1 == 0 && gfxHeader.alwaysZero2 == 0 && gfxHeader.alwaysZero3 == 0);
  //if (gfxHeader.count == 1 && gfxHeader.unknown2 == 1)
  //  assert(false);
  //rassert(!((gfxHeader.count > 1) ^ (gfxHeader.unknown2 != 0)));
  LOGD("[lbx]   WxH: %dx%d, frames: %d, palette: %c", gfxHeader.width, gfxHeader.height, gfxHeader.count, gfxHeader.paletteOffset ? 'y' : 'n');
  
  LBXOffset *frameOffsets = new LBXOffset[gfxHeader.count+1];
  fread(frameOffsets, sizeof(LBXOffset), gfxHeader.count+1, in);
  
  // create palette from standard palette
  const u16 PALETTE_SIZE = 256;
  Color* palette = new Color[PALETTE_SIZE];
  memcpy(palette, Gfx::mainPalette->raw(), sizeof(Color)*PALETTE_SIZE);
  
  LBXSpriteData *sprite = new LBXSpriteData(new IndexedPalette(palette), gfxHeader.count, gfxHeader.frameLoopRestart, gfxHeader.width, gfxHeader.height, gfxHeader.paletteOffset > 0);
  LBXPaletteHeader paletteHeader = {0,0,256};
  
  sprite->unknown2 = gfxHeader.unknown2;
  sprite->unknown3 = gfxHeader.unknown3;

  // there is a palette
  if (gfxHeader.paletteOffset > 0)
  {
    fseek(in, offset+gfxHeader.paletteOffset, SEEK_SET);
    
    fread(&paletteHeader, sizeof(LBXPaletteHeader), 1, in);
    LOGD("[lbx]     palette at offset %d, count %d starting at %d", paletteHeader.offset, paletteHeader.count, paletteHeader.firstIndex);
    
    fseek(in, offset+paletteHeader.offset, SEEK_SET);
    for (int i = 0; i < paletteHeader.count; ++i)
    {
      LBXPaletteEntry paletteEntry;
      fread(&paletteEntry, sizeof(LBXPaletteEntry), 1, in);
      
      palette[paletteHeader.firstIndex + i] = (0xFF << 24) | (paletteEntry.r << 18) | (paletteEntry.g << 10) | (paletteEntry.b << 2);
    }
  }
  
  for (int i = 0; i < gfxHeader.count; ++i)
  {
    u32 dataSize = frameOffsets[i+1] - frameOffsets[i];
    
    fseek(in, offset+frameOffsets[i], SEEK_SET);
    u8* data = new u8[dataSize];
    fread(data, sizeof(u8), dataSize, in);
    LOGD("[lbx]     frame %d at offset %d (size %d bytes)", i, frameOffsets[i], dataSize);
    
    // copy previous frame into current
    if (i > 0)
      std::copy(sprite->data[i-1], sprite->data[i-1]+(sprite->width*sprite->height), sprite->data[i]);
    
    scanGfxFrame(gfxHeader, paletteHeader, i, sprite->data[i], data, dataSize);
    
    delete [] data;
  }

  delete [] frameOffsets;
  
  return sprite;
}

void LBX::scanFileNames(const FileInfo& info, string_list& names, FILE *in)
{
  const LBXHeader& header = info.header;
  const offset_list& offsets = info.offsets;
  
  fseek(in, offsets[0] - sizeof(LBXFileName)*header.count, SEEK_SET);
  
  names.resize(header.count);
  fread(&names[0], sizeof(LBXFileName), header.count, in);
  
  //int i = 25;
  /*for (int i = 0; i < header.count; ++i)
  {
    printf("[lbx]   > (%d) %s/%s (%u %08X)\n", i, names[i].folder, names[i].name, offsets[i], offsets[i]);
  }*/
}

LBXSpriteData* LBX::scanTerrainGfx(LBXOffset offset, size_t count, FILE* in)
{
  constexpr size_t width = 20;
  constexpr size_t height = 18;

  LBXSpriteData* sprite = new LBXSpriteData(new SharedPalette(Gfx::mainPalette), count, 0, 20, 18, false);
  
  u8* buffer = new u8[width*height];

  for (size_t i = 0; i < count; ++i)
  {
    fseek(in, offset + TERRAIN_ENTRY_HEADER + i*TERRAIN_ENTRY_LENGTH, SEEK_SET);
    fread(buffer, 1, width*height, in);

    /* we need to transpose as graphics in game is stored by column */
    for (size_t x = 0; x < 20; ++x)
      for (size_t y = 0; y < 18; ++y)
        sprite->data[i][x+width*y] = buffer[y+height*x];
  }
  
  delete [] buffer;
  return sprite;
}

std::vector<LBXTerrainSpriteSpecs> LBX::scanTerrainTileInfo(FILE* in)
{
  constexpr size_t offset = 676992, offset2 = 680040;
  constexpr size_t length = 3048;
  constexpr size_t count = length / sizeof(u16);
  
  static_assert(count == TERRAIN_COUNT_PER_PLANE*2, "");
  
  std::vector<LBXTerrainSpriteSpecs> data;
  data.resize(count);
  
  fseek(in, offset, SEEK_SET);
  for (size_t i = 0; i < count; ++i)
  {
    fread(&data[i].data, sizeof(u8), 2, in);
  }
  
  fseek(in, offset2, SEEK_SET);
  for (size_t i = 0; i < count; ++i)
  {
    fread(&data[i].minimapColor, sizeof(u8), 1, in);
    //printf("index in array: %04x %04zu, values: %02x %02x, index in terrain: %d %04x, animated: %c, data: %02x\n", (u32)i, i, data[i].data[0], data[i].data[1], data[i].index(), data[i].index(), data[i].animated() ? 'y' : 'n', data[i].data2);
  }
  
  return data;
}

bool LBX::loadHeader(FileInfo& info, FILE *in)
{
  fread(&info.header, sizeof(LBXHeader), 1, in);
  
  if (info.header.magic == 0x0000FEAD)
  {
    info.offsets.resize(info.header.count+1);
    fread(&info.offsets[0], sizeof(LBXOffset), info.header.count+1, in);
    
    return true;
  }
  
  return false;
}

void LBX::loadFonts(const LBXHeader& header, vector<LBXOffset>& offsets, FILE *in)
{
  static const u16 FONT_NUM = 8;
  static const u16 FONT_CHAR_NUM = 94;
  
  static const LBXOffset FONT_HEIGHTS_OFFSET = 0x16A;
  static const LBXOffset FONT_WIDTH_OFFSET = 0x19A;
  static const LBXOffset FONT_DATA_OFFSET = 0x49A;
  
  LOGD("[lbx] loading %u fonts from fonts.lbx", FONT_NUM)
  
  // position to start of character heights resource
  fseek(in, offsets[0] + FONT_HEIGHTS_OFFSET, SEEK_SET);
  
  u16 *heights = new u16[8];
  fread(heights, FONT_NUM, sizeof(u16), in);
  
  // position to start of character widths
  fseek(in, offsets[0] + FONT_WIDTH_OFFSET, SEEK_SET);
  
  u8 **widths = new u8*[FONT_NUM];
  
  // read widths
  for (int i = 0; i < FONT_NUM; ++i)
  {
    // read 94 bytes for widths of current font
    widths[i] = new u8[FONT_CHAR_NUM];
    fread(widths[i], 1, FONT_CHAR_NUM, in);
    
    // skip 2 control characters
    u8 padding[2];
    fread(padding, 1, 2, in);
    assert(padding[0] == 0 && padding[1] == 0);
  }
  
  // position to start of character offsets
  fseek(in, offsets[0] + FONT_DATA_OFFSET, SEEK_SET);
  
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


    u8 width = 0;
    for (int j = 0; j < FONT_CHAR_NUM; ++j) width = std::max(widths[i][j], width);
    width += 2;
    
#if DEBUG
    u8 maxColor = 0;
#endif
    
    // allocate storage for all glyphs
    FontData::fonts[i] = new FontData(static_cast<FontType>(i), heights[i]+2, width);  // add 2 pixels by side for precomputed stroke
    u16 totalWidth = FontData::fonts[i]->tw();
    
    for (int j = 0; j < FONT_CHAR_NUM; ++j)
    {
      u8* glyph = FontData::fonts[i]->dataAt(0, j);
      
      fseek(in, offsets[0] + foffsets[i][j], SEEK_SET);
      
      u16 bx = 1;
      u16 by = 1;
      u16 x = 0, y = 0;
      u8 data;
      
      while (x < widths[i][j])
      {
        fread(&data, 1, 1, in);
        
        u8 high = (data >> 4) & 0x0F;
        u8 low = data & 0x0F;
        
        u8 color;
        u8 strain = 0;
        
        /* if data is 0x8N then N is strain length, if N is 0 then we just skip the column,
         else we have a strain of N transparent pixels */
        if (high == 0x08)
        {
          color = 0;
          strain = low;
        }
        /* otherwise for data 0xMN then N is the color and M is the strain length,
         as before is N is 0 we skip the column, otherwise we have a strain of N pixels of color M */
        else
        {
          color = low+3;
#if DEBUG
          maxColor = std::max(low, maxColor);
#endif
          strain = high;
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
              glyph[bx+x + (by+y)*totalWidth] = color;
            ++y;
          }
        }
      }
      
      static const s8 dirs[8][2] = {{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1}};
      
      /*TODO: missing single pixel strokes (eg. top right corners of medium bold which are now mapped to light stroke */
      
      // precompute font stroke
      for (int x = 0; x < width; ++x)
        for (int y = 0; y < heights[i]+2; ++y)
        {
          bool hasLightStroke = false, hasDarkStroke = false;
          
          for (int d = 0; d < 8; ++d)
          {
            int dx = x + dirs[d][0];
            int dy = y + dirs[d][1];
            
            if (dx >= 0 && dy >= 0 && dx < width && dy < heights[i]+2 && glyph[x + y*totalWidth] == 0)
            {
              u8 pixel = glyph[dx + dy*totalWidth];
              
              // if neighbour pixel is a real font pixel
              if (pixel > FontData::DARK_STROKE_VALUE)
              {
                hasLightStroke |= (d == 1 || d == 2 || ((d == 3 || d == 5) && (pixel > FontData::DARK_STROKE_VALUE+1)) || d == 4 || d == 6);
                hasDarkStroke |= (d == 0 || ((d == 7 || d == 6) && (pixel > FontData::DARK_STROKE_VALUE+1)) );
              }
            }
          }
          
          if (hasDarkStroke)
            glyph[x + y*totalWidth] = FontData::DARK_STROKE_VALUE;
          else if (hasLightStroke)
            glyph[x + y*totalWidth] = FontData::LIGHT_STROKE_VALUE;
          
        }
      
      FontData::fonts[i]->setGlyphWidth(j, widths[i][j]);
      
    }
    
    LOGD("[lbx] loading font %u, size: %ux%u, colors: %u", i, width-2, heights[i], maxColor)
  }
  
  for_each(widths, widths+FONT_NUM, [](u8* ww) { delete [] ww; });
  for_each(foffsets, foffsets+FONT_NUM, [](u16* ww) { delete [] ww; });

  
  delete [] foffsets;
  delete [] heights;
  delete [] widths;
}

void LBX::loadPalette(LBXOffset offset, IndexedPalette* palette, FILE* in)
{
  fseek(in, offset, SEEK_SET);
  LBXPaletteEntry* lpalette = new LBXPaletteEntry[256];
  fread(lpalette, sizeof(LBXPaletteEntry), 256, in);
  
  for (size_t i = 0; i < 256; ++i)
    palette->set(i, Color(lpalette[i].r << 2, lpalette[i].g << 2, lpalette[i].b << 2));
}

void LBX::loadPalettes(const LBXHeader &header, offset_list &offsets, FILE *in)
{
  /* palettes reside in FONTS.LBX */
  /* entry at index 2: main game palette */
  /*                3: load/save palette */
    
  loadPalette(offsets[2], Gfx::mainPalette, in);
  
  Gfx::mainPalette->set(232, BLACK_ALPHA);
  Gfx::mainPalette->set(233, BLACK_ALPHA);
  Gfx::mainPalette->set(0, TRANSPARENT);
  
  loadPalette(offsets[2], Gfx::mainPaletteOpaque, in);

  
  //loadPalette(offsets[3], Gfx::loadPalette, in);
}

std::string LBX::getLBXPath(const std::string& name)
{
  return Platform::instance()->getResourcePath() + "/data/lbx/" + name + ".lbx";
}

FILE* LBX::getDescriptor(const LBXFile& lbx)
{
  string name = getLBXPath(lbx.fileName);
  return fopen(name.c_str(), "rb");
}

void LBX::load()
{
  {
    /*FileInfo info;
    FILE *in = fopen(getLBXPath("fonts").c_str(), "rb");
    loadHeader(info, in);
    loadFonts(info.header, info.offsets, in);*/
  }
  {
    FileInfo info;
    FILE *in = fopen(getLBXPath("buildesc").c_str(), "rb");
    loadHeader(info, in);
    
    vector<TextFiller> inserters = {
      TextFiller(0, [](u16 index, std::string& str) {
        static const I18 buildings[] = {
          I18::BUILDING_DESC_TRADE_GOODS,
          I18::BUILDING_DESC_HOUSING,
          I18::BUILDING_DESC_BARRACKS,
          I18::BUILDING_DESC_ARMORY,
          I18::BUILDING_DESC_FIGHTERS_GUILD,
          I18::BUILDING_DESC_ARMORERS_GUILD,
          I18::BUILDING_DESC_WAR_COLLEGE,
          I18::BUILDING_DESC_SMITHY,
          I18::BUILDING_DESC_STABLE,
          I18::BUILDING_DESC_ANIMISTS_GUILD,
          I18::BUILDING_DESC_FANTASTIC_STABLE,
          I18::BUILDING_DESC_SHIP_WRIGHTS_GUILD,
          I18::BUILDING_DESC_SHIP_YARD,
          I18::BUILDING_DESC_MARITIME_GUILD,
          I18::BUILDING_DESC_SAWMILL,
          I18::BUILDING_DESC_LIBRARY,
          I18::BUILDING_DESC_SAGES_GUILD,
          I18::BUILDING_DESC_ORACLE,
          I18::BUILDING_DESC_ALCHEMISTS_GUILD,
          I18::BUILDING_DESC_UNIVERSITY,
          I18::BUILDING_DESC_WIZARDS_GUILD,
          I18::BUILDING_DESC_SHRINE,
          I18::BUILDING_DESC_TEMPLE,
          I18::BUILDING_DESC_PARTHENON,
          I18::BUILDING_DESC_CATHEDRAL,
          I18::BUILDING_DESC_MARKETPLACE,
          I18::BUILDING_DESC_BANK,
          I18::BUILDING_DESC_MERCHANTS_GUILD,
          I18::BUILDING_DESC_GRANARY,
          I18::BUILDING_DESC_FARMERS_MARKET,
          I18::BUILDING_DESC_FORESTERS_GUILD,
          I18::BUILDING_DESC_BUILDERS_HALL,
          I18::BUILDING_DESC_MECHANICIANS_GUILD,
          I18::BUILDING_DESC_MINERS_GUILD,
          I18::BUILDING_DESC_CITY_WALLS
        };
        
        if (index > 0)
          i18n::data[buildings[index-1]] = str;
      })
    };
    
    loadArrayFile(info, inserters, in);
    
  }
}

/*
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
 
 
 }*/

size_t Repository::bytesUsed;
size_t Repository::spritesUsed;
LBXFile Repository::data[LBX_COUNT];
std::vector<LBXTerrainSpriteSpecs> Repository::terrainData;

LBXFile& Repository::file(const std::string &name)
{
  auto it = std::find_if(data, data+LBX_COUNT, [&name](const LBXFile& file) { return strings::tolower(file.fileName) == name; });
  
  assert(it < data+LBX_COUNT);
  
  return *it;
}

const LBXFile& Repository::loadLBX(LBXID ident)
{
  if (ident == LBXID::TERRAIN)
    return loadLBXTerrain();
  else if (ident == LBXID::TERRTYPE)
    return loadLBXTerrainMap();
  else if (ident == LBXID::HELP)
    return loadLBXHelp();
  else if (ident == LBXID::FONTS)
    return loadLBXFontsAndPalettes();
  
  
  LBXFile& lbx = file(ident);
  
  FILE *in = LBX::getDescriptor(lbx);
  LBX::loadHeader(lbx.info, in);
  
  if (ident == LBXID::SOUNDFX || ident == LBXID::SNDDRV || ident == LBXID::NEWSOUND || ident == LBXID::CMBTSND || ident == LBXID::INTROSFX || ident == LBXID::INTROSND)
    lbx.info.header.type = LBXFileType::SOUND;
  else if (ident == LBXID::MUSIC)
    lbx.info.header.type = LBXFileType::MUSIC;
  
  LOGD("[lbx] caching %zu assets %s", lbx.info.offsets.size(), lbx.fileName.c_str()/*, LBX::getLBXPath(lbx.fileName).c_str()*/);

  if (lbx.info.header.type == LBXFileType::GRAPHICS)
  {
    lbx.sprites = new LBXSpriteData*[lbx.size()];
    std::fill(lbx.sprites, lbx.sprites+lbx.size(), nullptr);
  }
  else if (lbx.info.header.type == LBXFileType::DATA_ARRAY)
  {
    lbx.arrays = new LBXArrayData*[lbx.size()];
    std::fill(lbx.arrays, lbx.arrays+lbx.size(), nullptr);
  }
  
  
  fclose(in);
  
  return lbx;
}

const LBXSpriteData* Repository::loadLBXSpriteData(SpriteInfo info)
{
  LBXFile& lbx = file(info.lbx());
  
  string name = LBX::getLBXPath(lbx.fileName);
  FILE *in = fopen(name.c_str(), "rb");
  
  LOGD("[lbx] loading gfx entry %u from %s", info.index(), file(info.lbx()).fileName.c_str());
  LBXSpriteData* spriteData = LBX::scanGfx(lbx.info.header, lbx.info.offsets[info.index()], in);
  gfxAllocated(spriteData);
  LOGD3("[lbx] lbx cache size: %zu sprites in %zu bytes", spritesUsed, bytesUsed);
  
  lbx.sprites[info.index()] = spriteData;
  
  fclose(in);
  
  return spriteData;
}

const LBXArrayData* Repository::loadLBXArrayData(const LBXFile& lbx, size_t index)
{
  string name = LBX::getLBXPath(lbx.fileName);
  FILE *in = fopen(name.c_str(), "rb");

  LBXArrayData* arrayData = LBX::loadArray(lbx.info.offsets[index], in);
  lbx.arrays[index] = arrayData;
  
  fclose(in);
  
  return arrayData;
}

const LBXSpriteData* Repository::loadLBXSpriteTerrainData(SpriteInfo info)
{
  LBXFile& lbx = file(info.lbx());
  FILE *in = LBX::getDescriptor(lbx);

  LOGD("[lbx] loading gfx entry %u from %s", info.index(), file(info.lbx()).fileName.c_str());
  LBXSpriteData* sprite = LBX::scanTerrainGfx(lbx.info.offsets[info.index()], terrainData[info.index()].animated() ? 4 : 1, in);
  lbx.sprites[info.index()] = sprite;
  gfxAllocated(sprite);
  
  fclose(in);
  
  return sprite;
}

LBXArrayData* Repository::loadLBXSpriteTerrainMappingData(LBXFile& lbx, size_t i, FILE* in)
{
  LOGD("[lbx] loading terrain mapping %zu from %s", i, lbx.fileName.c_str());
  
  LBXArrayData* array = new LBXArrayData(1, 512);

  fseek(in, lbx.info.offsets[i] , SEEK_SET);
  
  u16 v = 0;
  u16 c = 0;
  
  while (c < 255)
  {
    fread(&v, sizeof(u16), 1, in);
    
    /* since shores are stored as a bitmask which starts from north east while implementation assumes that first direction is north
       we need to adjust values
     */
    u16 ai = ((c+1) >> 1) | (((c+1) & 0x01) ? 0x80 : 0x00);
    
    array->set(ai, v);
    ++c;
  }
    
  return array;
}

const LBXFile& Repository::loadLBXFontsAndPalettes()
{
  LBXFile& lbx = file(LBXID::FONTS);
  FILE* in = LBX::getDescriptor(lbx);
  
  LBX::loadHeader(lbx.info, in);
  LBX::loadFonts(lbx.info.header, lbx.info.offsets, in);
  LBX::loadPalettes(lbx.info.header, lbx.info.offsets, in);
  
  /*string_list names;
  LBX::scanFileNames(lbx.info, names, in);
  for (size_t i = 0; i < lbx.size(); ++i)
  {
    size_t length = lbx.info.offsets[i+1] - lbx.info.offsets[i];
    fseek(in, lbx.info.offsets[i], SEEK_SET);
    
    std::string outName = string(names[i].folder) + "-" + names[i].name;
    std::replace(outName.begin(), outName.end(), '/', '-');
    
    FILE* out = fopen(outName.c_str(), "wb");
    
    byte* buffer = new byte[length];
    fread(buffer, 1, length, in);
    fwrite(buffer, 1, length, out);
    fclose(out);
  }*/
  
  
  fclose(in);
  
  
  
  return lbx;
}

const LBXFile& Repository::loadLBXTerrain()
{
  LBXFile& lbx = file(LBXID::TERRAIN);
  
  FILE *in = LBX::getDescriptor(lbx);
  fread(&lbx.info.header, sizeof(LBXHeader), 1, in);
  
  terrainData = LBX::scanTerrainTileInfo(in);
  
  fclose(in);
  
  lbx.info.header.type = LBXFileType::TILES;
  lbx.info.header.count = terrainData.size();
  
  lbx.info.offsets.reserve(terrainData.size());
  for (u32 i = 0; i < terrainData.size(); ++i)
    lbx.info.offsets.push_back(TERRAIN_BASE + TERRAIN_ENTRY_LENGTH*terrainData[i].index());
  
  LOGD("[lbx] caching %zu assets %s", lbx.info.offsets.size(), lbx.fileName.c_str()/*, LBX::getLBXPath(lbx.fileName).c_str()*/);
  
  lbx.sprites = new LBXSpriteData*[lbx.size()];
  std::fill(lbx.sprites, lbx.sprites+lbx.size(), nullptr);

  return lbx;
}

const LBXFile& Repository::loadLBXTerrainMap()
{
  LBXFile& lbx = file(LBXID::TERRTYPE);
  
  constexpr size_t offset[] = { 548+2, 1062 };
  
  FILE *in = LBX::getDescriptor(lbx);
  
  fread(&lbx.info.header, sizeof(LBXHeader), 1, in);
  lbx.info.header.count = 2;
  lbx.info.header.type = LBXFileType::TILES_MAPPING;
  
  lbx.info.offsets.push_back(offset[0]);
  lbx.info.offsets.push_back(offset[1]);

  
  lbx.arrays = new LBXArrayData*[2];
  lbx.arrays[0] = loadLBXSpriteTerrainMappingData(lbx, 0, in);
  lbx.arrays[1] = loadLBXSpriteTerrainMappingData(lbx, 1, in);
  

  
  fclose(in);

  return lbx;
}

const LBXFile& Repository::loadLBXHelp()
{
  static_assert(sizeof(LBXHelpEntry) == 1048, "");
  constexpr size_t offset = 37069 + 4;
  constexpr size_t count = 806;
  
  LBXFile& lbx = file(LBXID::HELP);
  
  FILE* in = LBX::getDescriptor(lbx);
  
  fread(&lbx.info.header, sizeof(LBXHeader), 1, in);
  lbx.info.offsets.resize(lbx.size());
  fread(&lbx.info.offsets[0], sizeof(LBXOffset), 3, in);
  
  lbx.info.header.count = 2;
  lbx.info.offsets.pop_back();
  lbx.info.header.type = LBXFileType::GRAPHICS;
  lbx.sprites = new LBXSpriteData*[lbx.size()];
  std::fill(lbx.sprites, lbx.sprites+lbx.size(), nullptr);

  fseek(in, offset, SEEK_SET);
  
  size_t i = 0;
  
  auto& dest = help::Data::data;
  
  dest.resize(count);
  
  for (i = 0; i < 806; ++i)
  {
    size_t off = ftell(in);
    
    LBXHelpEntry entry;
    fread(&entry, sizeof(LBXHelpEntry), 1, in);
    
    for (size_t j = 0; entry.lbxName[j] != '\0'; ++j)
      if (entry.lbxName[j] == '.')
      {
        entry.lbxName[j] = '\0';
        break;
      }
      else entry.lbxName[j] = std::tolower(entry.lbxName[j]);
    
    for (size_t j = 0; entry.text[j] != '\0'; ++j)
      if (entry.text[j] == 0x14)
        entry.text[j] = '\n';
    
    auto& current = dest[i];
    
    if (!entry.hasGfx())
    {
      current = help::Paragraph(entry.title, entry.text);
    }
    else
    {
      current = help::Paragraph(entry.title, entry.text, SpriteInfo(file(entry.lbxName).ident, entry.lbxIndex));
    }
    
    if (entry.type == -1)
      current.next = &dest[i+1];
    else if (entry.type != 0)
      current.next = &dest[entry.type];
    else
      current.next = nullptr;
    
    //printf("%3zu %s %c\n%s\n\n", i, current.title.c_str(), current.next ? 'y' : 'n', current.text.c_str());

    
    /*printf("%3zu %08zu: %s", i, off, entry.title);
    
    if (entry.hasGfx())
      printf(" %s(%u)", entry.lbxName, entry.lbxIndex);
    
    printf("   %d", entry.type);
    printf(" %s", entry.text);
    
    printf("\n");*/
  }

  LOGD("[lbx] read %zu help entries", i);
  
  fclose(in);
  
  return lbx;
}

void Repository::gfxAllocated(const LBXSpriteData* data)
{
  bytesUsed += data->memoryUsedInBytes();
  ++spritesUsed;
}

void Repository::gfxDeallocated(const LBXSpriteData* data)
{
  bytesUsed -= data->memoryUsedInBytes();
  --spritesUsed;
}

void Repository::init()
{
  bytesUsed = 0;
  spritesUsed = 0;
  
  static const char* names[] = {
    "armylist",
    "backgrnd",
    "book",
    "builddat",
    "buildesc",
    "chriver",
    "cityname",
    "cityscap",
    "citywall",
    "cmbdesrc",
    "cmbdesrt",
    "cmbgrasc",
    "cmbgrass",
    "cmbmagic",
    "cmbmounc",
    "cmbmount",
    "cmbtcity",
    "cmbtfx",
    "cmbtsnd",
    "cmbtundc",
    "cmbtundr",
    "cmbtwall",
    "combat",
    "compix",
    "conquest",
    "desc",
    "desert",
    "diplomac",
    "diplomsg",
    "eventmsg",
    "events",
    "figures1",
    "figures2",
    "figures3",
    "figures4",
    "figures5",
    "figures6",
    "figures7",
    "figures8",
    "figures9",
    "figure10",
    "figure11",
    "figure12",
    "figure13",
    "figure14",
    "figure15",
    "figure16",
    "fonts",
    "halofam",
    "help",
    "herodata",
    "hire",
    "hlpentry",
    "intro",
    "introsfx",
    "introsnd",
    "itemdata",
    "itemisc",
    "itempow",
    "items",
    "lilwiz",
    "listdat",
    "load",
    "lose",
    "magic",
    "main",
    "mainscrn",
    "mapback",
    "message",
    "monster",
    "moodwiz",
    "music",
    "names",
    "newgame",
    "newsound",
    "portrait",
    "reload",
    "resource",
    "scroll",
    "snddrv",
    "soundfx",
    "specfx",
    "special",
    "special2",
    "spelldat",
    "spellose",
    "spells",
    "spellscr",
    "splmastr",
    "terrain",
    "terrstat",
    "terrtype",
    "tundra",
    "units1",
    "units2",
    "unitview",
    "vortex",
    "wallrise",
    "win",
    "wizards",
    "wizlab"
  };
  
  for (int i = 0; i < LBX_COUNT; ++i)
    data[i] = LBXFile(static_cast<LBXID>(i), names[i]);
  
  loadLBXHelp();
}
