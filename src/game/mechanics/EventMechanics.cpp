#include "EventMechanics.h"

#include "Localization.h"

#include "Player.h"
#include "Game.h"
#include "Army.h"
#include "Unit.h"

using namespace std;

namespace eventimpl
{
  static const Event BAD_MOON = Event(I18::EVENT_BAD_MOON, Event::Type::GLOBAL);
  static const Event RED_CONJUNCTION = Event(I18::EVENT_RED_CONJUNCTION, Event::Type::GLOBAL);
  static const Event GREEN_CONJUNCTION = Event(I18::EVENT_GREEN_CONJUNCTION, Event::Type::GLOBAL);
  static const Event BLUE_CONJUNCTION = Event(I18::EVENT_BLUE_CONJUNCTION, Event::Type::GLOBAL);
  static const Event DEPLETION = Event(I18::EVENT_DEPLETION, Event::Type::CITY_NEGATIVE);
  static const Event DIPLOMATIC_MARRIAGE = Event(I18::EVENT_DIPLOMATIC_MARRIAGE, Event::Type::PLAYER_POSITIVE);
  static const Event DISJUNCTION = Event(I18::EVENT_DISJUNCTION, Event::Type::GLOBAL);
  static const Event DONATION = Event(I18::EVENT_DONATION, Event::Type::PLAYER_POSITIVE);
  static const Event EARTHQUAKE = Event(I18::EVENT_EARTHQUAKE, Event::Type::CITY_NEGATIVE);
  static const Event THE_GIFT = Event(I18::EVENT_THE_GIFT, Event::Type::PLAYER_POSITIVE);
  static const Event GOOD_MOON = Event(I18::EVENT_GOOD_MOON, Event::Type::GLOBAL);
  static const Event GREAT_METEOR = Event(I18::EVENT_GREAT_METEOR, Event::Type::CITY_NEGATIVE);
  static const Event MANA_SHORT = Event(I18::EVENT_MANA_SHORT, Event::Type::GLOBAL);
  static const Event NEW_MINERALS = Event(I18::EVENT_NEW_MINERALS, Event::Type::CITY_POSITIVE);
  static const Event PIRACY = Event(I18::EVENT_PIRACY, Event::Type::PLAYER_NEGATIVE);
  static const Event PLAGUE = Event(I18::EVENT_PLAGUE, Event::Type::CITY_NEGATIVE);
  static const Event POPULATION_BOOM = Event(I18::EVENT_POPULATION_BOOM, Event::Type::CITY_POSITIVE);
  static const Event REBELLION = Event(I18::EVENT_REBELLION, Event::Type::CITY_NEGATIVE);
}

const Event* Events::BAD_MOON = &eventimpl::BAD_MOON;
const Event* Events::RED_CONJUNCTION = &eventimpl::RED_CONJUNCTION;
const Event* Events::GREEN_CONJUNCTION = &eventimpl::GREEN_CONJUNCTION;
const Event* Events::BLUE_CONJUNCTION = &eventimpl::BLUE_CONJUNCTION;
const Event* Events::DEPLETION = &eventimpl::DEPLETION;
const Event* Events::DIPLOMATIC_MARRIAGE = &eventimpl::DIPLOMATIC_MARRIAGE;
const Event* Events::DISJUNCTION = &eventimpl::DISJUNCTION;
const Event* Events::DONATION = &eventimpl::DONATION;
const Event* Events::EARTHQUAKE = &eventimpl::EARTHQUAKE;
const Event* Events::THE_GIFT = &eventimpl::THE_GIFT;
const Event* Events::GOOD_MOON = &eventimpl::GOOD_MOON;
const Event* Events::GREAT_METEOR = &eventimpl::GREAT_METEOR;
const Event* Events::MANA_SHORT = &eventimpl::MANA_SHORT;
const Event* Events::NEW_MINERALS = &eventimpl::NEW_MINERALS;
const Event* Events::PIRACY = &eventimpl::PIRACY;
const Event* Events::PLAGUE = &eventimpl::PLAGUE;
const Event* Events::POPULATION_BOOM = &eventimpl::POPULATION_BOOM;
const Event* Events::REBELLION = &eventimpl::REBELLION;

static const Event* events[] = {
  Events::BAD_MOON,
  Events::RED_CONJUNCTION,
  Events::GREEN_CONJUNCTION,
  Events::BLUE_CONJUNCTION,
  Events::DEPLETION,
  Events::DIPLOMATIC_MARRIAGE,
  Events::DISJUNCTION,
  Events::DONATION,
  Events::EARTHQUAKE,
  Events::THE_GIFT,
  Events::GOOD_MOON,
  Events::GREAT_METEOR,
  Events::MANA_SHORT,
  Events::NEW_MINERALS,
  Events::PIRACY,
  Events::PLAGUE,
  Events::POPULATION_BOOM,
  Events::REBELLION
};

// unique_ptr<Army>
Army* EventMechanics::tryMercenaryOffer(const Player* player)
{
  //TODO: OSG pg 30
  return nullptr;
}

const Player* EventMechanics::pickVictim(Event::Type type) const
{
  return nullptr;
}

const Event* EventMechanics::pickRandomEvent() const
{
  u16 size = sizeof(events)/sizeof(events[0]);
  const Event* e = nullptr;
  bool allowed = false;
  
  while (!allowed)
  {
    u16 r = Util::randomIntUpTo(size);
    e = events[r];
    
    // two events cannot occur before turn 150
    if ((e == Events::DIPLOMATIC_MARRIAGE || e == Events::GREAT_METEOR)  && g->getTurnCount() < 150)
      continue;
    else
    {
      allowed = true;
    }
  }
  
  return e;
}

#pragma mark mercenary hero offers

float EventMechanics::chanceOfMercenaryHeroOffer(const Player* player)
{
  // (3% + (fame / 25) )  / min(1, # heroes of player / 2)
  
  //TODO: if list<Hero*> will be used to keep dead heroes we need to adjust
  size_t heroCount = player->getHeroes().size();
  
  if (heroCount == 6)
    return 0.0f;

  float chance = (0.03f + (player->getFame()/25) ) / std::min(1, (int)heroCount / 2);
  
  /* x 2 if famous */
  if (player->hasTrait("famous"))
    chance *= 2.0f;
  
  return std::min(chance, 0.10f);
}

u32 EventMechanics::feeForMercenaryHeroOffer(const Player* player, const Hero* hero)
{
  u32 base = 100 + (hero->getSpec()->requiredFame * 10);
  u32 fee = base * (3 * hero->getExperienceLevel()->ordinal()) / 4;
  
  if (player->hasTrait("charismatic"))
    fee /= 2;
  
  return fee;
}
