//
//  Diplomacy.cpp
//  OpenMoM
//
//  Created by Jack on 7/30/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "Diplomacy.h"

#include "game/Game.h"

#include "Localization.h"

const RelationLevel RelationLevel::LEVELS[] = {
  RelationLevel(ID::HATRED, I18::RELATION_HATRED, -100),
  RelationLevel(ID::TROUBLED, I18::RELATION_TROUBLED, -82),
  RelationLevel(ID::TENSE, I18::RELATION_TENSE, -64),
  RelationLevel(ID::RESTLESS, I18::RELATION_RESTLESS, -46),
  RelationLevel(ID::UNEASY, I18::RELATION_UNEASY, -28),
  RelationLevel(ID::NEUTRAL, I18::RELATION_NEUTRAL, -10),
  RelationLevel(ID::RELAXED, I18::RELATION_RELAXED, 11),
  RelationLevel(ID::CALM, I18::RELATION_CALM, 29),
  RelationLevel(ID::PEACEFUL, I18::RELATION_PEACEFUL, 47),
  RelationLevel(ID::FRIENDLY, I18::RELATION_FRIENDLY, 65),
  RelationLevel(ID::HARMONY, I18::RELATION_HARMONY, 83)
};


Relations::Relations(Player* player, Game* game) : player(player)
{
  for (auto p : game->getPlayers())
  {
    if (p != player)
      relations[p] = new Relation(player,p);
  }
}
