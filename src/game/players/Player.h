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
class Army;
class Hero;

class Player
{
  private:
    std::list<City*> cities;
    std::list<Army*> armies;
    std::list<Hero*> heroes;

    Game *g;
  
  public:
    Player(Game *game, std::string name, const Wizard& wizard, PlayerColor color, const Race& race, u16 mapWidth, u16 mapHeight) :
      g(game), wizard(wizard), race(race), color(color), name(name)
    { }
  
    Game *game() const { return g; }

    void add(City* city) { cities.push_back(city); }
    void add(Army* army) { armies.push_back(army); }
    void add(Hero* hero) { heroes.push_back(hero); }
  
    void remove(Army* army) { armies.remove(army); }
  
    void selectAll() { } // TODO
    s16 selectedCount() const { return 0; } // TODO
  
    void discoverRange(Position pos, s16 range) { } // TODO
  
    const std::string name;
    const Wizard& wizard;
    const PlayerColor color;
    const Race& race;
};

#endif
