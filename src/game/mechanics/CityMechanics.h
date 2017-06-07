#ifndef _CITY_MECHANICS_H_
#define _CITY_MECHANICS_H_

#include "City.h"
#include "Common.h"

#include <map>

class Building;
class Player;
class City;
class Game;
class RaceUnitSpec;
class Tile;

enum class RaceID: u8;

namespace std
{
  template<> struct hash<RaceID> { std::size_t operator()(const RaceID& k) const { return static_cast<u8>(k); } };
}

class CityMechanics
{
private:
  static const std::multimap<const Building*, const Building*> buildingDependsOn;
  static const std::multimap<RaceID, const Building*> disallowedBuildingsByRace;
  static const std::map<const Building*, const Building*> buildingReplacementMap;
  
  Game* const game;
  
protected:
  
public:
  CityMechanics(Game* game) : game(game) { }
  
  bool isBuildingCurrentlyReplaced(const City* city, const Building* building);
  
  bool isBuildingAllowed(const City* city, const Building* building);
  bool isBuildingAllowedForTerrain(const City *city, const Building* building);
  bool isBuildingAllowedForRace(const City* city, const Building* building);
  bool isUnitAllowed(const City* city, const RaceUnitSpec* unit);
  std::list<const Productable*> itemsUnlockedByBuilding(const Building* building, const City *city);
  const std::list<const RaceUnitSpec*> availableUnits(const City* city);
  const std::list<const Building*> availableBuildings(const City* city);
  
  void lambdaOnCitySurroundings(const City* city, const std::function<void(Tile*)>& functor);
  
  s16 countSurroundTileType(const City* city, TileType type);
  s16 countSurroundResource(const City* city, Resource type);
  s16 countSurroundManaNode(const City* city, School type);
  float resourceBonus(const City* city, Resource resource, float value);
  
  s16 computeInitialPopulation(const Player* player, const Position& position) { return 300; } // TODO: real behavior
  s16 computeInitialPopulationGrowth(const City* city);
  
  s16 computeFood(const City* city) { return 2*(city->population/1000) - city->necessaryFood; } // TODO: real behavior
  Upkeep computeUpkeep(const City* city);
  s16 computeGold(const City* city);
  s16 computeProductionBonus(const City* city);
  s16 computeProduction(const City* city);
  s16 computeMana(const City* city);
  s16 computeKnowledge(const City* city);
  
  s16 computeMaxPopulationForTile(const Tile* tile);
  s16 computeMaxPopulation(const City* city);
  s16 computeUnrest(const City* city);
  
  void partitionPopulation(City* city);
  
  s16 computeGrowthRate(const City* city);
  
  s16 baseGold(const City* city);
  s16 baseProduction(const City* city);
  s16 baseGrowthRate(const City* city);
  
  u16 turnsRequiredForProduction(const City* city);
  
  bool canCityBeBuiltOnTile(Tile* tile);
  
  void updateValues(City* city)
  {
    if (city->isStillOutpost) return;
    
    city->maxPopulation = computeMaxPopulation(city);
    city->growthRate = computeGrowthRate(city);
    city->necessaryFood = city->population/1000;
    partitionPopulation(city);
    city->food = computeFood(city);
    city->upkeep = computeUpkeep(city);
    city->gold = computeGold(city);
    city->work = computeProduction(city);
    city->mana = computeMana(city);
    city->knowledge = computeKnowledge(city);
    
    updateProduction(city);
  }
  
  void buyProduction(City* city);
  bool isProductionBuyable(const City* city);
  void updateProduction(City* city);
  
  void growCity(City* city);

};

#endif
