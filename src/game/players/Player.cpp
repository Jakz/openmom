//
//  Player.cpp
//  OpenMoM
//
//  Created by Jack on 7/19/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "Player.h"

#include "Army.h"
#include "common/Util.h"
#include "City.h"
#include "Buildings.h"
#include "Unit.h"
#include "Game.h"

using namespace std;

FogMap::FogMap(Player* player, u16 w, u16 h) : player(player), size(w,h), map(new bool[PLANE_COUNT*w*h])
{
  std::fill(map.get(), map.get() + (w*h*PLANE_COUNT), false);
}

bool FogMap::get(Position position) const
{
  if (position.wrapAndCheckValidity(size.w, size.h))
    return unsafeGet(position);
  else
    return true;
}

void FogMap::set(Position position)
{
  if (position.wrapAndCheckValidity(size.w, size.h))
  {
    unsafeSet(position);
    player->discoverTile(position);
  }
}

void FogMap::setRect(s16 x, s16 y, s16 w, s16 h, Plane plane)
{
  for (int i = y; i < y+h; ++i)
    for (int j = x; j < x+w; ++j)
    {
      Position pos = Position(j,i,plane);
      set(pos);
    }
}

void FogMap::setRange(const Position& pos, s16 range)
{
  for (int i = -range; i <= range; ++i)
  {
    for (int j = -range; j <= range; ++j)
    {
      if (range == 1 || (!((i == -range || i == range) && (j == -range || j == range))))
      {
        Position npos = pos.relative(j, i);
        set(npos);
      }
    }
  }
}



Player::Player(Game *game, std::string name, const Wizard* wizard, PlayerColor color, const Race* race, u16 mapWidth, u16 mapHeight) :
g(game), wizard(wizard), race(race), color(color), name(name), fogMap(new FogMap(this, mapWidth, mapHeight)), combat(nullptr), spellBook(SpellBook(*this)), relations(this, game),
castingSkillCounter(0), castingSkillGained_(0), availableMana(0)
{
  
  goldPool = 5000;
  manaPool = 5000;
  taxRate = 1;
  alive = true;
}

void Player::updateCities() { for (auto* c : cities) g->cityMechanics.updateValues(c); }
void Player::growCities() { for (auto* c : cities) g->cityMechanics.growCity(c); }

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

const TaxRate& Player::getTaxRate()
{
  //TODO: manage
  static const TaxRate rate = {0.5f, 0.2f};
  return rate;
}

City* Player::cityWithFortress() const
{
  for (auto c : cities)
    if (c->hasBuilding(Building::MAGE_FORTRESS))
      return c;
  
  return nullptr;
}

City* Player::cityWithSummoningCircle() const
{
  for (auto c : cities)
    if (c->hasBuilding(Building::SUMMONING_CIRCLE))
      return c;
  
  return nullptr;
}

s32 Player::castingSkillBase() const { return g->playerMechanics.computeBaseCastingSkill(this) + castingSkillGained_; }
s32 Player::castingSkill() const { return castingSkillBase() + g->playerMechanics.computeBonusCastingSkill(this); }
s32 Player::researchPoints() const { return g->spellMechanics.actualResearchGain(this, spellBook.getCurrentResearch()); }

bool Player::hasSpell(const GlobalSpell* spell) const
{
  for (auto s : spells)
    if (s.spell == spell)
      return true;
  return false;
}


s16 Player::globalSkillSpellsCount(const Unit* u) const
{
  u32 count = static_cast<u32>(count_if(spells.begin(), spells.end(), [&](const SpellCast& cast) {
    const Spell* spell = cast.spell;
    return spell->type == SpellType::GLOBAL_SKILL;
  }));
  
  return count;
}

const SkillGlobalSpell* Player::nthGlobalSkillSpell(u16 i, const Unit* u) const
{
  for (auto& cast : spells)
  {
    const Spell* spell = cast.spell;
    if (spell->type == SpellType::GLOBAL_SKILL)
    {
      if (i > 0) { --i; continue; }
      return static_cast<const SkillGlobalSpell*>(spell);
    }
  }
  
  return static_cast<const SkillGlobalSpell*>(spells.front().spell); // TODO: hacky and bad practice, the whole management should be refactored
}



