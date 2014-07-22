#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "Common.h"
#include "Localization.h"

#include <array>

enum class UnitLevelID : u8
{
  RECRUIT = 0,
  REGULAR,
  VETERAN,
  ELITE
};

enum class HeroLevelID : u8
{
  HERO = 0,
  MYRMIDON,
  CAPTAIN,
  COMMANDER,
  CHAMPION,
  LORD,
  GRAND_LORD,
  SUPER_HERO,
  DEMIGOD
};

typedef std::array<s16,5> level_bonuses;

class Level
{
protected:
  s32 maxXP;

public:
  Level(I18 name, const Level* next, s32 maxXP, level_bonuses bonuses) : name(name), next(next), maxXP(maxXP), bonuses(bonuses) { }
  
  bool hasLeveled(s32 xp) const { return xp > maxXP && next; }
  
  const Level* next;
  const I18 name;
  const level_bonuses bonuses;
  virtual s16 index() const = 0;
};

class UnitLevel : public Level
{
public:
  UnitLevel(UnitLevelID ident, I18 name, const Level* next, s32 maxXP, level_bonuses bonuses) : Level(name,next,maxXP,bonuses), ident(ident) { }
  
  const UnitLevelID ident;
  s16 index() const { return static_cast<s16>(ident); }
  
  static const UnitLevel RECRUIT, REGULAR, VETERAN, ELITE;
  static constexpr const UnitLevel* const LEVELS[] = {&RECRUIT, &REGULAR, &VETERAN, &ELITE};
};

class HeroLevel : public Level
{
public:
  HeroLevel(HeroLevelID ident, I18 name, const Level* next, s32 maxXP, level_bonuses bonuses) : Level(name,next,maxXP,bonuses), ident(ident) { }
  
  const HeroLevelID ident;
  s16 index() const { return static_cast<s16>(ident); }
  
  static const HeroLevel HERO, MYRMIDON, CAPTAIN, COMMANDER, CHAMPION, LORD, GRAND_LORD, SUPER_HERO, DEMIGOD;
  static constexpr const HeroLevel* const LEVELS[] = {&HERO, &MYRMIDON, &CAPTAIN, &COMMANDER, &CHAMPION, &LORD, &GRAND_LORD, &SUPER_HERO, &DEMIGOD};

};

#endif
