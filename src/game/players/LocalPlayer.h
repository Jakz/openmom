//
//  LocalPlayer.h
//  OpenMoM
//
//  Created by Jack on 7/19/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _LOCAL_PLAYER_H_
#define _LOCAL_PLAYER_H_

#include "Player.h"

class Game;

class LocalPlayer : public Player
{
private:
  s16 viewportX, viewportY;
  
public:
  LocalPlayer(Game *game, std::string name, const Wizard& wizard, PlayerColor color, const Race& race, u16 mapWidth, u16 mapHeight) :
    Player(game,name,wizard,color,race,mapWidth,mapHeight) { }
  
  void discoverTile(const Position& position) override { } // TODO

  bool hasMessage() { return false; } // TODO
};

#endif
