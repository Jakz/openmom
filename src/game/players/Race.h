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

enum RaceID : u8
{
	RACE_BARBARIANS = 0,
	RACE_BEASTMEN,
	RACE_DARK_ELVES,
	RACE_DRACONIANS,
	RACE_DWARVES,
	RACE_GNOLLS,
	RACE_HALFLINGS,
	RACE_HIGH_ELVES,
	RACE_HIGH_MEN,
	RACE_KLACKONS,
	RACE_LIZARDMEN,
	RACE_NOMADS,
	RACE_ORCS,
	RACE_TROLLS,
  
  RACE_COUNT
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
    const I18 name;
    const I18 uname;
    const HouseType houseType;
    const s16 growthBonus;
    const float outpostGrowthChance;
  
  static const Race& race(RaceID ident) { return races[ident]; }
};

#endif
