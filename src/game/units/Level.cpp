#include "Level.h"


const UnitLevel UnitLevel::ELITE   = UnitLevel(  UnitLevelID::ELITE, I18::PLACEHOLDER,             nullptr, 500, {2,1,3,1,1});
const UnitLevel UnitLevel::VETERAN = UnitLevel(UnitLevelID::VETERAN, I18::PLACEHOLDER,   &UnitLevel::ELITE, 119, {1,1,2,0,0});
const UnitLevel UnitLevel::REGULAR = UnitLevel(UnitLevelID::REGULAR, I18::PLACEHOLDER, &UnitLevel::VETERAN,  59, {1,0,1,0,0});
const UnitLevel UnitLevel::RECRUIT = UnitLevel(UnitLevelID::RECRUIT, I18::PLACEHOLDER, &UnitLevel::REGULAR,  19, {0,0,0,0,0});

const HeroLevel HeroLevel::DEMIGOD    = HeroLevel(   HeroLevelID::DEMIGOD, I18::PLACEHOLDER,                nullptr,  -1, {8,4,8,8,3});
const HeroLevel HeroLevel::SUPER_HERO = HeroLevel(HeroLevelID::SUPER_HERO, I18::PLACEHOLDER,    &HeroLevel::DEMIGOD, 999, {7,4,7,7,3});
const HeroLevel HeroLevel::GRAND_LORD = HeroLevel(HeroLevelID::GRAND_LORD, I18::PLACEHOLDER, &HeroLevel::SUPER_HERO, 599, {6,3,6,6,2});
const HeroLevel HeroLevel::LORD       = HeroLevel(      HeroLevelID::LORD, I18::PLACEHOLDER, &HeroLevel::GRAND_LORD, 449, {5,3,5,5,2});
const HeroLevel HeroLevel::CHAMPION   = HeroLevel(  HeroLevelID::CHAMPION, I18::PLACEHOLDER,       &HeroLevel::LORD, 299, {4,2,4,4,1});
const HeroLevel HeroLevel::COMMANDER  = HeroLevel( HeroLevelID::COMMANDER, I18::PLACEHOLDER,   &HeroLevel::CHAMPION, 199, {3,2,3,3,1});
const HeroLevel HeroLevel::CAPTAIN    = HeroLevel(   HeroLevelID::CAPTAIN, I18::PLACEHOLDER,  &HeroLevel::COMMANDER, 119, {2,1,2,2,0});
const HeroLevel HeroLevel::MYRMIDON   = HeroLevel(  HeroLevelID::MYRMIDON, I18::PLACEHOLDER,    &HeroLevel::CAPTAIN,  59, {1,1,1,1,0});
const HeroLevel HeroLevel::HERO       = HeroLevel(      HeroLevelID::HERO, I18::PLACEHOLDER,   &HeroLevel::MYRMIDON,  19, {0,0,0,0,0});
