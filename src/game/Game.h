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

class Player;
class LocalPlayer;
class World;

class Game
{
public:
  Game();
  
  Settings settings;
  
  MapMechanics mapMechanics;
  World* const world;
};


class LocalGame
{
  public:
    LocalGame();
  
    LocalPlayer *currentPlayer;
  
  
  
    static LocalGame* i;
};

#endif
