//
//  LairMechanics.h
//  OpenMoM
//
//  Created by Jack on 8/1/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _LAIR_MECHANICS_H_
#define _LAIR_MECHANICS_H_

#include "common/Common.h"

class Game;
class Place;
class ManaNode;
class Tile;

struct Range
{
  const s16 min;
  const s16 max;
};

class LairMechanics
{
private:
  Game* game;
  
  const Range& budgetForPlace(Place* place, Plane plane);

public:
  LairMechanics(Game* game) : game(game) { }
  
  void generateGuardsForPlace(Place *place, Tile* tile);
  void generateGuardsForNode(ManaNode* node, Tile* tile);
};

#endif
