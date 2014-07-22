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

class FogMap
{
private:
  bool*** map;
  const u16 w, h;
  Player *player;
  
public:
  FogMap(u16 w, u16 h);
  
  bool get(const Position& position);
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
private:
  std::list<City*> cities;
  std::list<Army*> armies;
  std::list<Hero*> heroes;
  std::list<ManaNode*> nodes;
  std::list<SpellCast> spells;

  std::unordered_set<TraitID> traits;

  // SpellBook TODO
  // Relations TODO
  // Combat TODO
  FogMap *fogMap;

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

  s32 goldDelta() { return goldGain - goldUpkeep; }
  s32 manaDelta() { return manaRatios[0] - manaUpkeep; }
  s32 foodDelta() { return foodGain - foodUpkeep; }
  
  s32 castingSkillBase() { return /*TODO game.playerMechanics.computeBaseCastingSkill(this) +*/ castingSkillGained_; }
  s32 castingSkill() { return castingSkillBase() /*TODO + game.playerMechanics.computeBonusCastingSkill(this)*/; }
  s32 castingSkillGained() { return castingSkillGained_; }
  s32 manaRatio(u8 index) { return manaRatios[index]; }
  
  /* TODO:
   SAGE MASTER: bonus -20% research cost
   CONJURER trait: bonus -25% research cost summon
   NATURE MASTERY trait: bonus -15% research
   CHAOS MASTERY trait: bonus -15% research
   SORCERY MASTERY trait: bonus -15% research
   10% per book >= 8
   
   */
  
  s32 baseResearchPoints() { return researchGain + manaRatios[1]; }
  s32 researchPoints() { return 0; /*TODO game.spellMechanics.actualResearchGain(this, spellBook().currentResearch()); */ }
  
  void setManaRatios(s32 m, s32 r, s32 s) { manaRatios[0] = m; manaRatios[1] = r; manaRatios[2] = s; }
  
  void combatCast(Spell spell) { /* TODO spellBook.combatSpell(spell); */ }


  void add(City* city) { cities.push_back(city); }
  void add(Army* army) { armies.push_back(army); }
  void add(Hero* hero) { heroes.push_back(hero); }
  void add(ManaNode* node) { nodes.push_back(node); }
  void add(const SpellCast& spell) { spells.push_back(spell); }
  
  void updateCities() { for (auto* c : cities) return; } // TODO game.cityMechanics.updateValues(c);
  void growCities() { for (auto* c : cities) return; } // TODO game.cityMechanics.growCity(c);
  
  void fameGain(s32 fame) { this->fame += fame; }
  void fameLoss(s32 fame) { this->fame -= fame; }

  void turnBeginArmies();
  void refreshArmies();
  void remove(Army* army) { armies.remove(army); }

  void selectAll() { } // TODO
  s16 selectedCount() const { return 0; } // TODO
  
  s16 globalSkillSpellsCount(const Unit* u) const;
  const SkillGlobalSpell& nthGlobalSkillSpell(u16 i, const Unit* u) const;

  
  //SpellBook* book() { return spellBook; } TODO
  FogMap* fog() { return fogMap; }
  virtual void discoverTile(const Position& position) = 0;
  
  Game *game() const { return g; }
  
  
  City* cityWithFortress();
  City* cityWithSummoningCircle();
  
  bool hasTrait(TraitID trait) { return traits.find(trait) != traits.end(); }
  bool hasMastery(School school) { return (school == NATURE && hasTrait(TraitID::NATURE_MASTERY)) || (school == CHAOS && hasTrait(TraitID::CHAOS_MASTERY)) || (school == SORCERY && hasTrait(TraitID::SORCERY_MASTERY)); }
  
  const std::string name;
  const Wizard& wizard;
  const PlayerColor color;
  const Race& race;
  
  static constexpr const float TAX_RATES[] = {0.5f,1.0f,1.5f,2.0f,2.5f,3.0f};

};

#endif
