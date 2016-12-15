//
//  MapMechanics.h
//  OpenMoM
//
//  Created by Jack on 7/22/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _MAP_MECHANICS_H_
#define _MAP_MECHANICS_H_

#include "common/Common.h"
#include "game/units/Army.h"

//enum TileType : u8;


class Game;
class World;
class ManaNode;
class Army;
class Tile;

class MapMechanics
{
private:
  Game& g;
  
public:
  MapMechanics(Game& g) : g(g) { }
  
  float chanceResourceForTile(TileType type, Plane which) const;
  Resource generateResourceForTile(TileType type, Plane which) const;
  
  ManaNode* generateManaNode(World* world, const Position position, const School school) const;
  
  u16 turnsRequiredToBuildRoadOnTile(Tile* tile) const;
  
  
  s16 baseMovementCost(TileType type) const;
  s16 specificMovementCost(World* world, const Position& position, const Army* army) const;
  const movement_list movementTypeOfArmy(const unit_list& units) const;
};


#endif
