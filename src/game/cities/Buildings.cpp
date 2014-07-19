#include "Buildings.h"

#include "Localization.h"

const Building Building::buildings[] = {
  Building(I18_BUILDING_BUILDERS_HALL, 60, 1, 0),
  Building(I18_BUILDING_SMITHY, 40, 1, 0),
  Building(I18_BUILDING_CITY_WALLS, 150, 2, 0),
  Building(I18_BUILDING_SHRINE, 100, 1, 0),
  Building(I18_BUILDING_TEMPLE, 200, 2, 0),
  Building(I18_BUILDING_PARTHENON, 400, 3, 0),
  Building(I18_BUILDING_CATHEDRAL, 800, 4, 0),
  Building(I18_BUILDING_ANIMISTS_GUILD, 300, 5, 0),
  Building(I18_BUILDING_ORACLE, 500, 4, 0),
  Building(I18_BUILDING_ALCHEMISTS_GUILD, 250, 3, 0),
  Building(I18_BUILDING_WIZARDS_GUILD, 1000, 5, 3),
  Building(I18_BUILDING_STABLE, 80, 2, 0),
  Building(I18_BUILDING_FANTASTIC_STABLE, 600, 6, 0),
  Building(I18_BUILDING_BARRACKS, 30, 0, 0),
  Building(I18_BUILDING_ARMORY, 80, 2, 0),
  Building(I18_BUILDING_FIGHTERS_GUILD, 200, 3, 0),
  Building(I18_BUILDING_ARMORERS_GUILD, 350, 4, 0),
  Building(I18_BUILDING_WAR_COLLEGE, 500, 5, 0),
  Building(I18_BUILDING_SHIP_WRIGHTS_GUILD, 100, 1, 0),
  Building(I18_BUILDING_SHIP_YARD, 200, 2, 0),
  Building(I18_BUILDING_MARITIME_GUILD, 400, 4, 0),
  Building(I18_BUILDING_MARKETPLACE, 100, 1, 0),
  Building(I18_BUILDING_BANK, 250, 3, 0),
  Building(I18_BUILDING_MERCHANTS_GUILD, 600, 5, 0),
  Building(I18_BUILDING_GRANARY, 40, 1, 0),
  Building(I18_BUILDING_FARMERS_MARKET, 100, 2, 0),
  Building(I18_BUILDING_LIBRARY, 60, 1, 0),
  Building(I18_BUILDING_UNIVERSITY, 300, 3, 0),
  Building(I18_BUILDING_SAGES_GUILD, 120, 2, 0),
  Building(I18_BUILDING_MINERS_GUILD, 300, 3, 0),
  Building(I18_BUILDING_MECHANICIANS_GUILD, 600, 5, 0),
  Building(I18_BUILDING_SAWMILL, 100, 2, 0),
  Building(I18_BUILDING_FORESTERS_GUILD, 200, 2, 0),
  
  Building(I18_BUILDING_TRADE_GOODS, 0, 0, 0),
  Building(I18_BUILDING_HOUSING, 0, 0, 0),
  
  /* TODO: manage name, eg Jack's Fortress */
  Building(I18_BUILDING_MAGE_FORTRESS, 0, 0, 0),
  
  Building(I18_BUILDING_SUMMONING_CIRCLE, 0, 0, 0)
};



const std::string& Building::productionName() const { return i18n::s(name); }
