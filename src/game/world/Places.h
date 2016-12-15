#ifndef _PLACES_H_
#define _PLACES_H_

#include "common/Common.h"

#include <vector>

class Player;
class Army;

class ManaNode
{
private:
  s16 mana;
  bool warped;
  bool meldGuardianSpirit;

public:
  const School school;

  Player* owner;
  std::vector<Position> auras;

  Army* guards;

  ManaNode(School school, s16 mana = 0) : school(school), owner(nullptr), guards(nullptr), mana(mana), warped(false), meldGuardianSpirit(false) { }
  
  s16 getMana() const { return mana; }
  bool isWarped() const { return warped; }
  bool isMeldByGuardianSprit() const { return meldGuardianSpirit; }
};

class Place
{
public:
  PlaceType type;
  Army *army;
  bool isWeak;

  Place(PlaceType type, bool isWeak) : type(type), isWeak(isWeak), army(nullptr) { }
};


#endif
