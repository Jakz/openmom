#ifndef _PLACES_H_
#define _PLACES_H_

#include "common/Common.h"

#include <vector>

class Player;
class Army;

class Place
{
public:
  PlaceType type;
  Army *army;
  bool isWeak;
  bool cleared;

  Place(PlaceType type, bool isWeak) : type(type), isWeak(isWeak), army(nullptr), cleared(false) { }
  Place(PlaceType type) : Place(type, false) { }
    
  virtual bool isNode() const { return false; }
};

class ManaNode : public Place
{
private:
  s16 mana;
  bool warped;
  bool meldGuardianSpirit;
  
public:
  const School school;
  
  Player* owner;
  std::vector<Position> auras;
  
  ManaNode(School school, s16 mana = 0) : Place(PlaceType::MANA_NODE), school(school), owner(nullptr), mana(mana), warped(false), meldGuardianSpirit(false) { }
  
  s16 getMana() const { return mana; }
  bool isWarped() const { return warped; }
  bool isMeldByGuardianSprit() const { return meldGuardianSpirit; }
  
  bool isNode() const override { return true; }
};

#endif
