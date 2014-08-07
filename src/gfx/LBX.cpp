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

#include "Font.h"
#include "Common.h"
#include "Gfx.h"

#include <iostream>

#include <cstdio>
#include <string>
#include <dirent.h>
#include <vector>

#include <SDL2/SDL.h>

using namespace std;






const Color BLACK_ALPHA = RGB(0, 255, 0);
const Color TRANSPARENT = RGB(255, 0, 255);

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






void LBX::loadArray(LBXOffset offset, LBXArray& info, const TextFiller& inserter, FILE *in)
{
  fseek(in, offset, SEEK_SET);
  
  char *buffer = new char[info.size];
  
  for (int i = 0; i < info.count; ++i)
  {
    fread(buffer, 1, info.size, in);
    //printf("  > %s\n",buffer);
    inserter.push(string(buffer));
  }
}


void LBX::loadArrayFile(LBXHeader& header, offset_list& offsets, std::vector<TextFiller>& inserters, FILE *in)
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
    
    /*printf("Array at 0x%08X, %4d elements, %4d size, ends at 0x%08X (next at: 0x%08X)\n",
           offsets[i],arrays[i].count,arrays[i].size,startArrayOffset,nextEntryOffset);*/
    
    if (startArrayOffset != nextEntryOffset) cout << "ERROR! OFFSET DIFFERS!" << endl;
    else
    {
      loadArray(offsets[i]+sizeof(LBXArray), arrays[i], inserters[i], in);
    }
  }
}



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
  const u16 PALETTE_SIZE = std::extent<decltype(Gfx::PALETTE)>::value;
  Color* palette = new Color[PALETTE_SIZE];
  memcpy(palette, Gfx::PALETTE, sizeof(Color)*PALETTE_SIZE);

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
  SDL_LockSurface(image);
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
    
    SDL_UnlockSurface(image);
    SDL_SaveBMP(image, (to_string(offset)+" "+to_string(i)+".bmp").c_str());
    
    
    delete [] data;
  }
  
  SDL_FreeSurface(image);
  
  delete [] palette;
}

void LBX::scanFileNames(LBXHeader& header, offset_list& offsets, string_list& names, FILE *in)
{
  fseek(in, offsets[0] - sizeof(LBXFileName)*header.count, SEEK_SET);
  
  names.resize(header.count);
  fread(&names[0], sizeof(LBXFileName), header.count, in);
  
  //int i = 25;
  for (int i = 0; i < header.count; ++i)
  {
    printf("> (%d) %s/%s (%u %08X)\n", i, names[i].folder, names[i].name, offsets[i], offsets[i]);
    //scanGfx(header, offsets[i], in);
  }
}

static std::string path = string(getenv("PWD")) + "/OpenMoM.app/Contents/Resources/data/lbx/";

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
    u8 width = 0;
    for (int j = 0; j < FONT_CHAR_NUM; ++j) width = std::max(widths[i][j], width);
    width += 2;
    
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
        
        if (high == 0x08)
        {
          color = 0;
          strain = low;
        }
        else
        {
          color = low+3;
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
  }
}

void LBX::load()
{
  {
    LBXHeader header;
    offset_list offsets;
    FILE *in = fopen((path+"fonts.lbx").c_str(), "rb");
    loadHeader(header, offsets, in);
    loadFonts(header, offsets, in);
  }
  {
    LBXHeader header;
    offset_list offsets;
    FILE *in = fopen((path+"buildesc.lbx").c_str(), "rb");
    loadHeader(header, offsets, in);
    
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
    
    loadArrayFile(header, offsets, inserters, in);

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

#include "ViewManager.h"
#include "Buttons.h"

const u16 MAX_PER_PAGE = 23;

static const vector<string> files = {
  "armylist",
  "backgrnd"
};

static offset_list offsets;

LBXView::LBXView(ViewManager* gvm) : View(gvm), selectedLBX(files.end()), lbxOffset(0), contentOffset(0)
{
  buttons.resize(4);
  buttons[0] = TristateButton::build("prev lbx", 5, 5+23*8, TextureID::ARMIES_ARROWS, 0)->setAction([this](){  });
  buttons[1] = TristateButton::build("next lbx", 15, 5+23*8, TextureID::ARMIES_ARROWS, 1)->setAction([this](){  });

  buttons[2] = TristateButton::build("prev content", 50, 5+23*8, TextureID::ARMIES_ARROWS, 0)->setAction([this](){ --contentOffset; updateContentButtons(); });
  buttons[3] = TristateButton::build("next nextcontent", 60, 5+23*8, TextureID::ARMIES_ARROWS, 1)->setAction([this](){ ++contentOffset; updateContentButtons(); });
  
  for (auto &b : buttons) b->hide();
  
  hasNextContent = false;
  hasPrevContent = false;
  hasNextFile = files.size() > MAX_PER_PAGE;
  hasPrevFile = 0;
  
  for (auto it = files.begin(); it != files.end(); ++it)
  {
    string name = path + *it + ".lbx";
    
    LBXHeader header;
    offsets.clear();
    string_list names;
    
    FILE *in = fopen(name.c_str(), "rb");
    
    LBX::loadHeader(header, offsets, in);
    
    LBX::scanFileNames(header, offsets, names, in);
    filesForLBX[it] = names;
    
    fclose(in);
  }
}

void LBXView::draw()
{
  for (int i = 0; i < files.size(); ++i)
    Fonts::drawString(files[i], selectedLBX == (files.begin()+i) ? FontFaces::Small::REDW : FontFaces::Small::WHITE, 5, 5+i*8, ALIGN_LEFT);
  
  map<file_content_iterator, string_list>::iterator it = filesForLBX.find(selectedLBX);
  
  if (it != filesForLBX.end())
  {
    for (int i = 0; i < MAX_PER_PAGE; ++i)
    {
      if (i + contentOffset < it->second.size())
      {
        auto &el = it->second[i+contentOffset];
        Fonts::drawString(string(el.folder)+"/"+el.name, FontFaces::Small::WHITE, 50, 5+i*8, ALIGN_LEFT);
      }
    }
  }
  /*
  if (hasPrevContent)
  if (hasNextContent)
    Gfx::draw(TextureID::ARMIES_ARROWS, 0, 1, 50, 5+23*8);*/

}

void LBXView::mouseReleased(u16 x, u16 y, MouseButton b)
{
  if (x >= 0 && y > 5 && y < HEIGHT)
  {
    x -= 5;
    y -= 5;
    
    y /= 8;
    
    if (y < files.size() && x < Fonts::stringWidth(FontFaces::Small::WHITE, files[y]) + 10)
    {
      selectedLBX = files.begin()+y;
      selectLBX();
    }
  }
}

void LBXView::updateContentButtons()
{
  buttons[3]->showIf(contentOffset + MAX_PER_PAGE < filesForLBX[selectedLBX].size());
  buttons[2]->showIf(contentOffset > 0);
}

void LBXView::selectLBX()
{
  updateContentButtons();
}

