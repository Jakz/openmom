//
//  Player.h
//  OpenMoM
//
//  Created by Jack on 7/19/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Common.h"


#include <list>
#include <string>

class Race;
class Game;
class City;

class Player
{
  private:
    std::list<City*> cities;

    Game *g;
  
  public:
    Player(Game *game, std::string name, const Wizard& wizard, PlayerColor color, const Race& race, u16 mapWidth, u16 mapHeight) :
      g(game), wizard(wizard), race(race), color(color), name(name)
    { }
  
    Game *game() { return g; }

    void add(City* city) { cities.push_back(city); }
  
    const std::string name;
    const Wizard& wizard;
    const PlayerColor color;
    const Race& race;
};

#endif
