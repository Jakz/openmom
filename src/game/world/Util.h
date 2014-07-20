//
//  Util.h
//  OpenMoM
//
//  Created by Jack on 7/20/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _UTIL_H_
#define _UTIL_H_

#include "Common.h"

#include <random>

class Util
{
private:
  static std::mt19937 rng;
  static std::uniform_real_distribution<> real;
  static std::uniform_int_distribution<> integer;


public:
  static float rand() { return real(rng); }
  
  static u32 randomIntUpTo(u32 i) { return integer(rng)%i; }
  static u32 randomIntInclusive(u32 l, u32 h) { return l + integer(rng)%(h-l+1); }

  static bool chance(u32 percent) { return randomIntUpTo(101) <= percent; }
  static bool chance(float percent) { return rand() <= percent; }
  
  static constexpr PositionOffset DIRS[] = {{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1}};
  static constexpr PositionOffset ODIRS[] = {{0,-1},{1,0},{0,1},{-1,0}};
  
  static constexpr size_t DIRS_LENGTH = sizeof(DIRS)/sizeof(DIRS[0]);
  static constexpr size_t ODIRS_LENGTH = sizeof(ODIRS)/sizeof(ODIRS[0]);
};

#endif
