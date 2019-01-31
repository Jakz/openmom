//
//  Player.h
//  OpenMoM
//
//  Created by Jack on 7/19/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "common/Common.h"
#include "Diplomacy.h"
#include "Spells.h"
#include "SpellBook.h"
#include "Army.h"

#include <list>
#include <string>
#include <unordered_set>

class FogMap
{
private:
  std::unique_ptr<bool[]> map;
  const Size size;
  Player* const player;
  
  bool unsafeGet(const Position& p) const { return map[p.plane*(size.w*size.h) + size.w*p.y + p.x]; }
  void unsafeSet(const Position& p) { map[p.plane*(size.w*size.h) + size.w*p.y + p.x] = true; }
  
public:
  FogMap(Player* player, u16 w, u16 h);
  
  bool get(Position position) const;
  inline void set(Position position);
  
  void setRect(s16 x, s16 y, s16 w, s16 h, Plane plane);
  void setRange(const Position& position, s16 range);
  
  ~FogMap() { }
};

struct TaxRate
{
  float goldPerCitizen;
  float unrestPercent;
};

class PlayerInterface
{
public:
  virtual void selectAll() = 0;
  virtual Army* getSelectedArmy() const = 0;
  virtual const unit_list& getSelectedUnits() const = 0;
  
  virtual void push(anims::Animation* animation) = 0;
  virtual void send(msgs::Message* message) = 0;
  virtual s16 selectedCount() const = 0;
  virtual void discoverTile(const Position& position) = 0;
  virtual void setSpellTarget(Target target) = 0;
  virtual void moveCombatUnit(combat::CombatUnit* unit) = 0;
};

class CombatPlayerInterface
{
public:
  virtual void combatTurnBegun() = 0;
  virtual void combatTurnEnded() = 0;
};

using item_vault_t = std::array<std::unique_ptr<const items::Item>, 4>;

class Player : public PlayerInterface, public CombatPlayerInterface
{
protected:
  std::list<City*> cities;
  std::list<Army*> armies;
  std::vector<Hero*> heroes;
  /* mana nodes owned by the player */
  std::list<ManaNode*> nodes;
  /* global spells of the player */
  cast_list spells;
  
  item_vault_t _vault;

  std::unordered_set<const Retort*> retorts;

  SpellBook spellBook;
  Relations relations;
  const combat::Combat* combat;
  mutable FogMap *fogMap;

  value_t taxRate; // TODO: maybe enum

  value_t manaRatios[3];
  value_t fame;

  value_t goldGain, manaGain, foodGain;
  value_t goldUpkeep, manaUpkeep, foodUpkeep;
  value_t goldPool, manaPool;

  value_t researchGain;
  
  value_t castingSkillCounter;
  value_t castingSkillGained_;
  value_t availableMana;

  bool alive;

  Game *g;
  
public:
  Player(Game *game, const std::string& name, const Wizard* wizard, PlayerColor color, const Race* race, u16 mapWidth, u16 mapHeight);

  bool isAlive() const { return alive; }
  
  void spendGold(s32 amount) { goldPool -= amount; }
  
  value_t totalGoldPool() const { return goldPool; }
  value_t totalManaPool() const { return manaPool; }
  
  value_t goldDelta() const { return goldGain - goldUpkeep; }
  value_t manaDelta() const { return manaRatios[0] - manaUpkeep; }
  value_t foodDelta() const { return foodGain - foodUpkeep; }
  
  value_t castingSkillBase() const;
  value_t castingSkill() const;
  value_t castingSkillGained() const { return castingSkillGained_; }
  value_t manaRatio(size_t index) const { return manaRatios[index]; }
  value_t getAvailableMana() const { return availableMana; }
  value_t getManaGain() const { return manaGain; }
  
  item_vault_t& vault() { return _vault; }
  
  const TaxRate& getTaxRate();
  value_t getFame() const { return fame; }
  
  void alchemy(value_t gold, value_t mana) { goldPool += gold; manaPool -= mana; }
  
  const combat::Combat* getCombat() const { return combat; }
  void setCombat(combat::Combat* combat) { this->combat = combat; }
  
  value_t baseResearchPoints() const { return researchGain + manaRatios[1]; }
  value_t researchPoints() const;
  
  void setManaRatios(value_t m, value_t r, value_t s) { manaRatios[0] = m; manaRatios[1] = r; manaRatios[2] = s; }
  
  void combatCast(const Spell* spell) { spellBook.combatCast(spell); }

  void add(City* city) { cities.push_back(city); }
  void add(Army* army) { armies.push_back(army); }
  void add(Hero* hero) { heroes.push_back(hero); }
  void add(ManaNode* node) { nodes.push_back(node); } // TODO: here we store a raw pointer of a unique_ptr allocated object
  void add(const SpellCast& spell) { spells.push_back(spell); }
  const cast_list& getSpells() const { return spells; }
  
  void updateCities();
  void growCities();
  
  void fameGain(s32 fame) { this->fame += fame; }
  void fameLoss(s32 fame) { this->fame -= fame; }

  void turnBeginArmies();
  void refreshArmies();
  void remove(Army* army) { armies.remove(army); }
  
  SpellBook* book() { return &spellBook; }
  const SpellBook* book() const { return &spellBook; }
  
  Relations* getRelations() { return &relations; }
  
  const std::vector<Hero*>& getHeroes() const { return heroes; }
  const std::list<Army*>& getArmies() const { return armies; }
  const std::list<City*>& getCities() const { return cities; }
  
  count_t globalSkillSpellsCount(const Unit* u) const;
  const SkillGlobalSpell* nthGlobalSkillSpell(u16 i, const Unit* u) const;
  bool hasSpell(const GlobalSpell* spell) const;

  
  FogMap* fog() const { return fogMap; }
  
  Game *game() const { return g; }
  
  
  City* cityWithFortress() const;
  City* cityWithSummoningCircle() const;
  
  bool hasRetort(const std::string& ident) const {
    return std::find_if(retorts.begin(), retorts.end(), [&ident](const Retort* retort) { return retort->identifier == ident; }) != retorts.end();
  }
  bool hasMastery(School school) const
  {
    return (school == School::NATURE && hasRetort("nature_mastery")) ||
      (school == School::CHAOS && hasRetort("chaos_mastery")) ||
      (school == School::SORCERY && hasRetort("sorcery_mastery"));
  }
  
  const std::string name;
  const Wizard* wizard;
  const PlayerColor color;
  const Race* const race;
    
  friend class PlayerMechanics;
};

#endif
