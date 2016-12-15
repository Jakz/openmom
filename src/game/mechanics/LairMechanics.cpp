//
//  LairMechanics.cpp
//  OpenMoM
//
//  Created by Jack on 8/1/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "game/mechanics/LairMechanics.h"

#include "game/Game.h"
#include "Places.h"
#include "tile.h"

enum LairType
{
  TOWER_OF_WIZARDRY = 0,
  NORMAL_LAIR,
  WEAK_LAIR
};

static const Range LAIR_BUDGETS[][2][5] = {
  {{{175,300},{350,600},{575, 900},{700,1200},{875,1500}},{{175,300},{350, 600},{575, 900},{700,1200},{875,1500}}}, // towers
  {{{ 25,325},{ 50,750},{ 75,1075},{100,1500},{125,1825}},{{ 50,625},{100,1250},{150,1875},{200,2500},{250,3125}}}, // weak lairs
  {{{  2, 25},{  5, 50},{  7,  75},{ 10, 100},{ 12, 125}},{{  2, 50},{  5, 100},{  7, 150},{100, 200},{ 12, 250}}
  }}; // lairs

const Range& LairMechanics::budgetForPlace(Place* place, Plane plane)
{
  if (place->type == PLACE_TOWER_OF_WIZARDRY)
    return LAIR_BUDGETS[TOWER_OF_WIZARDRY][plane][game->settings.group(SETTING_DIFFICULTY).index()];
  else if (place->isWeak)
    return LAIR_BUDGETS[WEAK_LAIR][plane][game->settings.group(SETTING_DIFFICULTY).index()];
  else
    return LAIR_BUDGETS[NORMAL_LAIR][plane][game->settings.group(SETTING_DIFFICULTY).index()];
}


void LairMechanics::generateGuardsForPlace(Place *place, Tile* tile)
{

}

void LairMechanics::generateGuardsForNode(ManaNode* node, Tile* tile)
{
  
}
