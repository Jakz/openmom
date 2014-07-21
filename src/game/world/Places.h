#ifndef _PLACES_H_
#define _PLACES_H_

#include "Common.h"

#include <vector>

class Player;
class Army;

class ManaNode
{
  public:
    const School school;
    s16 mana;
  
    Player* owner;
    std::vector<Position> auras;
  
    Army* guards;
  
    ManaNode(School school, s16 mana = 0) : school(school), owner(nullptr), guards(nullptr), mana(mana) { }
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
