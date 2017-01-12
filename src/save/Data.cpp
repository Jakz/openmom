#include "Data.h"

#include "Localization.h"

static const Trait traitInfo[] = {
  {TraitID::ALCHEMY,1,NO_SCHOOL},
  {TraitID::WARLORD,2,NO_SCHOOL},
  {TraitID::CHANNELER,2,NO_SCHOOL},
  {TraitID::ARCHMAGE,1,NO_SCHOOL},
  {TraitID::ARTIFICER,1,NO_SCHOOL},
  {TraitID::CONJURER,1,NO_SCHOOL},
  {TraitID::SAGE_MASTER,1,NO_SCHOOL},
  {TraitID::MYRRAN,3,NO_SCHOOL},
  {TraitID::DIVINE_POWER,2,NO_SCHOOL},
  {TraitID::FAMOUS,2,NO_SCHOOL},
  {TraitID::RUNEMASTER,1,NO_SCHOOL},
  {TraitID::CHARISMATIC,1,NO_SCHOOL},
  {TraitID::CHAOS_MASTERY,1,CHAOS},
  {TraitID::NATURE_MASTERY,1,NATURE},
  {TraitID::SORCERY_MASTERY,1,SORCERY},
  {TraitID::INFERNAL_POWER,1,NO_SCHOOL},
  {TraitID::MANA_FOCUSING,1,NO_SCHOOL},
  {TraitID::NODE_MASTERY,1,NO_SCHOOL}
};

static const Wizard wizardInfo[] = {
  {WizardID::MERLIN, I18::WIZARD_NAME_MERLIN},
  {WizardID::RAVEN, I18::WIZARD_NAME_RAVEN},
  {WizardID::SHAREE, I18::WIZARD_NAME_SHAREE},
  {WizardID::LO_PAN, I18::WIZARD_NAME_LO_PAN},
  {WizardID::JAFAR, I18::WIZARD_NAME_JAFAR},
  {WizardID::OBERIC, I18::WIZARD_NAME_OBERIC},
  {WizardID::RJAK, I18::WIZARD_NAME_RJAK},
  {WizardID::SSS_RA, I18::WIZARD_NAME_SSS_RA},
  {WizardID::TAURON, I18::WIZARD_NAME_TAURON},
  {WizardID::FREYA, I18::WIZARD_NAME_FREYA},
  {WizardID::HORUS, I18::WIZARD_NAME_HORUS},
  {WizardID::ARIEL, I18::WIZARD_NAME_ARIEL},
  {WizardID::TLALOC, I18::WIZARD_NAME_TLALOC},
  {WizardID::KALI, I18::WIZARD_NAME_KALI}
};

const Trait& Data::trait(const TraitID trait)
{
  return traitInfo[static_cast<u8>(trait)];
}

const Wizard& Data::wizard(const WizardID wizard)
{
  return wizardInfo[static_cast<size_t>(wizard)];
}

#include <unordered_map>

#include "Skill.h"
#include "Race.h"
#include "UnitSpec.h"
#include "Level.h"

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

template<> Data::map_t<const Building*>& Data::containerFor() {
  //TODO: hardcoded for now, then YAML will be used

  static map_t<const Building*> buildingMap = {
    { "barracks", Building::BARRACKS },
    { "armory", Building::ARMORY },
    { "fighters_guild", Building::FIGHTERS_GUILD },
    { "armorers_guild", Building::ARMORERS_GUILD },
    { "war_college", Building::WAR_COLLEGE },
    
    { "smithy", Building::SMITHY },
    
    { "stable", Building::STABLE },
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

template<> Data::map_t<const Race*>& Data::containerFor() {
  //TODO: hardcoded for now, then YAML will be used
  
  static map_t<const Race*> raceMap = {
    { "barbarians", &Race::race(RaceID::BARBARIANS) },
    { "beastmen", &Race::race(RaceID::BEASTMEN) },
    { "dark_elves", &Race::race(RaceID::DARK_ELVES) },
    { "draconians", &Race::race(RaceID::DRACONIANS) },
    { "dwarves", &Race::race(RaceID::DWARVES) },
    { "gnolls", &Race::race(RaceID::GNOLLS) },
    { "halflings", &Race::race(RaceID::HALFLINGS) },
    { "high_elves", &Race::race(RaceID::HIGH_ELVES) },
    { "high_men", &Race::race(RaceID::HIGH_MEN) },
    { "klackons", &Race::race(RaceID::KLACKONS) },
    { "lizardmen", &Race::race(RaceID::LIZARDMEN) },
    { "nomads", &Race::race(RaceID::NOMADS) },
    { "orcs", &Race::race(RaceID::ORCS) },
    { "trolls", &Race::race(RaceID::TROLLS) },
  };
  return raceMap;
}

std::vector<const RaceUnitSpec*> Data::unitsForRace(const Race* race)
{
  std::vector<const RaceUnitSpec*> racialUnits;
  const auto& units = containerFor<const UnitSpec*>();
  for (const auto& entry : units)
  {
    if (entry.second->type == UnitType::RACIAL)
    {
      const RaceUnitSpec* rspec = static_cast<const RaceUnitSpec*>(entry.second);
      if (rspec->race == race)
        racialUnits.push_back(rspec);
    }
  }
  
  return racialUnits;
}
