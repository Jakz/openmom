#include "City.h"

#include "game/players/Player.h"
#include "game/cities/Buildings.h"

City::City(Player *owner, std::string name, value_t population, Position position) :
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
