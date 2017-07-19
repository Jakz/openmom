#pragma once

#include "LBX.h"

namespace lbx
{
  class Repository
  {
  private:
    static Palette* _mainPalette;
    
    static std::vector<LBXTerrainSpriteSpecs> terrainData;
    static LBXFile data[LBX_COUNT];
    static LBXFile& file(LBXID ident) { return data[static_cast<size_t>(ident)]; }
    static LBXFile& file(const std::string& name);
    
    static size_t bytesUsed;
    static size_t spritesUsed;
    static void gfxAllocated(const LBXSpriteData* data);
    static void gfxDeallocated(const LBXSpriteData* data);
    
  public:
    static void init();
    
    static bool shouldAllocateLBX(LBXID ident) { return file(ident).sprites == nullptr; }
    
    static const LBXFile& loadLBX(LBXID ident);
    static const LBXFile& holderForID(LBXID ident) { return file(ident); }
    
    static const LBXFile& loadLBXTerrain();
    static const LBXFile& loadLBXTerrainMap();
    static const LBXFile& loadLBXFontsAndPalettes();
    
    static const LBXFile& loadLBXHelp();
    static void loadBuildDesc();
    
    static bool shouldAllocateSprite(SpriteInfo info) { return file(info.lbx()).sprites[info.index()] == nullptr; }
    
    static void loadMultipleLBXSpriteData(LBXID lbx);
    static const LBXSpriteData* loadLBXSpriteData(SpriteInfo info);
    static const LBXArrayData* loadLBXArrayData(const LBXFile& lbx, size_t index);
    
    static const LBXSpriteData* loadLBXSpriteTerrainData(SpriteInfo info);
    static LBXArrayData* loadLBXSpriteTerrainMappingData(LBXFile& lbx, size_t i, FILE* in);
    
    static const std::vector<LBXTerrainSpriteSpecs>& terrainInfo() { return terrainData; }
    
    static void clearCache(LBXID ident)
    {
      const LBXFile& f = file(ident);
      for (size_t i = 0; i < f.size(); ++i)
        if (f.sprites[i])
        {
          gfxDeallocated(f.sprites[i]);
          delete f.sprites[i];
          f.sprites[i] = nullptr;
        }
    }
    
    static const LBXSpriteData* spriteFor(SpriteInfo info) {
      assert(static_cast<u32>(info.lbx()) < LBX_COUNT);
      
      LBXFile& lbxFile = file(info.lbx());
      const LBXSpriteData* sprite = lbxFile.sprites[info.index()];
      
      if (!sprite)
        sprite = loadLBXSpriteData(info);
      
      return sprite;
    }
    
    static const LBXArrayData* arrayFor(LBXID ident, size_t index)
    {
      LBXFile& lbxFile = file(ident);
      return lbxFile.arrays[index];
    }
    
    static file_handle getDescriptor(const LBXFile& ident);
    static Path getLBXPath(const std::string& ident);
    
    friend class LBXView;
  };
}
