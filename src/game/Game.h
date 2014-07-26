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
#include <string>

class GlobalSpell;
class Player;
class LocalPlayer;
class World;
class City;
class Player;
class Tile;

class Game
{
private:
  std::list<City*> cities;
  std::list<Player*> players;
  
  std::list<Player*>::iterator current;
  
  bool firstTurn;
  s32 turnCounter;
  
public:
  Game();
  
  void dummyInit();
  
  void placeArmy(Army* army, const Position& position);
  void computeRoute(Army* army, const Position& position);
  
  bool settleCity(Army* army, const std::string name);
  void settleCity(Player* player, const std::string name, const Position& position);
  void settleCity(Player* player, const std::string name, u16 population, const Position& position);
  void settleCity(City* city);
  
  Player* currentPlayer() { return *current; }

  const std::list<City*>& getCities() { return cities; }
  
  bool meldNode(const Army* army, const Position& position);
  
  bool startCast(Player* player, const Spell* spell);
  void castSpell(Unit* unit, const Spell* spell, const Player* player);
  bool castSpell(Unit* unit, Player* player, bool combat);
  void castSpell(City* city, const Spell* spell, const Player* player);
  void castCombatspell(Player* player, const Spell* spell);
  bool castSpell(City* city, Player* player);
  bool castSpell(const GlobalSpell* spell, Player* player);
  bool castSpell(Tile* tile, Player* player);
  void dispelCast(City* city, const Player* player, const Spell* spell);
  void cancelCast(Player* player);
  
  void addSkill(Unit* unit, const Skill* skill);
  
  bool consumeMovement(Army* army);
  bool revalidateRoute(Army* army);
  
  const std::vector<const Productable*> availableUnits(const City* city);
  
  bool alchemy(Player* player, s32 amount, bool fromGold);

  void turnBeginning();
  
  void nextTurn()
  {
    ++current;
    
    if (current == players.end())
    {
      firstTurn = false;
      ++turnCounter;
      current = players.begin();
    }
    
    if (!firstTurn)
      turnBeginning();
  }
  
  void nextLocal()
  {
    // TODO
  }
  
  u32 getTurnCount() { return turnCounter; }

  
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
  
  
  
    static LocalGame* i;
};

#endif
