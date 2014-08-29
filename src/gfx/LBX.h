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

typedef u32 LBXOffset;
typedef std::vector<LBXOffset> offset_list;
typedef std::vector<LBXFileName> string_list;

enum LBXFileID : u8
{
  LBX_ARMYLIST,
  LBX_BACKGRND,
  LBX_UNITS1,
  LBX_UNITS2,
  LBX_MAINSCRN,
  LBX_MAIN,
  
  LBX_COUNT
};

struct LBXSpriteInfo
{
  LBXFileID lbx;
  u16 index;
};

class LBXSprite
{
  
};

class LBXRepository
{
private:
  static std::string* fileNames;
  static offset_list* offsets;
  static LBXSprite ***sprites;
  
public:

  static void init()
  {
    fileNames = new std::string[LBX_COUNT];
    
    std::string names[] = {"armylist",
    "backgrnd",
    "units1",
    "units2",
    "mainscrn",
    "main"
    };
    
    for (int i = 0; i < LBX_COUNT; ++i)
      fileNames[i] = names[i];
    
    offsets = new offset_list[LBX_COUNT];
    sprites = new LBXSprite**[LBX_COUNT];
    
    for (int i = 0; i < LBX_COUNT; ++i)
      sprites[i] = nullptr;
  }
  
  static bool shouldAllocateLBX(LBXFileID ident) { return sprites[ident] == nullptr; }
  static void loadLBX(LBXFileID ident);
  
  static bool shouldAllocateSprite(LBXSpriteInfo& info) { return sprites[info.lbx][info.index] == nullptr; }
  static void loadLBXSprite(LBXSpriteInfo& info);
  
  static LBXSprite* spriteFor(LBXSpriteInfo& info) { return sprites[info.lbx][info.index]; }
  
  friend class LBXView;
};

class LBX
{
private:
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
  
  static SDL_Surface* scanGfx(LBXHeader& header, LBXOffset offset, FILE *in);
  static void scanFileNames(LBXHeader& header, offset_list& offsets, string_list& names, FILE *in);

  static void loadText(LBXHeader& header, offset_list& offsets, FILE *in);
  static void loadFonts(LBXHeader& header, offset_list& offsets, FILE *in);
  
public:
  static void load();
  
  friend class LBXView;
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
  
  std::vector<offset_list> foffsets;
  std::vector<LBXHeader> headers;
  
  std::map<std::string, string_list> filesForLBX;
  std::map<std::string, std::vector<SDL_Surface*> > gfxForLBX;
  
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
