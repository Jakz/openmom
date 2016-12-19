#include "Settings.h"

#include "Localization.h"

Settings::Settings()
{
  settings.push_back(SettingGroup(SETTING_DIFFICULTY, I18::PLACEHOLDER, DIFFICULTY_NORMAL, {
    {I18::PLACEHOLDER, DIFFICULTY_INTRO},
    {I18::PLACEHOLDER, DIFFICULTY_EASY},
    {I18::PLACEHOLDER, DIFFICULTY_NORMAL},
    {I18::PLACEHOLDER, DIFFICULTY_HARD},
    {I18::PLACEHOLDER, DIFFICULTY_IMPOSSIBLE}}));
  
  settings.push_back(SettingGroup(SETTING_LAND_SIZE, I18::PLACEHOLDER, LAND_SIZE_LARGE, {
    {I18::PLACEHOLDER, LAND_SIZE_SMALL},
    {I18::PLACEHOLDER, LAND_SIZE_MEDIUM},
    {I18::PLACEHOLDER, LAND_SIZE_LARGE}}));
  
  settings.push_back(SettingGroup(SETTING_OPPONENTS, I18::PLACEHOLDER, OPPONENTS_ONE, {
    {I18::PLACEHOLDER, OPPONENTS_ONE},
    {I18::PLACEHOLDER, OPPONENTS_TWO},
    {I18::PLACEHOLDER, OPPONENTS_THREE},
    {I18::PLACEHOLDER, OPPONENTS_FOUR}}));
  
  settings.push_back(SettingGroup(SETTING_MAGIC_POWER, I18::PLACEHOLDER, MAGIC_POWER_NORMAL, {
    {I18::PLACEHOLDER, MAGIC_POWER_WEAK},
    {I18::PLACEHOLDER, MAGIC_POWER_NORMAL},
    {I18::PLACEHOLDER, MAGIC_POWER_POWERFUL}}));
}

const std::string& SettingGroup::groupName()
{
  return i18n::s(name);
}

const std::string& SettingGroup::currentValue()
{
  return i18n::s(group[selected].name);
}