#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "Common.h"
#include "Data.h"

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

using level_bonuses = std::unordered_map<Property, s16, enum_hash>;

class Level
{
protected:
  s16 _index;
  s32 _minXP;
  bool _canAdvanceToNext;
  const level_bonuses bonuses;
  const Level* const next;

public:
  Level(s16 index, I18 name, SpriteInfo icon, const Level* next, s32 minXP, level_bonuses bonuses, bool canAdvanceToNext = true) :
  _index(index), visuals({name,icon}), next(next), _minXP(minXP), bonuses(bonuses), _canAdvanceToNext(canAdvanceToNext)
  { }
  
  bool hasLeveled(s32 xp) const { return next && _canAdvanceToNext && xp >= next->_minXP; }
  
  s16 index() const { return _index; }
  s16 ordinal() const { return index() + 1; }
  
  s16 getBonusProperty(Property property) const { auto it = bonuses.find(property); return it != bonuses.end() ? it->second : 0; }
  
  struct
  {
    const I18 name;
    const SpriteInfo icon;
  } visuals;
  
  friend class experience_level;
};

class experience_level
{
private:
  const experience_levels* _levels;
  const Level* _level;
  s32 _xp;

  void tryToLevelUp()
  {
    while (_levels && _level->hasLeveled(_xp))
      _level = _level->next;
  }
  
public:
  experience_level(s32 startingXP, const experience_levels* levels) : _xp(startingXP), _levels(levels), _level(nullptr)
  {
    if (_levels)
    {
      _level = levels->front().get();
      tryToLevelUp();
    }
  }

  void increaseExperience(s32 amount)
  {
    _xp += amount;
    tryToLevelUp();
  }
  
  bool isValid() const { return _levels != nullptr; }
  s32 xp() const { return _xp; }
  const Level* level() const { return _level; }
  s16 getBonusProperty(Property property) const { return _level ? _level->getBonusProperty(property) : 0; }
};

#endif
