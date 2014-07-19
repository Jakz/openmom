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
  
  
  public:
    LocalPlayer(Game *game) : Player(game) { }
  
    bool hasMessage() { return false; } // TODO
};

#endif
