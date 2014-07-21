#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include "Common.h"


#include <vector>

enum SettingGroupID
{
  SETTING_MAGIC_POWER,
  SETTING_DIFFICULTY,
  SETTING_OPPONENTS,
  SETTING_LAND_SIZE
};

enum SettingID
{
  MAGIC_POWER_WEAK,
  MAGIC_POWER_NORMAL,
  MAGIC_POWER_POWERFUL,
  
  DIFFICULTY_INTRO,
  DIFFICULTY_EASY,
  DIFFICULTY_NORMAL,
  DIFFICULTY_HARD,
  DIFFICULTY_IMPOSSIBLE,
  
  OPPONENTS_ONE,
  OPPONENTS_TWO,
  OPPONENTS_THREE,
  OPPONENTS_FOUR,
  
  LAND_SIZE_SMALL,
  LAND_SIZE_MEDIUM,
  LAND_SIZE_LARGE
};

struct Setting
{
  const I18 name;
  const SettingID value;
};

struct SettingGroup
{
  const SettingGroupID ident;
  const I18 name;
  const std::vector<const Setting> group;
  u8 selected;
  
  SettingGroup(SettingGroupID ident, I18 name, SettingID start, std::initializer_list<const Setting> settings) : ident(ident), name(name), group(settings)
  {
    selected = std::find_if(group.begin(), group.end(), [&](const Setting& s){ return s.value == start; }) - group.begin();
  }
  
  void next() { selected = (selected + 1) % group.size(); }
  void prev() { selected = selected > 0 ? selected - 1 : group.size() - 1; }
  
  const std::string& groupName();
  const std::string& currentValue();
  
  const SettingID value() { return group[selected].value; }
  
};

struct Settings
{
  private:
    std::vector<SettingGroup> settings;
  
  public:
    Settings();
  
    SettingGroup &group(SettingGroupID ident) { return *std::find_if(settings.begin(), settings.end(), [&](SettingGroup &group) { return group.ident == ident; });  }
};


#endif
