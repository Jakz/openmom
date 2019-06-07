#ifndef _ARMY_H_
#define _ARMY_H_

#include "common/Common.h"
#include "UnitEffects.h"

#include <list>
#include <unordered_set>
#include <memory>

class Unit;
namespace pathfind { class Route; }
class Player;

class movement_list
{
private:
  std::unordered_set<MovementType, enum_hash> data;
  
public:
  using value_type = decltype(data)::value_type;
  using iterator = decltype(data)::iterator;
  using const_iterator = decltype(data)::const_iterator;

  movement_list() = default;
  movement_list(const decltype(data)& data) : data(data) { }
  movement_list(const movement_list& other) : data(other.data) { }
  movement_list(const std::vector<const MovementEffect*>& effects) {
    for (const auto* e : effects) data.insert(e->subType());
  }
  
  movement_list& operator+=(std::initializer_list<MovementType> types) {
    data.insert(types);
    return *this;
  }

  size_t size() const { return data.size(); }
  
  void insert(iterator it, MovementType effect) { data.insert(it, effect); }
  void add(MovementType effect) { data.insert(effect); }
    
  const_iterator begin() const{ return data.begin(); }
  const_iterator end() const { return data.end(); }

  bool operator==(const movement_list& other) const { return data == other.data; }

  bool contains(MovementType type) const { return data.find(type) != data.end(); }
};

class movement_list_group
{
public:
  using predicate_t = std::function<bool(const movement_list&)>;
  
  inline predicate_t simplePredicate(MovementType effect) const
  {
    return [effect] (const movement_list& l) { return l.contains(effect); };
  }
  
private:
  //TODO: was const movement_list
  std::vector<movement_list> data;

public:
  using value_type = decltype(data)::value_type;
  
  movement_list_group(size_t size) { data.reserve(size); }
  
  using iterator = decltype(data)::iterator;
  using const_iterator = decltype(data)::const_iterator;
  size_t size() const { return data.size(); }
  const_iterator begin() const { return data.begin(); }
  const_iterator end() const { return data.end(); }
  iterator begin() { return data.begin(); }
  iterator end() { return data.end(); }
  
  void push_back(const movement_list&& list) { data.push_back(list); }
  
  bool all_of(const predicate_t& predicate) const { return std::all_of(data.begin(), data.end(), predicate); }
  bool any_of(const predicate_t& predicate) const { return std::any_of(data.begin(), data.end(), predicate); }
  bool none_of(const predicate_t& predicate) const { return std::none_of(data.begin(), data.end(), predicate); }
  
  bool all_of(MovementType e) const { return all_of(simplePredicate(e)); }
  bool any_of(MovementType e) const { return any_of(simplePredicate(e)); }
  bool none_of(MovementType e) const { return none_of(simplePredicate(e)); }

};

typedef std::list<Unit*> unit_list;

class Army
{
  using Route = pathfind::Route;

private:
  Position position;
  bool isPatrol;

  Player *owner;
  unit_list units;
  
  movement_list movementType;
  
  void updateMovementType();

  std::unique_ptr<Route> route;

public:
  Army(Player* owner, std::initializer_list<Unit*> units = {});
  ~Army();
  
  value_t sightRange();
  
  value_t availableMoves();
  void resetMoves();
  
  const movement_list& getMovementType() { return movementType; }
  bool hasMovement(MovementType movement) const { return movementType.contains(movement); }
  
  const Unit* firstSelected() { return !units.empty() ? units.front() : nullptr; }
  void merge(Army* army);
  
  void patrol();
  void unpatrol() { isPatrol = false; }
  bool isPatrolling() const { return isPatrol; }
  bool isPlaced() const { return position.x != -1; } //TODO: maybe useless?
  const Position& getPosition() const { return position; }
  void setPosition(const Position& position) { this->position = position; }
  
  void add(Unit* unit);
  Unit* remove(Unit* unit);
  
  bool isFull() const { return size() == 9; } //TODO: magic number
  size_t size() const { return units.size(); }
  Unit* get(size_t index) const { return *std::next(units.begin(), index); }
  const unit_list& getUnits() const { return units; }
  const unit_list getUnits(std::function<bool(const Unit*)> predicate) const;
  
  bool any_of(const std::function<bool(const Unit*)> predicate) const {
    return std::any_of(units.begin(), units.end(), predicate);
  }
  
  Player* getOwner() const { return owner; }
  
  void turnBegin();
  
  //Route* getRoute() { return route; }
  const std::unique_ptr<Route>& getRoute() const { return route; }
  void setRoute(Route* route);
  void clearRoute();
  
  using iterator = unit_list::iterator;
  using const_iterator = unit_list::const_iterator;

  iterator begin() { return units.begin(); }
  iterator end() { return units.end(); }
  const_iterator begin() const { return units.begin(); }
  const_iterator end() const { return units.end(); }
};

#endif
