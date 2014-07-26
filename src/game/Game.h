//
//  Game.h
//  OpenMoM
//
//  Created by Jack on 7/19/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _GAME_H_
#define _GAME_H_

#include "Settings.h"
#include "MapMechanics.h"
#include "SpellMechanics.h"
#include "EventMechanics.h"
#include "CityMechanics.h"
#include "PlayerMechanics.h"

#include <list>

class Player;
class LocalPlayer;
class World;
class City;
class Player;

class Game
{
private:
  std::list<City*> cities;
  std::list<const Player*> players;
  
  std::list<const Player*>::iterator current;
  
  bool firstTurn;
  s32 turnCounter;
  
public:
  Game();
  
  void dummyInit();
  
  Settings settings;
  
  MapMechanics mapMechanics;
  SpellMechanics spellMechanics;
  EventMechanics eventMechanics;
  CityMechanics cityMechanics;
  PlayerMechanics playerMechanics;
  World* const world;
};


class LocalGame
{
private:
  const Game* game;

public:
  LocalGame(const Game* game) : game(game) { }
  
    LocalPlayer *currentPlayer;
  
    void placeArmy(const Army* army, const Position& position);
  
  
    static LocalGame* i;
};

#endif
