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

#include <vector>

class LBX
{
private:
  typedef u32 LBXOffset;
  typedef std::vector<LBXOffset> offset_list;
  
  struct LBXHeader
  {
    u16 count;
    u32 magic;
    u16 type;
  } __attribute__((__packed__));

  static bool loadHeader(LBXHeader& header, offset_list& offsets, FILE *in);
  
  static void scanGfx(LBXHeader& header, LBXOffset offset, FILE *in);
  static void scanFileNames(LBXHeader& header, offset_list& offsets, FILE *in);

  static void scanFonts(LBXHeader& header, std::vector<LBXOffset>& offsets, FILE *in);
  
public:
  static void load();

};

#endif
