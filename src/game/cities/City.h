#ifndef _CITY_H_
#define _CITY_H_

#include "common/Common.h"
#include "game/spells/Spells.h"

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
  
  value_t population;
  value_t maxPopulation;
  value_t growthRate;
  
  Position position;
  
  std::set<const Building*> buildings;
  const Productable* production;
  value_t productionPool;
  
  value_t reservedPopulation;
  value_t workers;
  value_t unrest;
  value_t farmers;
  
  value_t necessaryFood;
  value_t food;
  
  value_t work;
  
  Upkeep upkeep;
  
  value_t gold;
  
  value_t mana;
  
  value_t knowledge;
  
  mutable bool revalidateLayout; // TODO: should be here?
  
  CityPlacement placement;
  
  bool isStillOutpost;
  
  cast_list spells;
  

public:
  City(Player *owner, std::string name, value_t population, Position position);
  
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
  value_t getKnowledge() const { return knowledge; }
  value_t getWork() const { return work; }
  
  value_t getPopulationInThousands() const { return getPopulation() / 1000; }
  value_t getPopulation() const { return population; }
  value_t getMaxPopulation() const { return maxPopulation; }
  
  Player* getOwner() const { return owner; }
  
  value_t sightRange() { return 2; } // TODO
  
  bool hasPlacement(CityPlacement flag) const { return (placement & flag) != 0; }
  void setPlacement(CityPlacement flag) { placement = static_cast<CityPlacement>(placement | flag); }
  
  void addBuilding(const Building *b) { buildings.insert(b); revalidateLayout = true; }
  bool hasBuilding(const Building *b) const { return buildings.find(b) != buildings.end(); }
  const std::set<const Building*>& getBuildings() { return buildings; }
  
  void addSpell(const SpellCast spell) { spells.push_back(spell); }
  bool hasSpell(const CitySpell* spell) const {
    return std::find_if(spells.begin(), spells.end(), [spell](const SpellCast& cast) { return cast.spell == spell; }) != spells.end();
  }
  void removeSpell(const SpellCast& cast) {
    for (auto it = spells.begin(); it != spells.end(); ++it) if (&cast == &(*it)) spells.erase(it);
  }
  const cast_list& getSpells() { return spells; }
  
  const Race* const race;
  
  friend class CityMechanics;
  friend class CityLayout;
  friend class CityView;
};

#endif
