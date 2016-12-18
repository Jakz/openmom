//
//  Race.h
//  OpenMoM
//
//  Created by Jack on 7/17/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _RACE_H_
#define _RACE_H_

#include "Common.h"

enum class RaceID : u8
{
  BARBARIANS = 0,
  BEASTMEN,
  DARK_ELVES,
  DRACONIANS,
  DWARVES,
  GNOLLS,
  HALFLINGS,
  HIGH_ELVES,
  HIGH_MEN,
  KLACKONS,
  LIZARDMEN,
  NOMADS,
  ORCS,
  TROLLS,
};

enum HouseType : u8
{
	HOUSE_NORMAL,
	HOUSE_TREE,
	HOUSE_MUD
};

class Race
{
  private:
    static const Race races[];
  
  
  public:
    const RaceID ident;
    const HouseType houseType;
    const s16 growthBonus;
    const float outpostGrowthChance;
    const char* cityNames[20];
  
  static const Race& race(RaceID ident) { return races[static_cast<u8>(ident)]; }
};

#endif
