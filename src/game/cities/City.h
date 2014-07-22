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
  const Race& race;
  
  std::string name;
  
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
  
  bool revalidateLayout; // should be here?
  
  CityPlacement placement;
  
  bool isStillOutpost;
  
  cast_list spells;
  

public:
  City(Player *owner, std::string name, u16 population, Position position);
  
  u16 tileSize()
  {
    u16 sizes[] = {5000,9000,13000,17000};
    for (int i = 0; i < 4; ++i) if (population < sizes[i]) return i;

    return 4;
  }
  
  u16 sightRange() { return 2; } // TODO
  
  bool hasPlacement(CityPlacement flag) { return (placement & flag) != 0; }
  void setPlacement(CityPlacement flag) { placement = static_cast<CityPlacement>(placement | flag); }
  
  void addBuilding(const Building *b) { buildings.insert(b); }
  bool hasBuilding(const Building *b) { return buildings.find(b) != buildings.end(); }
  bool hasBuilding(const BuildingID b);
  const std::set<const Building*>& getBuildings() { return buildings; }
  
  void addSpell(const SpellCast spell) { spells.push_back(spell); }
  bool hasSpell(const CitySpell& spell) {
    return std::find_if(spells.begin(), spells.end(), [&](const SpellCast cast) { return &cast.spell == &spell; }) != spells.end();
  }
  // TODO: remove spell is missing
  const cast_list& getSpells() { return spells; }
};

#endif
