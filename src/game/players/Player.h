//
//  Player.h
//  OpenMoM
//
//  Created by Jack on 7/19/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _PLAYER_H_
#define _PLAYER_H_

class Game;

class Player
{
  private:
    Game *g;
  
  public:
    Player(Game *game) : g(game) { }
  
    Game *game() { return g; }
};

#endif
