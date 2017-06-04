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

enum class HouseType : u8
{
	NORMAL,
	TREE,
	MUD
};

class Race
{
  private:
    static const Race races[];

  public:
    //const RaceID ident;
    const HouseType houseType;
  
    const s16 baseProduction;
    const float taxIncomeMultiplier;
    const float miningBonusMultiplier;
    const float manaProducedPerCitizen;
    const s16 foodProductionPerFarmer;
  
    const s16 growthBonus;
    const float outpostGrowthChance;
  
    const char* cityNames[20];
  
    const std::string& name() const;
    const std::string& unitName() const;
  
  static const Race& race(RaceID ident) { return races[static_cast<u8>(ident)]; }
};

#endif
