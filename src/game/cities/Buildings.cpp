#include "Buildings.h"

#include "Localization.h"

namespace buildingimpl {
  static const Building buildings[] = {
    Building(I18::BUILDING_BUILDERS_HALL, I18::BUILDING_DESC_BUILDERS_HALL, 60, 1, 0),
    Building(I18::BUILDING_SMITHY, I18::BUILDING_DESC_SMITHY, 40, 1, 0),
    Building(I18::BUILDING_CITY_WALLS, I18::BUILDING_DESC_CITY_WALLS, 150, 2, 0),
    Building(I18::BUILDING_SHRINE, I18::BUILDING_DESC_SHRINE, 100, 1, 0),
    Building(I18::BUILDING_TEMPLE, I18::BUILDING_DESC_TEMPLE, 200, 2, 0),
    Building(I18::BUILDING_PARTHENON, I18::BUILDING_DESC_PARTHENON, 400, 3, 0),
    Building(I18::BUILDING_CATHEDRAL, I18::BUILDING_DESC_CATHEDRAL, 800, 4, 0),
    Building(I18::BUILDING_ANIMISTS_GUILD, I18::BUILDING_DESC_ANIMISTS_GUILD, 300, 5, 0),
    Building(I18::BUILDING_ORACLE, I18::BUILDING_DESC_ORACLE, 500, 4, 0),
    Building(I18::BUILDING_ALCHEMISTS_GUILD, I18::BUILDING_DESC_ALCHEMISTS_GUILD, 250, 3, 0),
    Building(I18::BUILDING_WIZARDS_GUILD, I18::BUILDING_DESC_WIZARDS_GUILD, 1000, 5, 3),
    Building(I18::BUILDING_STABLE, I18::BUILDING_DESC_STABLE, 80, 2, 0),
    Building(I18::BUILDING_FANTASTIC_STABLE, I18::BUILDING_DESC_FANTASTIC_STABLE, 600, 6, 0),
    Building(I18::BUILDING_BARRACKS, I18::BUILDING_DESC_BARRACKS, 30, 0, 0),
    Building(I18::BUILDING_ARMORY, I18::BUILDING_DESC_ARMORY, 80, 2, 0),
    Building(I18::BUILDING_FIGHTERS_GUILD, I18::BUILDING_DESC_FIGHTERS_GUILD, 200, 3, 0),
    Building(I18::BUILDING_ARMORERS_GUILD, I18::BUILDING_DESC_ARMORERS_GUILD, 350, 4, 0),
    Building(I18::BUILDING_WAR_COLLEGE, I18::BUILDING_DESC_WAR_COLLEGE, 500, 5, 0),
    Building(I18::BUILDING_SHIP_WRIGHTS_GUILD, I18::BUILDING_DESC_SHIP_WRIGHTS_GUILD, 100, 1, 0),
    Building(I18::BUILDING_SHIP_YARD, I18::BUILDING_DESC_SHIP_YARD, 200, 2, 0),
    Building(I18::BUILDING_MARITIME_GUILD, I18::BUILDING_DESC_MARITIME_GUILD, 400, 4, 0),
    Building(I18::BUILDING_MARKETPLACE, I18::BUILDING_DESC_MARKETPLACE, 100, 1, 0),
    Building(I18::BUILDING_BANK, I18::BUILDING_DESC_BANK, 250, 3, 0),
    Building(I18::BUILDING_MERCHANTS_GUILD, I18::BUILDING_DESC_MERCHANTS_GUILD, 600, 5, 0),
    Building(I18::BUILDING_GRANARY, I18::BUILDING_DESC_GRANARY, 40, 1, 0),
    Building(I18::BUILDING_FARMERS_MARKET, I18::BUILDING_DESC_FARMERS_MARKET, 100, 2, 0),
    Building(I18::BUILDING_LIBRARY, I18::BUILDING_DESC_LIBRARY, 60, 1, 0),
    Building(I18::BUILDING_UNIVERSITY, I18::BUILDING_DESC_UNIVERSITY, 300, 3, 0),
    Building(I18::BUILDING_SAGES_GUILD, I18::BUILDING_DESC_SAGES_GUILD, 120, 2, 0),
    Building(I18::BUILDING_MINERS_GUILD, I18::BUILDING_DESC_MINERS_GUILD, 300, 3, 0),
    Building(I18::BUILDING_MECHANICIANS_GUILD, I18::BUILDING_DESC_MECHANICIANS_GUILD, 600, 5, 0),
    Building(I18::BUILDING_SAWMILL, I18::BUILDING_DESC_SAWMILL, 100, 2, 0),
    Building(I18::BUILDING_FORESTERS_GUILD, I18::BUILDING_DESC_FORESTERS_GUILD, 200, 2, 0),
    
    Building(I18::BUILDING_TRADE_GOODS, I18::BUILDING_DESC_TRADE_GOODS, 0, 0, 0),
    Building(I18::BUILDING_HOUSING, I18::BUILDING_DESC_HOUSING, 0, 0, 0),
    
    /* TODO: manage name, eg Jack's Fortress */
    Building(I18::BUILDING_MAGE_FORTRESS, I18::EMPTY, 0, 0, 0),
    
    Building(I18::BUILDING_SUMMONING_CIRCLE, I18::EMPTY, 0, 0, 0)

  };
}

const std::string& Building::productionName() const { return i18n::s(name); }

const Building *Building::buildings[] = {
  BUILDERS_HALL,SMITHY,CITY_WALLS,SHRINE,TEMPLE,PARTHENON,CATHEDRAL,ANIMISTS_GUILD,ORACLE,ALCHEMISTS_GUILD,ORACLE,ANIMISTS_GUILD,WIZARDS_GUILD,STABLE,FANTASTIC_STABLE,
  BARRACKS,ARMORERS_GUILD,FIGHTERS_GUILD,ARMORERS_GUILD,WAR_COLLEGE,SHIP_WRIGHTS_GUILD,SHIP_YARD,MARITIME_GUILD,MARKETPLACE,BANK,MERCHANTS_GUILD,GRANARY,FARMERS_MARKET,LIBRARY,
  UNIVERSITY,SAGES_GUILD,MINERS_GUILD,MECHANICIANS_GUILD,SAWMILL,FORESTERS_GUILD,TRADE_GOODS,HOUSING,MAGE_FORTRESS,SUMMONING_CIRCLE
};
const size_t Building::COUNT = sizeof(buildings)/sizeof(buildings[0]);


const Building* Building::BUILDERS_HALL = &buildingimpl::buildings[0];
const Building* Building::SMITHY = &buildingimpl::buildings[1];
const Building* Building::CITY_WALLS = &buildingimpl::buildings[2];
const Building* Building::SHRINE = &buildingimpl::buildings[3];
const Building* Building::TEMPLE = &buildingimpl::buildings[4];
const Building* Building::PARTHENON = &buildingimpl::buildings[5];
const Building* Building::CATHEDRAL = &buildingimpl::buildings[6];
const Building* Building::ANIMISTS_GUILD = &buildingimpl::buildings[7];
const Building* Building::ORACLE = &buildingimpl::buildings[8];
const Building* Building::ALCHEMISTS_GUILD = &buildingimpl::buildings[9];
const Building* Building::WIZARDS_GUILD = &buildingimpl::buildings[10];
const Building* Building::STABLE = &buildingimpl::buildings[11];
const Building* Building::FANTASTIC_STABLE = &buildingimpl::buildings[12];
const Building* Building::BARRACKS = &buildingimpl::buildings[13];
const Building* Building::ARMORY = &buildingimpl::buildings[14];
const Building* Building::FIGHTERS_GUILD = &buildingimpl::buildings[15];
const Building* Building::ARMORERS_GUILD = &buildingimpl::buildings[16];
const Building* Building::WAR_COLLEGE = &buildingimpl::buildings[17];
const Building* Building::SHIP_WRIGHTS_GUILD = &buildingimpl::buildings[18];
const Building* Building::SHIP_YARD = &buildingimpl::buildings[19];
const Building* Building::MARITIME_GUILD = &buildingimpl::buildings[20];
const Building* Building::MARKETPLACE = &buildingimpl::buildings[21];
const Building* Building::BANK = &buildingimpl::buildings[22];
const Building* Building::MERCHANTS_GUILD = &buildingimpl::buildings[23];
const Building* Building::GRANARY = &buildingimpl::buildings[24];
const Building* Building::FARMERS_MARKET = &buildingimpl::buildings[25];
const Building* Building::LIBRARY = &buildingimpl::buildings[26];
const Building* Building::UNIVERSITY = &buildingimpl::buildings[27];
const Building* Building::SAGES_GUILD = &buildingimpl::buildings[28];
const Building* Building::MINERS_GUILD = &buildingimpl::buildings[29];
const Building* Building::MECHANICIANS_GUILD = &buildingimpl::buildings[30];
const Building* Building::SAWMILL = &buildingimpl::buildings[31];
const Building* Building::FORESTERS_GUILD = &buildingimpl::buildings[32];
const Building* Building::TRADE_GOODS = &buildingimpl::buildings[33];
const Building* Building::HOUSING = &buildingimpl::buildings[34];
const Building* Building::MAGE_FORTRESS = &buildingimpl::buildings[35];
const Building* Building::SUMMONING_CIRCLE = &buildingimpl::buildings[36];




