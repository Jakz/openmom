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

#include "Army.h"
#include "Unit.h"


Game::Game() : world(new World(this, 60,40)), mapMechanics(*this), spellMechanics(*this), eventMechanics(*this), cityMechanics(*this), playerMechanics(*this), firstTurn(true), turnCounter(0)
{
  
}

void Game::dummyInit()
{
  LocalPlayer *player = new LocalPlayer(this, "Kali", Data::wizard(KALI), GREEN, Race::race(RaceID::BARBARIANS), 60, 40);
  players.push_back(player);
  current = players.begin();
  
  world->fill(TILE_WATER, ARCANUS);
  world->fill(TILE_WATER, MYRRAN);
  world->rect(TILE_GRASS, 3, 3, 10, 10, ARCANUS);
  world->rect(TILE_MOUNTAIN, 4, 4, 2, 2, ARCANUS);
  world->line(TILE_RIVER, 4, 8, 11, false, ARCANUS);
  world->rect(TILE_GRASS, 1, 1, 6, 6, MYRRAN);
  world->line(TILE_MOUNTAIN, 6, 8, 5, false, MYRRAN);
  world->line(TILE_HILL, 5, 8, 6, true, MYRRAN);
  world->line(TILE_WATER, 2, 6, 6, true, ARCANUS);
  world->set(TILE_WATER, 4, 7, ARCANUS);
  world->set(TILE_HILL, 4,4, ARCANUS);
  world->set(TILE_DESERT, 8,8, ARCANUS);
  world->set(TILE_DESERT, 7,8, ARCANUS);
  world->set(TILE_DESERT, 9,8, ARCANUS);
  world->set(TILE_DESERT, 8,7, ARCANUS);
  world->set(TILE_DESERT, 8,9, ARCANUS);
  world->set(TILE_DESERT, 7,9, ARCANUS);
  world->set(TILE_DESERT, 9,9, ARCANUS);
  world->set(TILE_DESERT, 10,9, ARCANUS);
  
  //WorldGenerator.generate(world, ARCANUS);
  //WorldGenerator.generate(world, MYRRAN);

  Army* a = new Army(player, {
		new RaceUnit(*UnitSpec::raceSpec(UnitID::SPEARMEN, RaceID::BARBARIANS)),
		new RaceUnit(*UnitSpec::raceSpec(UnitID::SWORDSMEN, RaceID::BARBARIANS)),
		new RaceUnit(*UnitSpec::raceSpec(UnitID::BOWMEN, RaceID::BARBARIANS)),
		new RaceUnit(*UnitSpec::raceSpec(UnitID::CAVALRY, RaceID::BARBARIANS)),
    new FantasticUnit(*UnitSpec::summonSpec(UnitID::GREAT_DRAKE)),
		new RaceUnit(*UnitSpec::raceSpec(UnitID::BERSERKERS, RaceID::BARBARIANS)),
  });
  
  
  //map.get(5, 5, ARCANUS).placeManaNode(mapMechanics.generateManaNode(world, 5, 5, ARCANUS));
  //this.meldNode(a, 5, 5, ARCANUS);
  //map.get(5, 6, ARCANUS).placeManaNode(new ManaNode());
  //map.get(6, 5, ARCANUS).placeManaNode(new ManaNode());
  //map.get(6, 6, ARCANUS).placeManaNode(new ManaNode());
  world->get(4, 3, ARCANUS)->placeResource(Resource::COAL);
  world->get(3, 4, ARCANUS)->placeResource(Resource::WILD_GAME);
  //map.rect(TILE_GRASS, 2, 2, 6, 3);
  //map.rect(TILE_GRASS, 8, 4, 1, 1);
  world->get(3,4,ARCANUS)->placeRoad(true);
  world->get(3,5,ARCANUS)->placeRoad(false);
  world->get(4,5,ARCANUS)->placeRoad(true);
  world->get(5,5,ARCANUS)->placeRoad(true);
  world->get(7,5,ARCANUS)->placeRoad(true);
  world->set(TILE_VOLCANO, 0, 0, ARCANUS);
  world->set(TILE_VOLCANO, 59, 0, ARCANUS);
  world->set(TILE_VOLCANO, 0, 39, ARCANUS);
  world->set(TILE_VOLCANO, 59, 39, ARCANUS);
  
  world->set(TILE_HILL, 1, 2, MYRRAN);
  
  LocalGame* localGame = new LocalGame(this);

}








LocalGame* LocalGame::i = nullptr;

LocalGame::LocalGame()  { }
