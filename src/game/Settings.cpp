#include "Settings.h"

#include "Localization.h"

Settings::Settings()
{
  const std::string i18prefix = "game_option_";
  
  
  
  settings.push_back(SettingGroup(SETTING_DIFFICULTY, I18::PLACEHOLDER, DIFFICULTY_NORMAL, {
    {i18n::keyForString(i18prefix + "difficulty_intro"), DIFFICULTY_INTRO},
    {i18n::keyForString(i18prefix + "difficulty_easy"), DIFFICULTY_EASY},
    {i18n::keyForString(i18prefix + "difficulty_normal"), DIFFICULTY_NORMAL},
    {i18n::keyForString(i18prefix + "difficulty_hard"), DIFFICULTY_HARD},
    {i18n::keyForString(i18prefix + "difficulty_impossible"), DIFFICULTY_IMPOSSIBLE}}));
  
  settings.push_back(SettingGroup(SETTING_LAND_SIZE, I18::PLACEHOLDER, LAND_SIZE_LARGE, {
    {i18n::keyForString(i18prefix + "land_size_small"), LAND_SIZE_SMALL},
    {i18n::keyForString(i18prefix + "land_size_medium"), LAND_SIZE_MEDIUM},
    {i18n::keyForString(i18prefix + "land_size_large"), LAND_SIZE_LARGE}}));
  
  settings.push_back(SettingGroup(SETTING_OPPONENTS, I18::PLACEHOLDER, OPPONENTS_ONE, {
    {i18n::keyForString(i18prefix + "opponents_one"), OPPONENTS_ONE},
    {i18n::keyForString(i18prefix + "opponents_two"), OPPONENTS_TWO},
    {i18n::keyForString(i18prefix + "opponents_three"), OPPONENTS_THREE},
    {i18n::keyForString(i18prefix + "opponents_four"), OPPONENTS_FOUR}}));
  
  settings.push_back(SettingGroup(SETTING_MAGIC_POWER, I18::PLACEHOLDER, MAGIC_POWER_NORMAL, {
    {i18n::keyForString(i18prefix + "magic_weak"), MAGIC_POWER_WEAK},
    {i18n::keyForString(i18prefix + "magic_normal"), MAGIC_POWER_NORMAL},
    {i18n::keyForString(i18prefix + "magic_powerful"), MAGIC_POWER_POWERFUL}}));
}

const std::string& SettingGroup::groupName()
{
  return i18n::s(name);
}

const std::string& SettingGroup::currentValue()
{
  return i18n::s(group[selected].name);
}
