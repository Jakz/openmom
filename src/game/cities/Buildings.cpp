#include "Buildings.h"

#include "Localization.h"

const Building Building::buildings[] = {
  Building(BuildingID::BUILDERS_HALL,     I18::BUILDING_BUILDERS_HALL, 60, 1, 0),
  Building(BuildingID::SMITHY,            I18::BUILDING_SMITHY, 40, 1, 0),
  Building(BuildingID::CITY_WALLS,        I18::BUILDING_CITY_WALLS, 150, 2, 0),
  Building(BuildingID::SHRINE,            I18::BUILDING_SHRINE, 100, 1, 0),
  Building(BuildingID::TEMPLE,            I18::BUILDING_TEMPLE, 200, 2, 0),
  Building(BuildingID::PARTHENON,         I18::BUILDING_PARTHENON, 400, 3, 0),
  Building(BuildingID::CATHEDRAL,         I18::BUILDING_CATHEDRAL, 800, 4, 0),
  Building(BuildingID::ANIMISTS_GUILD,    I18::BUILDING_ANIMISTS_GUILD, 300, 5, 0),
  Building(BuildingID::ORACLE,            I18::BUILDING_ORACLE, 500, 4, 0),
  Building(BuildingID::ALCHEMISTS_GUILD,  I18::BUILDING_ALCHEMISTS_GUILD, 250, 3, 0),
  Building(BuildingID::WIZARDS_GUILD,     I18::BUILDING_WIZARDS_GUILD, 1000, 5, 3),
  Building(BuildingID::STABLE,            I18::BUILDING_STABLE, 80, 2, 0),
  Building(BuildingID::FANTASTIC_STABLE,  I18::BUILDING_FANTASTIC_STABLE, 600, 6, 0),
  Building(BuildingID::BARRACKS,          I18::BUILDING_BARRACKS, 30, 0, 0),
  Building(BuildingID::ARMORY,            I18::BUILDING_ARMORY, 80, 2, 0),
  Building(BuildingID::FIGHTERS_GUILD,    I18::BUILDING_FIGHTERS_GUILD, 200, 3, 0),
  Building(BuildingID::ARMORERS_GUILD,    I18::BUILDING_ARMORERS_GUILD, 350, 4, 0),
  Building(BuildingID::WAR_COLLEGE,       I18::BUILDING_WAR_COLLEGE, 500, 5, 0),
  Building(BuildingID::SHIP_WRIGHTS_GUILD,I18::BUILDING_SHIP_WRIGHTS_GUILD, 100, 1, 0),
  Building(BuildingID::SHIP_YARD,         I18::BUILDING_SHIP_YARD, 200, 2, 0),
  Building(BuildingID::MARITIME_GUILD,    I18::BUILDING_MARITIME_GUILD, 400, 4, 0),
  Building(BuildingID::MARKETPLACE,       I18::BUILDING_MARKETPLACE, 100, 1, 0),
  Building(BuildingID::BANK,              I18::BUILDING_BANK, 250, 3, 0),
  Building(BuildingID::MERCHANTS_GUILD,   I18::BUILDING_MERCHANTS_GUILD, 600, 5, 0),
  Building(BuildingID::GRANARY,           I18::BUILDING_GRANARY, 40, 1, 0),
  Building(BuildingID::FARMERS_MARKET,    I18::BUILDING_FARMERS_MARKET, 100, 2, 0),
  Building(BuildingID::LIBRARY,           I18::BUILDING_LIBRARY, 60, 1, 0),
  Building(BuildingID::UNIVERSITY,        I18::BUILDING_UNIVERSITY, 300, 3, 0),
  Building(BuildingID::SAGES_GUILD,       I18::BUILDING_SAGES_GUILD, 120, 2, 0),
  Building(BuildingID::MINERS_GUILD,      I18::BUILDING_MINERS_GUILD, 300, 3, 0),
  Building(BuildingID::MECHANICIANS_GUILD,I18::BUILDING_MECHANICIANS_GUILD, 600, 5, 0),
  Building(BuildingID::SAWMILL,           I18::BUILDING_SAWMILL, 100, 2, 0),
  Building(BuildingID::FORESTERS_GUILD,   I18::BUILDING_FORESTERS_GUILD, 200, 2, 0),
  
  Building(BuildingID::TRADE_GOODS,       I18::BUILDING_TRADE_GOODS, 0, 0, 0),
  Building(BuildingID::HOUSING,           I18::BUILDING_HOUSING, 0, 0, 0),
  
  /* TODO: manage name, eg Jack's Fortress */
  Building(BuildingID::MAGE_FORTRESS,     I18::BUILDING_MAGE_FORTRESS, 0, 0, 0),
  
  Building(BuildingID::SUMMONING_CIRCLE,   I18::BUILDING_SUMMONING_CIRCLE, 0, 0, 0)
};



const std::string& Building::productionName() const { return i18n::s(name); }
