//
//  Player.cpp
//  OpenMoM
//
//  Created by Jack on 7/19/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "Player.h"

#include "Army.h"
#include "Util.h"
#include "City.h"
#include "Buildings.h"
#include "Unit.h"

using namespace std;

FogMap::FogMap(u16 w, u16 h) : w(w), h(h), map{new bool**[PLANE_COUNT]}
{
  for (int i = 0; i < PLANE_COUNT; ++i)
  {
    map[i] = new bool*[w];
    for (int x = 0; x < w; ++x)
    {
      map[i][x] = new bool[h];
      memset(map[i][x], 0, sizeof(bool)*h);
    }
  }
}

bool FogMap::get(const Position& position) {
  s16 x = position.x, y = position.y;
  
  if (y < 0 || y >= h)
    return true;
  else
    return map[position.plane][Util::wrap(x, w)][y];
}

void FogMap::set(const Position& position) {
  s16 x = position.x, y = position.y;
  
  if (y >= 0 && y < h)
  {
    x = Util::wrap(x, w);
    
    map[position.plane][x][y] = true;
    player->discoverTile(position);
  }
}

void FogMap::setRect(s16 x, s16 y, s16 w, s16 h, Plane plane)
{
  for (int i = y; i < y+h; ++i)
    for (int j = x; j < x+w; ++j)
    {
      map[plane][j][i] = true;
      Position pos = Position(x,y,plane);
      player->discoverTile(pos);
    }
}

void FogMap::setRange(const Position& pos, s16 range)
{
  for (int i = -range; i <= range; ++i)
    for (int j = -range; j <= range; ++j)
      if (range == 1 || (!((i == -range || i == range) && (j == -range || j == range))))
      {
        map[pos.plane][j+pos.x][i+pos.y] = true;
        Position pos2 = Position(j+pos.x,i+pos.y,pos.plane);
        player->discoverTile(pos2);
      }
}

FogMap::~FogMap()
{
  for (int i = 0; i < PLANE_COUNT; ++i)
  {
    for (int x = 0; x < w; ++x)
      delete [] map[i][x];
    
    delete [] map[i];
    
  }
  delete [] map;
}







Player::Player(Game *game, std::string name, const Wizard& wizard, PlayerColor color, const Race& race, u16 mapWidth, u16 mapHeight) :
g(game), wizard(wizard), race(race), color(color), name(name), fogMap(new FogMap(mapWidth, mapHeight))
{
  
  goldPool = 5000;
  manaPool = 5000;
  taxRate = 1;
  alive = true;
}


void Player::turnBeginArmies()
{
  for (auto a : armies)
    a->turnBegin();
}

void Player::refreshArmies()
{
  for (auto a : armies)
    a->resetMoves();
}

City* Player::cityWithFortress()
{
  for (auto c : cities)
    if (c->hasBuilding(BuildingID::MAGE_FORTRESS))
      return c;
  
  return nullptr;
}

City* Player::cityWithSummoningCircle()
{
  for (auto c : cities)
    if (c->hasBuilding(BuildingID::SUMMONING_CIRCLE))
      return c;
  
  return nullptr;
}

s16 Player::globalSkillSpellsCount(Unit* u)
{
  u32 count = static_cast<u32>(count_if(spells.begin(), spells.end(), [&](const SpellCast& cast) {
    const Spell& spell = cast.spell;
    return spell.type == SpellType::GLOBAL_SKILL && !u->hasSpellSkill(static_cast<const SkillGlobalSpell&>(spell).skill.base);
  }));
  
  return count;
}

const SkillGlobalSpell& Player::nthGlobalSkillSpell(u16 i, Unit* u)
{
  for (auto& cast : spells)
  {
    const Spell& spell = cast.spell;
    if (spell.type == SpellType::GLOBAL_SKILL && !u->hasSpellSkill(static_cast<const SkillGlobalSpell&>(spell).skill.base))
    {
      if (i > 0) { --i; continue; }
      return static_cast<const SkillGlobalSpell&>(spell);
    }
  }
  
  return static_cast<const SkillGlobalSpell&>(spells.front().spell); // TODO: hacky and bad practice, the whole management should be refactored
}



