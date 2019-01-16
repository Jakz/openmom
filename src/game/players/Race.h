//
//  Race.h
//  OpenMoM
//
//  Created by Jack on 7/17/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _RACE_H_
#define _RACE_H_

#include "common/Common.h"

#include <vector>
#include <string>

enum class HouseType : u8
{
	NORMAL,
	TREE,
	MUD
};

class Race
{
public:
  const s16 growthBonus;
  const float outpostGrowthChance;
  
  const s16 baseProduction;
  const float taxIncomeMultiplier;
  const float miningBonusMultiplier;
  const float manaProducedPerCitizen;
  const s16 foodProductionPerFarmer;
  const Plane startingPlane;

  const std::vector<std::string> cityNames;
  
  Race(s16 growthBonus, float outpostGrowthChance, s16 baseProduction, float taxIncomeMultiplier, float miningBonusMultiplier, float manaProducedPerCitizen, s16 foodProductionPerFarmer, Plane startingPlane, const std::vector<std::string>& cityNames) :
  growthBonus(growthBonus), outpostGrowthChance(outpostGrowthChance),
  baseProduction(baseProduction), taxIncomeMultiplier(taxIncomeMultiplier),
  miningBonusMultiplier(miningBonusMultiplier), manaProducedPerCitizen(manaProducedPerCitizen),
  foodProductionPerFarmer(foodProductionPerFarmer), startingPlane(startingPlane),
  cityNames(cityNames)
  {
    
  }
};

#endif
