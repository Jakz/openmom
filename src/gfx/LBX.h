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
  
  static void scanGfx(LBXHeader& header, LBXOffset offset, FILE *in);
  static void scanFileNames(LBXHeader& header, offset_list& offsets, string_list& names, FILE *in);

  static void loadText(LBXHeader& header, offset_list& offsets, FILE *in);
  static void loadFonts(LBXHeader& header, offset_list& offsets, FILE *in);
  
public:
  static void load();
  
  friend class LBXView;
};

#include "View.h"

class ViewManager;

class LBXView : public View
{
private:
  s16 selectedLBX;
  
  void draw() override;
  void drawPost() override { }
  
public:
  LBXView(ViewManager* gvm);
  
  void activate() override { }
  void deactivate() override { }
  
  void mouseReleased(u16 x, u16 y, MouseButton b) override;
  void selectLBX(std::string filename);
};

#endif
