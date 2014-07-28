#ifndef _ARMY_H_
#define _ARMY_H_

#include "Common.h"

#include <list>
#include <unordered_set>

class MovementEffect;
class Unit;
class Route;
class Player;

typedef std::unordered_set<const MovementEffect*> movement_list;
typedef std::list<Unit*> unit_list;

class Army
{

private:
  Position position;
  bool isPatrol;

  Player *owner;
  unit_list units;
  
  movement_list movementType;
  
  void updateMovementType();

  Route* route;

public:
  Army(Player* owner, std::initializer_list<Unit*> units = {});
  
  s16 sightRange();
  
  s16 availableMoves();
  void resetMoves();
  
  const movement_list& getMovementType() { return movementType; }
  bool hasMovement(const MovementEffect& movement) const { return movementType.find(&movement) != movementType.end(); }
  
  Unit* firstSelected() { return !units.empty() ? units.front() : nullptr; }
  void merge(Army* army);
  
  void patrol();
  void unpatrol() { isPatrol = false; }
  bool isPatrolling() const { return isPatrol; }
  bool isPlaced() const { return position.x != -1; }
  const Position& getPosition() const { return position; }
  void setPosition(const Position& position) { this->position = position; }
  
  void add(Unit* unit);
  Unit* remove(Unit* unit);
  
  const size_t size() const { return units.size(); }
  Unit* get(u16 index) const { return *std::next(units.begin(), index); }
  const unit_list& getUnits() { return units; }
  
  Player* getOwner() const { return owner; }
  
  void turnBegin();
  
  Route* getRoute() { return route; }
  const Route* getRoute() const { return route; }
  void setRoute(Route* route);
  void clearRoute();
  
  typedef std::list<Unit*>::iterator iterator;
  typedef std::list<Unit*>::const_iterator const_iterator;
  iterator begin() { return units.begin(); }
  iterator end() { return units.end(); }
  const_iterator begin() const { return units.begin(); }
  const_iterator end() const { return units.end(); }
};

#endif
