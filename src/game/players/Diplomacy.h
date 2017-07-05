//
//  Diplomacy.h
//  OpenMoM
//
//  Created by Jack on 7/30/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _DIPLOMACY_H_
#define _DIPLOMACY_H_

#include "common/Common.h"

#include <unordered_map>

class Player;
class Game;

class RelationLevel
{
public:
  enum class ID
  {
    HATRED = 0,
    TROUBLED,
    TENSE,
    RESTLESS,
    UNEASY,
    NEUTRAL,
    RELAXED,
    CALM,
    PEACEFUL,
    FRIENDLY,
    HARMONY
  } ident;
  
  const I18 name;
  const s16 minDP;
  
  RelationLevel(ID ident, I18 name, s16 minDP) : ident(ident), name(name), minDP(minDP) { }
  
  static const RelationLevel* getByDP(s16 dp)
  {
    for (int i = std::extent<decltype(LEVELS)>::value; i > 0; --i)
      if (LEVELS[i].minDP <= dp)
        return &LEVELS[i];

    return nullptr;
  }
  
  static const RelationLevel LEVELS[];
};

class Relation
{
private:
  const RelationLevel *level;
  Player* const from;
  Player* const to;
  bool discovered;
  s16 dp;
  
public:
  Relation(Player* from, Player* to) : from(from), to(to), level(&RelationLevel::LEVELS[static_cast<u16>(RelationLevel::ID::NEUTRAL)]), discovered(false), dp(0) { }
  
  bool isDiscovered() const { return discovered; }
  
  s16 points() { return dp; }
  void incr(s16 v) { dp += v; level = RelationLevel::getByDP(dp); }
};

class Relations
{
private:
  Player* player;
  std::unordered_map<const Player*, Relation*> relations;
  
public:
  Relations(Player* player, Game *game);
  ~Relations()
  {
    for (auto p : relations)
      delete p.second;
  }
  
  Relation* get(const Player* player) { return relations[player]; }

  
  
  typedef std::unordered_map<const Player*, Relation*>::iterator iterator;
  typedef std::unordered_map<const Player*, Relation*>::const_iterator const_iterator;
  iterator begin() { return relations.begin(); }
  iterator end() { return relations.end(); }
  const_iterator begin() const { return relations.begin(); }
  const_iterator end() const { return relations.end(); }
};

#endif
