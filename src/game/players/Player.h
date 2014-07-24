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
#include "Spells.h"
#include "SpellBook.h"

#include <list>
#include <string>
#include <unordered_set>

class Race;
class Game;
class City;
class Army;
class Hero;
class ManaNode;
class Unit;
class Combat;

class FogMap
{
private:
  bool*** map;
  const u16 w, h;
  Player *player;
  
public:
  FogMap(u16 w, u16 h);
  
  bool get(const Position& position) const;
  void set(const Position& position);
  
  void setRect(s16 x, s16 y, s16 w, s16 h, Plane plane);
  void setRange(const Position& position, s16 range);
  
  ~FogMap();
};

namespace std
{
  template<>
  struct hash<TraitID>
  {
    std::size_t operator()(const TraitID& k) const { return static_cast<u16>(k); }
  };
}

class Player
{
protected:
  std::list<City*> cities;
  std::list<Army*> armies;
  std::list<Hero*> heroes;
  std::list<ManaNode*> nodes;
  std::list<const SpellCast> spells;

  std::unordered_set<TraitID> traits;

  SpellBook spellBook;
  // Relations TODO
  const Combat* combat;
  mutable FogMap *fogMap;

  u8 taxRate; // TODO: maybe enum

  s32 manaRatios[3];
  s32 fame;

  s32 goldGain, manaGain, foodGain;
  s32 goldUpkeep, manaUpkeep, foodUpkeep;
  s32 goldPool, manaPool;

  s32 researchGain;
  
  s32 castingSkillCounter;
  s32 castingSkillGained_;
  s32 availableMana;

  bool alive;

  Game *g;
  
public:
  Player(Game *game, std::string name, const Wizard& wizard, PlayerColor color, const Race& race, u16 mapWidth, u16 mapHeight);

  void spendGold(s32 amount) { goldPool -= amount; }
  
  u8 getTaxRate() { return taxRate; }
  s32 totalGoldPool() { return goldPool; }
  
  s32 goldDelta() const { return goldGain - goldUpkeep; }
  s32 manaDelta() const { return manaRatios[0] - manaUpkeep; }
  s32 foodDelta() const { return foodGain - foodUpkeep; }
  
  s32 castingSkillBase() const;
  s32 castingSkill() const;
  s32 castingSkillGained() const { return castingSkillGained_; }
  s32 manaRatio(u8 index) const { return manaRatios[index]; }
  
  const Combat* getCombat() const { return combat; }
  void setCombat(Combat* combat) { this->combat = combat; }
  
  s32 baseResearchPoints() const { return researchGain + manaRatios[1]; }
  s32 researchPoints() const;
  
  void setManaRatios(s32 m, s32 r, s32 s) { manaRatios[0] = m; manaRatios[1] = r; manaRatios[2] = s; }
  
  void combatCast(const Spell* spell) { spellBook.combatCast(spell); }


  void add(City* city) { cities.push_back(city); }
  void add(Army* army) { armies.push_back(army); }
  void add(Hero* hero) { heroes.push_back(hero); }
  void add(ManaNode* node) { nodes.push_back(node); }
  void add(const SpellCast& spell) { spells.push_back(spell); }
  const cast_list& getSpells() const { return spells; }
  
  void updateCities() { for (auto* c : cities) return; } // TODO game.cityMechanics.updateValues(c);
  void growCities() { for (auto* c : cities) return; } // TODO game.cityMechanics.growCity(c);
  
  void fameGain(s32 fame) { this->fame += fame; }
  void fameLoss(s32 fame) { this->fame -= fame; }

  void turnBeginArmies();
  void refreshArmies();
  void remove(Army* army) { armies.remove(army); }

  void selectAll() { } // TODO
  s16 selectedCount() const { return 0; } // TODO
  
  const SpellBook* book() const { return &spellBook; }
  
  s16 globalSkillSpellsCount(const Unit* u) const;
  const SkillGlobalSpell* nthGlobalSkillSpell(u16 i, const Unit* u) const;

  
  FogMap* fog() const { return fogMap; }
  virtual void discoverTile(const Position& position) = 0;
  
  Game *game() const { return g; }
  
  
  City* cityWithFortress();
  City* cityWithSummoningCircle();
  
  bool hasTrait(TraitID trait) const { return traits.find(trait) != traits.end(); }
  bool hasMastery(School school) const { return (school == NATURE && hasTrait(TraitID::NATURE_MASTERY)) || (school == CHAOS && hasTrait(TraitID::CHAOS_MASTERY)) || (school == SORCERY && hasTrait(TraitID::SORCERY_MASTERY)); }
  
  const std::string name;
  const Wizard& wizard;
  const PlayerColor color;
  const Race& race;
  
  static constexpr const float TAX_RATES[] = {0.5f,1.0f,1.5f,2.0f,2.5f,3.0f};

};

#endif
