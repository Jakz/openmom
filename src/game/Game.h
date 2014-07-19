//
//  Game.h
//  OpenMoM
//
//  Created by Jack on 7/19/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _GAME_H_
#define _GAME_H_

class Player;
class LocalPlayer;

class Game
{
  
};


class LocalGame
{
  public:
    LocalPlayer *currentPlayer;
  
    static LocalGame *i;
};

#endif
