//
//  MapMechanics.h
//  OpenMoM
//
//  Created by Jack on 7/22/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _MAP_MECHANICS_H_
#define _MAP_MECHANICS_H_

#include "Common.h"
#include "Army.h"

//enum TileType : u8;


class Game;
class World;
class ManaNode;
class Army;
class Tile;

class MapMechanics
{
private:
  Game* const g;
  
public:
  MapMechanics(Game* g) : g(g) { }
  
  float chanceResourceForTile(TileType type, Plane which) const;
  Resource generateResourceForTile(TileType type, Plane which) const;
  
  ManaNode* generateManaNode(World* world, const Position position, const School school) const;
  
  virtual u16 turnsRequiredToBuildRoadOnTile(Tile* tile) const;
  virtual s16 movementCost(World* world, const Position& position, const movement_list& movement) const;
  
  const movement_list movementTypeForSetOfEffects(const movement_list_group& movements) const;
  const movement_list movementTypeOfArmy(const unit_list& units) const;
  
  // TODO: real mechanics
  bool canSeeEnemyUnitsInCity(const Player* player, const City* city) const { return true; }
  // TODO: is that even necessary?
  bool canSeeEnemyUnitsInArmy(const Player* player, const Tile* tile) const { return true; }
};


#endif
