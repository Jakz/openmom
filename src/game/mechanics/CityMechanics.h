#ifndef _CITY_MECHANICS_H_
#define _CITY_MECHANICS_H_

#include "City.h"
#include "common/Common.h"

#include <map>

class Building;
class Player;
class City;
class Game;
class RaceUnitSpec;
class Tile;

enum class RaceID: u8;

struct tilecount_t
{
  value_t _count;
  value_t _shared;

  tilecount_t(value_t count, value_t shared) :_count(count), _shared(shared) { }
  tilecount_t() : tilecount_t(0,0) { }

  void incr(bool shared) { ++_count; if (shared) ++_shared; }
  tilecount_t operator+(const tilecount_t& other) { return tilecount_t(_count + other._count, _shared + other._shared); }

  value_t total() const { return _count; }
  value_t shared() const { return _shared; }
  value_t exclusive() const { return _count - _shared; }
};

namespace std
{
  template<> struct hash<RaceID> { std::size_t operator()(const RaceID& k) const { return static_cast<u8>(k); } };
}

class CityMechanics
{
private:
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
  const std::vector<const Building*> availableBuildings(const City* city);
  
  void lambdaOnCitySurroundings(const City* city, const std::function<void(Tile*)>& functor);
  
  tilecount_t countSurroundTileType(const City* city, TileType type);
  tilecount_t countSurroundResource(const City* city, Resource type);
  tilecount_t countSurroundManaNode(const City* city, School type);
  float resourceBonus(const City* city, Resource resource, float value);
  
  value_t computeInitialPopulation(const Player* player, const Position& position) { return 300; } // TODO: real behavior
  value_t computeInitialPopulationGrowth(const City* city);
  
  value_t computeFood(const City* city) { return 2*(city->population/1000) - city->necessaryFood; } // TODO: real behavior
  Upkeep computeUpkeep(const City* city);
  value_t computeGold(const City* city);
  value_t computeProductionBonus(const City* city);
  value_t computeProduction(const City* city);
  value_t computeMagicPower(const City* city);
  value_t computeKnowledge(const City* city);
  
  float getRacialUnrest(const Race* cityRace, const Race* ownerRace);
  value_t computeMaxPopulationForTile(const Tile* tile);
  value_t computeMaxPopulation(const City* city);
  value_t computeUnrest(const City* city);
  
  void partitionPopulation(City* city);
  
  value_t computeGrowthRate(const City* city);
  
  value_t baseGold(const City* city);
  value_t baseProduction(const City* city);
  value_t baseGrowthRate(const City* city);
  
  value_t turnsRequiredForProduction(const City* city);
  
  bool canRoadBeBuiltOnTile(const Tile* tile);
  bool canCityBeBuiltOnTile(const Tile* tile);
  
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
    city->magicPower = computeMagicPower(city);
    city->knowledge = computeKnowledge(city);
    
    updateProduction(city);
  }
  
  void buyProduction(City* city);
  bool isProductionBuyable(const City* city);
  void updateProduction(City* city);
  
  void growCity(City* city);

};

#endif
