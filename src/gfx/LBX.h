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

#include <string>
#include <vector>
#include <functional>

namespace lbx
{
  enum LBXFileType : u16
  {
    GRAPHICS = 0,
    DATA_ARRAY = 5
  };
  
  struct LBXHeader
  {
    u16 count;
    u32 magic;
    LBXFileType type;
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

  using LBXOffset = u32;
  using offset_list = std::vector<LBXOffset>;
  using string_list = std::vector<LBXFileName>;
  
  struct FileInfo
  {
    LBXHeader header;
    offset_list offsets;
  };

  class SDL_Surface;

  class LBXSpriteData : public SpriteSheet
  {
  private:


  public:
    LBXSpriteData(const Palette* palette, u16 count, u16 width, u16 height, bool hasCustomPalette) : palette(palette), count(count), width(width), height(height), data(new u8*[count]), hasCustomPalette(hasCustomPalette)
    {
      for (u16 i = 0; i < count; ++i)
      {
        data[i] = new u8[width*height];
        memset(data[i], 0, sizeof(u8)*width*height);
      }
    }
    
    ~LBXSpriteData()
    {
      delete [] palette;
      std::for_each(data, data+count, [](const u8* sprite) { delete sprite; });
      delete [] data;
    }

    u8 **data;
    const bool hasCustomPalette;
    const Palette* palette;
    const u16 count;
    const u16 width;
    const u16 height;
    
    Color at(u16 x, u16 y, u16 c, u16 r) const override { return palette->get(data[c+r][x+y*width]); }

    u16 tw() const override { return count*width; }
    u16 th() const override { return height; }

    u16 sw(u16,u16) const override { return width; }
    u16 sh(u16,u16) const override { return height; }
    
    const Palette* getPalette() const override { return palette; }
    
    size_t memoryUsedInBytes() const { return count*width*height + sizeof(Color)*256; }
  };
  
  class LBXSpriteDataWithPalette : public SpriteSheet
  {
  private:
    const LBXSpriteData* const sprite;
    const Palette* const palette;
    
  public:
    LBXSpriteDataWithPalette(const LBXSpriteData* sprite, const Palette* palette) : sprite(sprite), palette(palette) { }
    
    Color at(u16 x, u16 y, u16 c, u16 r) const override { return palette->get(sprite->data[c+r][x+y*sprite->width]); }
    
    u16 tw() const override { return sprite->tw(); }
    u16 th() const override { return sprite->th(); }
    
    u16 sw(u16,u16) const override { return sprite->sw(0,0); }
    u16 sh(u16,u16) const override { return sprite->sh(0,0); }

    const Palette* getPalette() const override { return palette; }
  };
  
  struct LBXArrayData
  {
    u16 count;
    u16 size;
    u8** data;
    
    LBXArrayData(u16 count, u16 size) : count(count), size(size), data(new u8*[count])
    {
      for (size_t i = 0; i < count; ++i)
      {
        data[i] = new u8[size];
        std::fill(data[i], data[i]+size, 0);
      }
    }
    
    ~LBXArrayData()
    {
      std::for_each(data, data+count, [](u8* data) { delete [] data; });
      delete [] data;
    }
  };

  struct LBXFile
  {
    LBXID ident;
    std::string fileName;
    FileInfo info;
    
    union
    {
      LBXSpriteData** sprites;
      LBXArrayData** arrays;
    };
    
    bool isLoaded() const { return (info.header.type == LBXFileType::GRAPHICS && sprites) || (info.header.type == LBXFileType::DATA_ARRAY && arrays); }
    
    LBXFile() : sprites(nullptr) { }
    LBXFile(LBXID ident, const std::string& fileName) : ident(ident), fileName(fileName), sprites(nullptr) { }
    
    const u16 size() const { return info.header.count; }
  };

  class Repository
  {
  private:
    static LBXFile data[LBX_COUNT];
    static LBXFile& file(LBXID ident) { return data[static_cast<size_t>(ident)]; }
    
    static size_t bytesUsed;
    static void gfxAllocated(const LBXSpriteData* data);
    static void gfxDeallocated(const LBXSpriteData* data);
    
  public:
    static void init();
    
    static bool shouldAllocateLBX(LBXID ident) { return file(ident).sprites == nullptr; }
    
    static const LBXFile& loadLBX(LBXID ident);
    static const LBXFile& holderForID(LBXID ident) { return file(ident); }
    
    static bool shouldAllocateSprite(SpriteInfo info) { return file(info.lbx()).sprites[info.index()] == nullptr; }
    
    static const LBXSpriteData* loadLBXSpriteData(SpriteInfo info);
    static const LBXArrayData* loadLBXArrayData(const LBXFile& lbx, size_t index);
    
    static const LBXSpriteData* spriteFor(SpriteInfo info) {
      assert(static_cast<u32>(info.lbx()) < LBX_COUNT);
      
      const LBXSpriteData* sprite = file(info.lbx()).sprites[info.index()];
      
      if (!sprite)
        sprite = loadLBXSpriteData(info);
        
      return sprite;
    }
    
    friend class LBXView;
  };

  class LBX
  {
  public:
    static void scanGfxFrame(const LBXGfxHeader& header, const LBXPaletteHeader& pheader, u16 index, u8* image, u8* data, u32 dataLength);

    
    class TextFiller
    {
      private:
        std::function<void(u16,std::string&)> lambda;
        mutable u16 index;
      
        
      public:
        TextFiller(u16 index, std::function<void(u16,std::string&)> lambda) : index(index), lambda(lambda) { }
        void push(std::string str) const { lambda(index, str); ++index; }
    };
    
    static bool loadHeader(FileInfo& offsets, FILE *in);
    
    static void loadArray(LBXOffset offset, LBXArray& info, const TextFiller& inserter, FILE *in);
    static void loadArrayFile(const FileInfo& info, std::vector<TextFiller>& inserters, FILE *in);
    
    static LBXArrayData* loadArray(LBXOffset offset, FILE *in);
    
    
    static LBXSpriteData* scanGfx(const LBXHeader& header, LBXOffset offset, FILE *in);
    static void scanFileNames(const FileInfo& info, string_list& names, FILE *in);

    static void loadText(const LBXHeader& header, offset_list& offsets, FILE *in);
    static void loadFonts(const LBXHeader& header, offset_list& offsets, FILE *in);
      
    static FILE* getDescriptor(const LBXFile& ident);

    static std::string getLBXPath(const std::string& ident);
    
  public:
    static void load();
    
    friend class LBXView;
    friend class Repository;
  };
}

#endif
