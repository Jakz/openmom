//
//  LBX.h
//  OpenMoM
//
//  Created by Jack on 7/30/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _LBX_H_
#define _LBX_H_

#include "Common.h"

#include "ColorMap.h"
//#include "Gfx.h"

#include <string>
#include <vector>
#include <functional>

struct LBXHeader
{
  u16 count;
  u32 magic;
  u16 type;
} __attribute__((__packed__));

struct LBXArray
{
  u16 count;
  u16 size;
} __attribute__((__packed__));

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

typedef u32 LBXOffset;
typedef std::vector<LBXOffset> offset_list;
typedef std::vector<LBXFileName> string_list;

class SDL_Surface;

class LBXSpriteData : public SpriteSheet
{
private:


public:
  LBXSpriteData(const Palette* palette, u16 count, u16 width, u16 height) : palette(palette), count(count), width(width), height(height), data(new u8*[count])
  {
    for (u16 i = 0; i < count; ++i)
    {
      data[i] = new u8[width*height];
      memset(data[i], 0, sizeof(u8)*width*height);
    }
  }

  u8 **data;
  const Palette* palette;
  const u16 count;
  const u16 width;
  const u16 height;
  
  SDL_Surface *surface;
  
  Color at(u16 x, u16 y, u16 c, u16 r) const override { return palette->get(data[c+r][x+y*width]); }

  u16 tw() const override { return count*width; }
  u16 th() const override { return height; }

  u16 sw(u16,u16) const override { return width; }
  u16 sh(u16,u16) const override { return height; }
  
  const Palette* getPalette() const { return palette; }
};

struct LBXFile
{
  LBXFileID ident;
  std::string fileName;
  LBXHeader header;
  offset_list offsets;
  LBXSpriteData **sprites;
  
  LBXFile() : sprites(nullptr) { }
  LBXFile(LBXFileID ident, std::string fileName) : ident(ident), fileName(fileName), sprites(nullptr) { }
  
  const u16 size() const { return header.count; }
};

class LBXRepository
{
private:
  static LBXFile data[LBX_COUNT];
  
public:

  static void init()
  {    
    std::string names[] = {
    "armylist",
    "backgrnd",
    "units1",
    "units2",
    "mainscrn",
    "main",
    "specfx"
    };
    
    for (int i = 0; i < LBX_COUNT; ++i)
      data[i] = LBXFile(static_cast<LBXFileID>(i), names[i]);
  }
  
  static bool shouldAllocateLBX(LBXFileID ident) { return data[ident].sprites == nullptr; }
  static const LBXFile& loadLBX(LBXFileID ident);
  static const LBXFile& holderForID(LBXFileID ident) { return data[ident]; }
  
  static bool shouldAllocateSprite(LBXSpriteDataInfo& info) { return data[info.lbx].sprites[info.index] == nullptr; }
  static const LBXSpriteData* loadLBXSpriteData(const LBXSpriteDataInfo& info);
  
  static const LBXSpriteData* spriteFor(const LBXSpriteDataInfo& info) {
    const LBXSpriteData* sprite = data[info.lbx].sprites[info.index];
    
    if (!sprite)
      sprite = loadLBXSpriteData(info);
      
    return sprite;
  }
  
  friend class LBXView;
};

class LBX
{
private:
  static void scanGfxFrame(LBXGfxHeader& header, LBXPaletteHeader &pheader, u16 index, u8* image, u8* data, u32 dataLength);

  
  class TextFiller
  {
    private:
      std::function<void(u16,std::string&)> lambda;
      mutable u16 index;
    
      
    public:
      TextFiller(u16 index, std::function<void(u16,std::string&)> lambda) : index(index), lambda(lambda) { }
      void push(std::string str) const { lambda(index, str); ++index; }
  };
  
  static bool loadHeader(LBXHeader& header, offset_list& offsets, FILE *in);
  
  static void loadArray(LBXOffset offset, LBXArray& info, const TextFiller& inserter, FILE *in);
  static void loadArrayFile(LBXHeader& header, offset_list& offsets, std::vector<TextFiller>& inserters, FILE *in);
  
  static LBXSpriteData* scanGfx(LBXHeader& header, LBXOffset offset, FILE *in);
  static void scanFileNames(LBXHeader& header, offset_list& offsets, string_list& names, FILE *in);

  static void loadText(LBXHeader& header, offset_list& offsets, FILE *in);
  static void loadFonts(LBXHeader& header, offset_list& offsets, FILE *in);
  
public:
  static void load();
  
  friend class LBXView;
  friend class LBXRepository;
};

#include "View.h"

#include <map>

class ViewManager;


class LBXView : public View
{
private:
  s16 selectedLBX;
  s16 selectedContent;
  
  s16 lbxOffset;
  s16 contentOffset;
  
  bool hasNextFile, hasPrevFile;
  bool hasNextContent, hasPrevContent;
    
  std::map<LBXFileID, string_list> filesForLBX;
  
  void updateContentButtons();
  
  void draw() override;
  void drawPost() override { }
  
public:
  LBXView(ViewManager* gvm);
  
  void activate() override { }
  void deactivate() override { }
  
  void mouseReleased(u16 x, u16 y, MouseButton b) override;
  void selectLBX();
  void selectGFX();
};

#endif
