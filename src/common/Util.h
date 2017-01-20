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
#include <vector>

class Util
{
private:
  static std::mt19937 rng;
  static std::uniform_real_distribution<float> real;
  static std::uniform_int_distribution<> integer;


public:
  static void seed(u32 s) { rng.seed(s); }
  static float rand() { return real(rng); }
  static float rand(float max) { return real(rng)*max; }
  
  static u32 passingRollsf(u32 count, float ch)
  {
    u32 passed = 0;
    for (u32 i = 0; i < count; ++i)
      if (chance(ch)) ++passed;
    return passed;
  }
  
  static u32 passingRolls(u32 count, u32 ch)
  {
    u32 passed = 0;
    for (u32 i = 0; i < count; ++i)
      if (chance(ch)) ++passed;
    return passed;
  }
  
  static u32 randomIntUpTo(u32 i) { return integer(rng)%i; }
  static u32 randomIntInclusive(s32 l, s32 h) { return l + integer(rng)%(h-l+1); }

  static bool oneOfTwoChance() { return rand() <= 0.50f; }
  static bool chance(u32 percent) { return randomIntUpTo(101) <= percent; }
  static bool chance(float percent) { return rand() <= percent; }
  
  static constexpr PositionOffset DIRS[] = {{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1}};
  static constexpr PositionOffset ODIRS[] = {{0,-1},{1,0},{0,1},{-1,0}};
  
  static constexpr size_t DIRS_LENGTH = sizeof(DIRS)/sizeof(DIRS[0]);
  static constexpr size_t ODIRS_LENGTH = sizeof(ODIRS)/sizeof(ODIRS[0]);
  
  template<typename T> static const T& randomElement(const std::vector<T>& data) { return data[randomIntUpTo(data.size())]; }
  template<typename T> static typename T::const_iterator randomElementIterator(const T& data) { return data.begin() + randomIntUpTo(data.size()); }

  
  static inline s32 wrap(const s32 v, const s32 m) { return v < 0 ? (v + m) : ((v >= m) ? v%m : v); }
};

#endif
