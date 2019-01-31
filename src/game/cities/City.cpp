#include "City.h"

#include "game/players/Player.h"
#include "game/cities/Buildings.h"

City::City(Player* owner, const Race* race, const std::string& name, value_t population, Position position) :
  owner(owner), name(name), population(population), position(position), race(race), production(nullptr), isStillOutpost(population < 1000)
{
  if (population < 1000)
    isStillOutpost = true;

  /* TODO: calculate max population */
  maxPopulation = 25;

  if (owner)
    owner->add(this);

  production = Building::HOUSING;
  productionPool = 0;
}

City::City(Player*owner, const std::string& name, value_t population, Position position) : City(owner, owner->race, name, population, position) { }
