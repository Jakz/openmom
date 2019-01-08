#include "Data.h"

#include "Localization.h"

#include <unordered_map>

#include "Spells.h"
#include "Skill.h"
#include "Race.h"
#include "UnitSpec.h"
#include "Level.h"

#if defined(DEBUG)
template<> const char* Data::nameForDataType<const Retort*>() { return "Retort"; }
template<> const char* Data::nameForDataType<const UnitSpec*>() { return "UnitSpec"; }
template<> const char* Data::nameForDataType<const Spell*>() { return "Spell"; }
template<> const char* Data::nameForDataType<const Race*>() { return "Race"; }
template<> const char* Data::nameForDataType<const Wizard*>() { return "Wizard"; }
template<> const char* Data::nameForDataType<const Skill*>() { return "Skill"; }
template<> const char* Data::nameForDataType<const Building*>() { return "Building"; }
#endif

const std::array<School, 5>& Data::schoolsWithoutArcane()
{
  static const std::array<School, 5> schools = { School::LIFE, School::DEATH, School::CHAOS, School::NATURE, School::SORCERY};
  return schools;
}

const std::array<SpellRarity, 4>& spellRarities()
{
  static const std::array<SpellRarity, 4> rarities = { SpellRarity::COMMON, SpellRarity::UNCOMMON, SpellRarity::RARE, SpellRarity::VERY_RARE };
  return rarities;
}


experience_levels Data::normalUnitLevels;
experience_levels Data::heroLevels;

Data::unit_dependency_map_t Data::unitDependsOnBuilding;

template<> Data::map_t<const Skill*>& Data::containerFor() {
  static map_t<const Skill*> skillsMap;
  return skillsMap;
}

template<> Data::map_t<const UnitSpec*>& Data::containerFor() {
  static map_t<const UnitSpec*> unitsMap;
  return unitsMap;
}

template<> Data::map_t<const Spell*>& Data::containerFor() {
  static map_t<const Spell*> spellMap;
  return spellMap;
}

template<> Data::map_t<const Race*>& Data::containerFor() {
  static map_t<const Race*> raceMap;
  return raceMap;
}

template<> Data::map_t<const Retort*>& Data::containerFor() {
  static map_t<const Retort*> retortMap;
  return retortMap;
}

template<> Data::map_t<const Wizard*>& Data::containerFor() {
  static map_t<const Wizard*> wizardMap;
  return wizardMap;
}

template<> Data::map_t<const Building*>& Data::containerFor() {
  //TODO: hardcoded for now, then YAML will be used

  static map_t<const Building*> buildingMap = {
    { "barracks", Building::BARRACKS },
    { "armory", Building::ARMORY },
    { "fighters_guild", Building::FIGHTERS_GUILD },
    { "armorers_guild", Building::ARMORERS_GUILD },
    { "war_college", Building::WAR_COLLEGE },
    
    { "smithy", Building::SMITHY },
    
    { "stables", Building::STABLE },
    { "animists_guild", Building::ANIMISTS_GUILD },
    { "fantastic_stable", Building::FANTASTIC_STABLE },
    
    { "ship_wrights_guild", Building::SHIP_WRIGHTS_GUILD },
    { "ship_yard", Building::SHIP_YARD },
    { "maritime_guild", Building::MARITIME_GUILD },
    
    { "sawmill", Building::SAWMILL },
    
    { "library", Building::LIBRARY },
    { "sages_guild", Building::SAGES_GUILD },
    { "oracle", Building::ORACLE },
    
    { "alchemists_guild", Building::ALCHEMISTS_GUILD },
    { "university", Building::UNIVERSITY },
    { "wizards_guild", Building::WIZARDS_GUILD },
    
    { "shrine", Building::SHRINE },
    { "temple", Building::TEMPLE },
    { "parthenon", Building::PARTHENON },
    { "cathedral", Building::CATHEDRAL },
    
    { "marketplace", Building::MARKETPLACE },
    { "bank", Building::BANK },
    { "merchants_guild", Building::MERCHANTS_GUILD },
    
    { "granary", Building::GRANARY },
    { "farmers_market", Building::FARMERS_MARKET },
    { "foresters_guild", Building::FORESTERS_GUILD },
    
    { "builders_hall", Building::BUILDERS_HALL },
    
    { "mechanicians_guild", Building::MECHANICIANS_GUILD },
    { "miners_guild", Building::MINERS_GUILD },
    
    { "city_walls", Building::CITY_WALLS },
    
    { "trade_goods", Building::TRADE_GOODS },
    { "housing", Building::HOUSING },
    { "mage_fortress", Building::MAGE_FORTRESS },
    { "summoning_circle", Building::SUMMONING_CIRCLE },
  };
  return buildingMap;
}

const Skill* Data::skill(const key_type& ident) { return get<const Skill*>(ident); }
const Building* Data::building(const key_type& ident) { return get<const Building*>(ident); }
const UnitSpec* Data::unit(const key_type& ident) { return get<const UnitSpec*>(ident); }
const Spell* Data::spell(const key_type& ident) { return get<const Spell*>(ident); }
const Race* Data::race(const key_type& ident) { return get<const Race*>(ident); }
const Retort* Data::retort(const key_type& ident) { return get<const Retort*>(ident); }
const Wizard* Data::wizard(const key_type& ident) { return get<const Wizard*>(ident); }

std::vector<const RaceUnitSpec*> Data::unitsForRace(const Race* race)
{
  std::vector<const RaceUnitSpec*> racialUnits;
  const auto& units = containerFor<const UnitSpec*>();
  for (const auto& entry : units)
  {
    if (entry.second->productionType() == Productable::Type::UNIT)
    {
      const RaceUnitSpec* rspec = static_cast<const RaceUnitSpec*>(entry.second);
      if (rspec->race == race)
        racialUnits.push_back(rspec);
    }
  }
  
  return racialUnits;
}
