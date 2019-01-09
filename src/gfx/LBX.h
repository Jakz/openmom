//
//  LBX.h
//  OpenMoM
//
//  Created by Jack on 7/30/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _LBX_H_
#define _LBX_H_

#include "common/Common.h"
#include "platform/Path.h"

#include "ColorMap.h"

#include <string>
#include <vector>
#include <functional>

namespace lbx
{
  constexpr size_t TERRAIN_COUNT_PER_PLANE = 0x2FA;
  constexpr size_t TERRAIN_BASE = 768;
  constexpr size_t TERRAIN_ENTRY_LENGTH = 384;
  constexpr size_t TERRAIN_ENTRY_HEADER = 16;
  
  enum LBXFileType : u16
  {
    GRAPHICS = 0,
    FONTS = 2,
    DATA_ARRAY = 5,
    TILES = 111,
    TILES_MAPPING = 112,
    SOUND = 113,
    MUSIC = 114,
    HELP
  };

  STRUCT_PACKING_PUSH
  
  struct LBXHeader
  {
    u16 count;
    u32 magic;
    LBXFileType type;
  } PACKED;

  struct LBXArray
  {
    u16 count;
    u16 size;
  } PACKED;

  struct LBXFileName
  {
    char folder[8];
    char padding;
    char name[22];
    char padding2;
  } PACKED;

  struct LBXGfxHeader
  {
    u16 width;
    u16 height;
    u16 alwaysZero1;
    u16 count;
    u16 frameLoopRestart;
    u16 alwaysZero2;
    u16 alwaysZero3;
    u16 paletteOffset;
    u8 unknown2;
    u8 unknown3;
  } PACKED;

  struct LBXPaletteHeader
  {
    u16 offset;
    u16 firstIndex;
    u16 count;
    u16 unknown;
    
  } PACKED;

  struct LBXPaletteEntry
  {
    u8 r;
    u8 g;
    u8 b;
  } PACKED;
  
  enum LBXHelpEntryType
  {
    LAST_ENTRY = 0,
    KEEP_TO_NEXT = -1
  };
  
  struct LBXHelpEntry
  {
    char title[30];
    char lbxName[14];
    u16 lbxIndex;
    s16 type;
    char text[1000];
    
    bool hasGfx() const { return lbxName[0] != '\0'; }
    
  } PACKED;

  STRUCT_PACKING_POP

  using LBXOffset = u32;
  using offset_list = std::vector<LBXOffset>;
  using string_list = std::vector<LBXFileName>;
  
  struct LBXTerrainSpriteSpecs
  {
    u8 data[2];
    u8 minimapColor;
    u16 index() const { return ((data[0]&0x7F)/3)*128 + data[1] - 2; }
    bool animated() const { return data[0] & 0x80; }
  };
  
  struct FileInfo
  {
    LBXHeader header;
    offset_list offsets;
  };

  class LBXSpriteData : public SpriteSheet
  {
  private:


  public:
    LBXSpriteData(const Palette* palette, u16 count, u16 frameLoopRestart, u16 width, u16 height, bool hasCustomPalette) : palette(palette), count(count), frameLoopRestart(frameLoopRestart), width(width), height(height), data(new u8*[count]), hasCustomPalette(hasCustomPalette)
    {
      for (u16 i = 0; i < count; ++i)
      {
        data[i] = new u8[width*height];
        memset(data[i], 0, sizeof(u8)*width*height);
      }
    }
    
    ~LBXSpriteData()
    {
      delete palette;
      std::for_each(data, data+count, [](const u8* sprite) { delete sprite; });
      delete [] data;
    }

    u8 **data;
    const bool hasCustomPalette;
    const Palette* palette;
    const u16 count;
    const u16 frameLoopRestart;
    const u16 width;
    const u16 height;
    
    u8 unknown2;
    u8 unknown3;
    
    u32 at(index_t x, index_t y, index_t c, index_t r) const override { return data[c+r][x+y*width]; }

    index_t tw() const override { return count*width; }
    index_t th() const override { return height; }

    index_t sw(index_t, index_t) const override { return width; }
    index_t sh(index_t, index_t) const override { return height; }
    
    const Palette* getPalette() const override { return palette; }
    
    size_t memoryUsedInBytes() const { return count*width*height + sizeof(Color)*256; }
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
    
    template<typename T> T get(size_t index) const { return reinterpret_cast<T*>(data[0])[index]; }
    template<typename T> void set(size_t index, T value) const { reinterpret_cast<T*>(data[0])[index] = value; }

    
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
    
    bool isLoaded() const { return ((info.header.type == LBXFileType::GRAPHICS || info.header.type == LBXFileType::TILES) && sprites) || ((info.header.type == LBXFileType::DATA_ARRAY || info.header.type == LBXFileType::TILES_MAPPING) && arrays) || info.header.type == LBXFileType::FONTS; }
    
    LBXFile() : sprites(nullptr) { }
    LBXFile(LBXID ident, const std::string& fileName) : ident(ident), fileName(fileName), sprites(nullptr) { }
    
    const u16 size() const { return info.header.count; }
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
    
    static bool loadHeader(FileInfo& offsets, const file_handle& in);
    
    static void loadArray(LBXOffset offset, LBXArray& info, const TextFiller& inserter, const file_handle& in);
    static void loadArrayFile(const FileInfo& info, std::vector<TextFiller>& inserters, const file_handle& in);
    
    static LBXArrayData* loadArray(LBXOffset offset, FILE *in);
    
    
    static LBXSpriteData* scanGfx(const LBXHeader& header, LBXOffset offset, FILE *in);
    static void scanFileNames(const FileInfo& info, string_list& names, FILE *in);
    static LBXSpriteData* scanTerrainGfx(LBXOffset offset, size_t count, FILE* in);
    static std::vector<LBXTerrainSpriteSpecs> scanTerrainTileInfo(FILE* in);

    static void loadText(const LBXHeader& header, offset_list& offsets, FILE *in);
    static void loadFonts(const LBXHeader& header, offset_list& offsets, FILE *in);
    static void loadPalette(LBXOffset offset, IndexedPalette* palette, FILE* in);
    static void loadPalettes(const LBXHeader& header, offset_list& offsets, FILE* in);
        
  public:
    friend class LBXView;
    friend class Repository;
  };
}

#endif
