#ifndef _EVENT_MECHANICS_H_
#define _EVENT_MECHANICS_H_

#include "Common.h"
#include "Util.h"

#include <memory>

class Game;
class Army;
class Player;

class Event
{
public:
  const enum Type
  {
    PLAYER_POSITIVE,
    PLAYER_NEGATIVE,
    CITY_POSITIVE,
    CITY_NEGATIVE,
    GLOBAL
  } type;
  
  const I18 name;
  
  Event(I18 name, Type type) : name(name), type(type) { }
};

class Events
{
public:
  static const Event *BAD_MOON, *RED_CONJUNCTION, *GREEN_CONJUNCTION, *BLUE_CONJUNCTION, *DEPLETION, *DIPLOMATIC_MARRIAGE, *DISJUNCTION;
  static const Event *DONATION, *EARTHQUAKE, *THE_GIFT, *GOOD_MOON, *GREAT_METEOR, *MANA_SHORT, *NEW_MINERALS, *PIRACY, *PLAGUE, *POPULATION_BOOM, *REBELLION;
};


class EventMechanics
{
private:
  Game* const g;
  s16 turnStreak;
  
  void resetStreak() { turnStreak = 0; }
  bool willStreakEventEnd()
  {
    ++turnStreak;
    
    if (turnStreak <= 5)
      return false;
    else
    {
      float chance = 0.05f*(turnStreak - 5);
      
      if (Util::chance(chance))
      {
        resetStreak();
        return true;
      }
      else
        return false;
    }
  }

public:
  EventMechanics(Game* game) : g(game), turnStreak(0) { }
  
  Army* tryMercenaryOffer(const Player* player);
  
  const Player* pickVictim(Event::Type type) const;
  const Event* pickRandomEvent() const;
  
  
  
};


#endif
