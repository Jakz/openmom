//
//  LocalPlayer.h
//  OpenMoM
//
//  Created by Jack on 7/19/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _LOCAL_PLAYER_H_
#define _LOCAL_PLAYER_H_

#include "Common.h"

#include "Spells.h"
#include "Army.h"
#include "Player.h"

#include "Messages.h"

#include <list>

class Game;

class LocalPlayer : public Player
{
private:
  Position viewport;

  s16 spellBookPage;
  
  Army* selectedArmy;
  Route* selectedRoute;
  unit_list selectedUnits;
  
  bool drawSelectedArmy = true;
  bool mapGridEnabled = true;
  
  Target target = Target::NONE;
  
  CombatUnit *combatSelectedUnit;
  bool combatCurrentlyPlaying;
  
  std::list<const messages::Message*> messages;
  
public:
  LocalPlayer(Game *game, std::string name, const Wizard& wizard, PlayerColor color, const Race& race, u16 mapWidth, u16 mapHeight) :
    Player(game,name,wizard,color,race,mapWidth,mapHeight) { }
  
  void discoverTile(const Position& position) override { } // TODO

  bool hasMessage() { return false; } // TODO
};

#endif
