//
//  ColorMap.h
//  OpenMoM
//
//  Created by Jack on 7/18/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _COLOR_MAP_H_
#define _COLOR_MAP_H_

#include "Common.h"

#include <initializer_list>
#include <unordered_set>
#include <unordered_map>

class ColorMap
{
  public:
    virtual u32 get(u32 k) = 0;
};

class BlinkMap : public ColorMap
{
  private:
    const s16 s[3];
    const s16 d[3];
    const u16 ticks, updatedForTick = -1;
  
    std::unordered_set<Color> set;
    Color color;
  
  
  public:
    BlinkMap(std::initializer_list<Color>colors, u8 r1, u8 g1, u8 b1, u8 r2, u8 g2, u8 b2, u16 ticks) :
      set(colors), s{r1,g1,b1}, d{static_cast<s16>(r2-r1),static_cast<s16>(g2-g1),static_cast<s16>(b2-b1)}, ticks(ticks) { }
  
    u32 get(u32 k) override;
  
};

class HashColorMap : public ColorMap
{
  private:
    std::unordered_map<Color, Color> map;
  
  public:
    HashColorMap(std::initializer_list<Color> data)
    {
      std::initializer_list<Color>::iterator it = data.begin();
      
      while (it != data.end())
      {
        Color src = *it++;
        Color dst = *it++;
        map[src] = dst;
      }
    }
  
    u32 get(u32 k) override
    {
      std::unordered_map<Color,Color>::iterator it = map.find(k);
      
      return it != map.end() ? it->second : k;
    }
};

#endif
