//
//  LocalPlayer.cpp
//  OpenMoM
//
//  Created by Jack on 7/19/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "LocalPlayer.h"

#include "Util.h"
#include "World.h"
#include "Unit.h"
#include "Game.h"

using namespace std;

LocalPlayer::LocalPlayer(Game *game, std::string name, const Wizard& wizard, PlayerColor color, const Race& race, u16 mapWidth, u16 mapHeight) :
Player(game,name,wizard,color,race,mapWidth,mapHeight), viewport(Position(0,0,ARCANUS)), selectedArmy(nullptr), selectedRoute(nullptr), combatSelectedUnit(nullptr), combatCurrentlyPlaying(false), map(MiniMap(game->world)) { }

void LocalPlayer::setViewport(s16 x, s16 y)
{
  x = Util::wrap(x, g->world->w);
  
  viewport.x = x;
  viewport.y = y;
}

void LocalPlayer::selectArmy(Army* army)
{
  selectedArmy = army;
  
  if (!g->revalidateRoute(army))
    army->setRoute(nullptr);
  
  selectedRoute = army->getRoute();
}

const movement_list LocalPlayer::selectedArmyMovementType()
{
  return g->mapMechanics.movementTypeOfArmy(selectedUnits);
}


bool LocalPlayer::selectedArmyCanBuildOutpost()
{
  if (!g->cityMechanics.canCityBeBuiltOnTile(g->world->get(selectedArmy->getPosition())))
    return false;
  
  for (auto u : selectedUnits)
  {
    if (u->skills()->hasSimpleEffect(SimpleEffect::Type::CREATE_OUTPOST))
      return true;
  }
  
  return false;
}

void LocalPlayer::push(anims::Animation* animation)
{
  // TODO
}

void LocalPlayer::computeRoute(s16 dx, s16 dy)
{
  delete selectedRoute;
  selectedRoute = g->world->pathfinder.computeRoute(g->world, selectedArmy, dx, dy);
  
  selectedArmy->setRoute(selectedRoute);
  
  if (selectedRoute)
    selectedRoute->setArmy(selectedArmy);
}

bool LocalPlayer::consumeRoute()
{
  Army* army;
  if (!wholeSelected())
    army = splitAndSelect();
  else
    army = selectedArmy;
  
  army->setRoute(selectedRoute);
  selectedRoute->setArmy(army);
  return g->consumeMovement(selectedArmy);
}

Army* LocalPlayer::splitAndSelect()
{
  Army* newArmy = new Army(this);
  Route* r = selectedRoute;
  
  for (auto u : *selectedArmy)
  {
    if (isSelectedUnit(u))
      newArmy->add(u);
  }
  
  selectNone();
  for (auto u : *newArmy)
  {
    selectedArmy->remove(u);
    selectUnit(u);
  }
  
  newArmy->setPosition(selectedArmy->getPosition());
  selectedArmy->clearRoute();
  selectArmy(newArmy);
  
  selectedRoute = r;
  return newArmy;
}

void LocalPlayer::selectAll()
{
  selectedUnits.clear();
  
  for (auto u : *selectedArmy)
    selectedUnits.push_back(u);
}

void LocalPlayer::selectNone()
{
  selectedUnits.clear();
}

void LocalPlayer::selectUnit(Unit* unit)
{
  selectedUnits.push_back(unit);
  delete selectedRoute;
  selectedRoute = nullptr;
}

void LocalPlayer::deselectUnit(Unit* unit)
{
  if (selectedArmy && selectedUnits.size() > 1)
  {
    selectedUnits.remove(unit);
    delete selectedRoute;
    selectedRoute = nullptr;
  }
}

bool LocalPlayer::isSelectedUnit(Unit* unit)
{
  return find(selectedUnits.begin(), selectedUnits.end(), unit) != selectedUnits.end();
}

s16 LocalPlayer::selectedAvailMoves()
{
  s16 min = numeric_limits<s16>::max();
  for (auto u : selectedUnits)
  {
    min = std::min(u->getAvailableMoves(),min);
  }
  return min;
}
