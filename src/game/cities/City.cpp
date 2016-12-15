#include "City.h"

#include "Player.h"
#include "Buildings.h"

City::City(Player *owner, std::string name, u16 population, Position position) :
  owner(owner), name(name), population(population), position(position), race(owner->race), production(nullptr), isStillOutpost(population < 1000)
{
  if (population < 1000)
    isStillOutpost = true;
  
  /* TODO: calculate max population */
  maxPopulation = 25;
  owner->add(this);
  
  production = Building::HOUSING;
  productionPool = 0;
}

