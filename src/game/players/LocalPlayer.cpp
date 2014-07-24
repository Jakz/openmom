//
//  LocalPlayer.cpp
//  OpenMoM
//
//  Created by Jack on 7/19/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "LocalPlayer.h"

#include "Game.h"

LocalPlayer::LocalPlayer(Game *game, std::string name, const Wizard& wizard, PlayerColor color, const Race& race, u16 mapWidth, u16 mapHeight) :
Player(game,name,wizard,color,race,mapWidth,mapHeight), viewport(Position(0,0,ARCANUS)), selectedArmy(nullptr), selectedRoute(nullptr), combatSelectedUnit(nullptr), combatCurrentlyPlaying(false), map(MiniMap(game->world)) { }



void LocalPlayer::selectArmy(Army* army)
{
  selectedArmy = army;
  
  //if (!game->revalidateRoute(army)) // TODO
    army->setRoute(nullptr);
  
  selectedRoute = army->getRoute();
}

const movement_list LocalPlayer::selectedArmyMovementType()
{
  return g->mapMechanics.movementTypeOfArmy(selectedUnits);
}


bool LocalPlayer::selectedArmyCanBuildOutpost()
{
  // TODO
  return false;
}