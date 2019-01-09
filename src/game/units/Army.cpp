#include "Army.h"

#include "Unit.h"
#include "Player.h"
#include "Game.h"
#include "Pathfind.h"

using namespace std;

Army::Army(Player* owner, initializer_list<Unit*> units) : owner(owner), isPatrol(false), position(Position(-1, -1, ARCANUS)), route(nullptr)
{
  owner->add(this);
  
  for (auto u : units)
    add(u);
  
  if (units.begin() != units.end())
    updateMovementType();
}

const unit_list Army::getUnits(std::function<bool(const Unit*)> predicate) const
{
  unit_list units;
  std::copy_if(this->units.begin(), this->units.end(), std::back_inserter(units), predicate);
  return units;
}

void Army::setRoute(Route* route)
{
  this->route.reset(route);
  if (route)
    this->route->setArmy(this);
}

void Army::clearRoute() { setRoute(nullptr); }


void Army::updateMovementType()
{
  movementType = owner->game()->mapMechanics.movementTypeOfArmy(units);
}


value_t Army::sightRange()
{
  auto it = max_element(units.begin(), units.end(), [](const Unit* u1, const Unit* u2) { return u1->getProperty(Property::SIGHT) < u2->getProperty(Property::SIGHT); });
  return it != units.end() ? (*it)->getProperty(Property::SIGHT) : 0;
}

value_t Army::availableMoves()
{  
  value_t min = std::numeric_limits<value_t>::max();
  for (auto u : units)
  {
    value_t umoves = u->getAvailableMoves();
    min = std::min(min, umoves);
  }
  
  return min;
}

void Army::resetMoves()
{
  for (auto u : units) u->resetMoves();
}

void Army::merge(Army *army)
{
  units.insert(units.end(), army->units.begin(), army->units.end());
  
  owner->remove(army);
  owner->selectAll();
  
  updateMovementType();
}

void Army::patrol()
{
  isPatrol = true;
  owner->fog()->setRange(position, sightRange()+1);
}

void Army::add(Unit* unit)
{
  units.push_back(unit);
  unit->setArmy(this); // TODO: this is not set in merge, will it create problems?
  
  // if unit is an hero and army has an owner then
  // add it also to heroes of player
  // TODO: for now it's here, then we'll see
  if (unit->spec->productionType() == Productable::Type::HERO)
    owner->add(static_cast<Hero*>(unit));
  
  updateMovementType();
}

Unit* Army::remove(Unit *unit)
{
  units.remove(unit);
  unit->setArmy(nullptr);
  updateMovementType();
  return unit;
}

void Army::turnBegin()
{
  for (auto u : units) u->turnBegin();
}
