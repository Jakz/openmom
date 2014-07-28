#ifndef _CITY_H_
#define _CITY_H_

#include "Common.h"
#include "Spells.h"

#include <string>
#include <set>
#include <list>

enum class BuildingID : u8;

class Player;
class Race;
class Productable;
class Building;

enum CityPlacement : u16
{
  CITY_BY_RIVER,
  CITY_BY_SEA,
  CITY_BY_HILL,
  CITY_BY_MOUNTAIN
};

class City
{
private:
  Player* owner;
  
  const std::string name;
  
  u16 population;
  u16 maxPopulation;
  u16 growthRate;
  
  Position position;
  
  std::set<const Building*> buildings;
  const Productable* production;
  s16 productionPool;
  
  s16 reservedPopulation;
  s16 workers;
  s16 unrest;
  s16 farmers;
  
  s16 necessaryFood;
  s16 food;
  
  s16 work;
  
  Upkeep upkeep;
  
  s16 gold;
  
  s16 mana;
  
  s16 knowledge;
  
  mutable bool revalidateLayout; // TODO: should be here?
  
  CityPlacement placement;
  
  bool isStillOutpost;
  
  cast_list spells;
  

public:
  City(Player *owner, std::string name, u16 population, Position position);
  
  const std::string& getName() const { return name; }
  u16 tileSize() const
  {
    u16 sizes[] = {5000,9000,13000,17000};
    for (int i = 0; i < 4; ++i) if (population < sizes[i]) return i;

    return 4;
  }
  
  const Position& getPosition() const { return position; }
  
  void setProductable(const Productable* productable) { this->production = productable; }
  const Productable* getProductable() const { return production; }
  
  bool isOutpost() const { return isStillOutpost; }
  Upkeep getUpkeep() const { return upkeep; }
  Upkeep getProduction() const { return Upkeep(gold, mana, food); }
  s32 getKnowledge() const { return knowledge; }
  
  u16 getMaxPopulation() const { return maxPopulation; }
  
  Player* getOwner() { return owner; }
  
  u16 sightRange() { return 2; } // TODO
  
  bool hasPlacement(CityPlacement flag) const { return (placement & flag) != 0; }
  void setPlacement(CityPlacement flag) { placement = static_cast<CityPlacement>(placement | flag); }
  
  void addBuilding(const Building *b) { buildings.insert(b); }
  bool hasBuilding(const Building *b) const { return buildings.find(b) != buildings.end(); }
  const std::set<const Building*>& getBuildings() { return buildings; }
  
  void addSpell(const SpellCast spell) { spells.push_back(spell); }
  bool hasSpell(const CitySpell* spell) const {
    return std::find_if(spells.begin(), spells.end(), [&](const SpellCast cast) { return cast.spell == spell; }) != spells.end();
  }
  void removeSpell(const Spell* spell, const Player* player) { /* TODO const problems std::remove_if(spells.begin(), spells.end(), [&](const SpellCast& cast){return cast.spell == spell && cast.player == player; });*/ }
  const cast_list& getSpells() { return spells; }
  
  const Race& race;
  
  friend class CityMechanics;
  friend class CityLayout;
};

#endif
