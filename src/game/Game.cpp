//
//  Game.cpp
//  OpenMoM
//
//  Created by Jack on 7/19/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "Game.h"

#include "World.h"
#include "LocalPlayer.h"


Game::Game() : world(new World(this, 60,40))
{
  
}


LocalGame* LocalGame::i = nullptr;

LocalGame::LocalGame()  { }
