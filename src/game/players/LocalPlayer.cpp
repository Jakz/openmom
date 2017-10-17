//
//  LocalPlayer.cpp
//  OpenMoM
//
//  Created by Jack on 7/19/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "LocalPlayer.h"

#include "common/Util.h"
#include "World.h"
#include "Unit.h"
#include "Game.h"

#include "SDLHelper.h"
#include "ViewManager.h"

using namespace std;

LocalPlayer::LocalPlayer(Game *game, std::string name, const Wizard* wizard, PlayerColor color, const Race* race, u16 mapWidth, u16 mapHeight) :
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
}

const movement_list LocalPlayer::selectedArmyMovementType()
{
  return g->mapMechanics.movementTypeOfArmy(selectedUnits);
}

void LocalPlayer::push(anims::Animation* animation)
{
  SDL::gvm->push(animation);
}

void LocalPlayer::computeRoute(const Position goal)
{
  selectedRoute.reset(g->world->pathfinder.computeRoute(g->world, selectedArmy->getPosition(), selectedUnits, this, goal));
}

bool LocalPlayer::consumeRoute()
{
  Army* army;
  if (!wholeSelected())
    army = splitAndSelect();
  else
  {
    army = selectedArmy;
    g->world->get(army->getPosition())->unplaceArmy();
  }
  
  army->setRoute(selectedRoute.release());
  return g->consumeMovement(army);
}

Army* LocalPlayer::splitAndSelect()
{
  Army* newArmy = new Army(this);
  
  for (auto u : *selectedArmy)
  {
    if (isSelectedUnit(u))
    {
      newArmy->add(u);
      u->setArmy(newArmy);
    }
  }
  
  selectNone();
  for (auto u : *newArmy)
  {
    selectedArmy->remove(u);
    selectedUnits.push_back(u);
  }
  
  newArmy->setPosition(selectedArmy->getPosition());
  selectArmy(newArmy);
  
  
  this->selectedArmy = newArmy;
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
  selectedRoute.reset(nullptr);
}

void LocalPlayer::deselectUnit(Unit* unit)
{
  if (selectedArmy && selectedUnits.size() > 1)
  {
    selectedUnits.remove(unit);
    selectedRoute.reset(nullptr);
  }
}

bool LocalPlayer::isSelectedUnit(Unit* unit) const
{
  return find(selectedUnits.begin(), selectedUnits.end(), unit) != selectedUnits.end();
}

void LocalPlayer::resetArmy()
{
  selectedArmy = nullptr;
  selectedRoute.reset(nullptr);
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
