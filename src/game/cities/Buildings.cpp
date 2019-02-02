#include "Buildings.h"

#include "i18n/Localization.h"

namespace buildingimpl {
  static const Building BUILDERS_HALL = Building(I18::BUILDING_BUILDERS_HALL, I18::BUILDING_DESC_BUILDERS_HALL, 60, 1, 0);
  static const Building SMITHY = Building(I18::BUILDING_SMITHY, I18::BUILDING_DESC_SMITHY, 40, 1, 0);
  static const Building CITY_WALLS = Building(I18::BUILDING_CITY_WALLS, I18::BUILDING_DESC_CITY_WALLS, 150, 2, 0);
  static const Building SHRINE = Building(I18::BUILDING_SHRINE, I18::BUILDING_DESC_SHRINE, 100, 1, 0);
  static const Building TEMPLE = Building(I18::BUILDING_TEMPLE, I18::BUILDING_DESC_TEMPLE, 200, 2, 0);
  static const Building PARTHENON = Building(I18::BUILDING_PARTHENON, I18::BUILDING_DESC_PARTHENON, 400, 3, 0);
  static const Building CATHEDRAL = Building(I18::BUILDING_CATHEDRAL, I18::BUILDING_DESC_CATHEDRAL, 800, 4, 0);
  static const Building ANIMISTS_GUILD = Building(I18::BUILDING_ANIMISTS_GUILD, I18::BUILDING_DESC_ANIMISTS_GUILD, 300, 5, 0);
  static const Building ORACLE = Building(I18::BUILDING_ORACLE, I18::BUILDING_DESC_ORACLE, 500, 4, 0);
  static const Building ALCHEMISTS_GUILD = Building(I18::BUILDING_ALCHEMISTS_GUILD, I18::BUILDING_DESC_ALCHEMISTS_GUILD, 250, 3, 0);
  static const Building WIZARDS_GUILD = Building(I18::BUILDING_WIZARDS_GUILD, I18::BUILDING_DESC_WIZARDS_GUILD, 1000, 5, 3);
  static const Building STABLE = Building(I18::BUILDING_STABLE, I18::BUILDING_DESC_STABLE, 80, 2, 0);
  static const Building FANTASTIC_STABLE = Building(I18::BUILDING_FANTASTIC_STABLE, I18::BUILDING_DESC_FANTASTIC_STABLE, 600, 6, 0);
  static const Building BARRACKS = Building(I18::BUILDING_BARRACKS, I18::BUILDING_DESC_BARRACKS, 30, 0, 0);
  static const Building ARMORY = Building(I18::BUILDING_ARMORY, I18::BUILDING_DESC_ARMORY, 80, 2, 0);
  static const Building FIGHTERS_GUILD = Building(I18::BUILDING_FIGHTERS_GUILD, I18::BUILDING_DESC_FIGHTERS_GUILD, 200, 3, 0);
  static const Building ARMORERS_GUILD = Building(I18::BUILDING_ARMORERS_GUILD, I18::BUILDING_DESC_ARMORERS_GUILD, 350, 4, 0);
  static const Building WAR_COLLEGE = Building(I18::BUILDING_WAR_COLLEGE, I18::BUILDING_DESC_WAR_COLLEGE, 500, 5, 0);
  static const Building SHIP_WRIGHTS_GUILD = Building(I18::BUILDING_SHIP_WRIGHTS_GUILD, I18::BUILDING_DESC_SHIP_WRIGHTS_GUILD, 100, 1, 0);
  static const Building SHIP_YARD = Building(I18::BUILDING_SHIP_YARD, I18::BUILDING_DESC_SHIP_YARD, 200, 2, 0);
  static const Building MARITIME_GUILD = Building(I18::BUILDING_MARITIME_GUILD, I18::BUILDING_DESC_MARITIME_GUILD, 400, 4, 0);
  static const Building MARKETPLACE = Building(I18::BUILDING_MARKETPLACE, I18::BUILDING_DESC_MARKETPLACE, 100, 1, 0);
  static const Building BANK = Building(I18::BUILDING_BANK, I18::BUILDING_DESC_BANK, 250, 3, 0);
  static const Building MERCHANTS_GUILD = Building(I18::BUILDING_MERCHANTS_GUILD, I18::BUILDING_DESC_MERCHANTS_GUILD, 600, 5, 0);
  static const Building GRANARY = Building(I18::BUILDING_GRANARY, I18::BUILDING_DESC_GRANARY, 40, 1, 0);
  static const Building FARMERS_MARKET = Building(I18::BUILDING_FARMERS_MARKET, I18::BUILDING_DESC_FARMERS_MARKET, 100, 2, 0);
  static const Building LIBRARY = Building(I18::BUILDING_LIBRARY, I18::BUILDING_DESC_LIBRARY, 60, 1, 0);
  static const Building UNIVERSITY = Building(I18::BUILDING_UNIVERSITY, I18::BUILDING_DESC_UNIVERSITY, 300, 3, 0);
  static const Building SAGES_GUILD = Building(I18::BUILDING_SAGES_GUILD, I18::BUILDING_DESC_SAGES_GUILD, 120, 2, 0);
  static const Building MINERS_GUILD = Building(I18::BUILDING_MINERS_GUILD, I18::BUILDING_DESC_MINERS_GUILD, 300, 3, 0);
  static const Building MECHANICIANS_GUILD = Building(I18::BUILDING_MECHANICIANS_GUILD, I18::BUILDING_DESC_MECHANICIANS_GUILD, 600, 5, 0);
  static const Building SAWMILL = Building(I18::BUILDING_SAWMILL, I18::BUILDING_DESC_SAWMILL, 100, 2, 0);
  static const Building FORESTERS_GUILD = Building(I18::BUILDING_FORESTERS_GUILD, I18::BUILDING_DESC_FORESTERS_GUILD, 200, 2, 0);

  static const Building TRADE_GOODS = Building(I18::BUILDING_TRADE_GOODS, I18::BUILDING_DESC_TRADE_GOODS, 0, 0, 0);
  static const Building HOUSING = Building(I18::BUILDING_HOUSING, I18::BUILDING_DESC_HOUSING, 0, 0, 0);

  /* TODO: manage name, eg Jack's Fortress */
  static const Building MAGE_FORTRESS = Building(I18::BUILDING_MAGE_FORTRESS, I18::EMPTY, 0, 0, 0);

  static const Building SUMMONING_CIRCLE = Building(I18::BUILDING_SUMMONING_CIRCLE, I18::EMPTY, 0, 0, 0);
}

const std::string& Building::productionName() const { return i18n::s(name); }

const Building* Building::buildings[] = {
  TRADE_GOODS,
  HOUSING,
  BARRACKS,
  ARMORY,
  FIGHTERS_GUILD,
  ARMORERS_GUILD,
  WAR_COLLEGE,
  SMITHY,
  STABLE,
  ANIMISTS_GUILD,
  FANTASTIC_STABLE,
  SHIP_WRIGHTS_GUILD,
  SHIP_YARD,
  MARITIME_GUILD,
  SAWMILL,
  LIBRARY,
  SAGES_GUILD,
  ORACLE,
  ALCHEMISTS_GUILD,
  UNIVERSITY,
  WIZARDS_GUILD,
  SHRINE,
  TEMPLE,
  PARTHENON,
  CATHEDRAL,
  MARKETPLACE,
  BANK,
  MERCHANTS_GUILD,
  GRANARY,
  FARMERS_MARKET,
  FORESTERS_GUILD,
  BUILDERS_HALL,
  MECHANICIANS_GUILD,
  MINERS_GUILD,
  CITY_WALLS,
  MAGE_FORTRESS,
  SUMMONING_CIRCLE
};
const size_t Building::COUNT = sizeof(buildings)/sizeof(buildings[0]);


const Building* Building::BARRACKS = &buildingimpl::BARRACKS;
const Building* Building::ARMORY = &buildingimpl::ARMORY;
const Building* Building::FIGHTERS_GUILD = &buildingimpl::FIGHTERS_GUILD;
const Building* Building::ARMORERS_GUILD = &buildingimpl::ARMORERS_GUILD;
const Building* Building::WAR_COLLEGE = &buildingimpl::WAR_COLLEGE;

const Building* Building::SMITHY = &buildingimpl::SMITHY;

const Building* Building::STABLE = &buildingimpl::STABLE;
const Building* Building::ANIMISTS_GUILD = &buildingimpl::ANIMISTS_GUILD;
const Building* Building::FANTASTIC_STABLE = &buildingimpl::FANTASTIC_STABLE;

const Building* Building::SHIP_WRIGHTS_GUILD = &buildingimpl::SHIP_WRIGHTS_GUILD;
const Building* Building::SHIP_YARD = &buildingimpl::SHIP_YARD;
const Building* Building::MARITIME_GUILD = &buildingimpl::MARITIME_GUILD;

const Building* Building::SAWMILL = &buildingimpl::SAWMILL;

const Building* Building::LIBRARY = &buildingimpl::LIBRARY;
const Building* Building::SAGES_GUILD = &buildingimpl::SAGES_GUILD;
const Building* Building::ORACLE = &buildingimpl::ORACLE;

const Building* Building::ALCHEMISTS_GUILD = &buildingimpl::ALCHEMISTS_GUILD;
const Building* Building::UNIVERSITY = &buildingimpl::UNIVERSITY;
const Building* Building::WIZARDS_GUILD = &buildingimpl::WIZARDS_GUILD;

const Building* Building::SHRINE = &buildingimpl::SHRINE;
const Building* Building::TEMPLE = &buildingimpl::TEMPLE;
const Building* Building::PARTHENON = &buildingimpl::PARTHENON;
const Building* Building::CATHEDRAL = &buildingimpl::CATHEDRAL;

const Building* Building::MARKETPLACE = &buildingimpl::MARKETPLACE;
const Building* Building::BANK = &buildingimpl::BANK;
const Building* Building::MERCHANTS_GUILD = &buildingimpl::MERCHANTS_GUILD;

const Building* Building::GRANARY = &buildingimpl::GRANARY;
const Building* Building::FARMERS_MARKET = &buildingimpl::FARMERS_MARKET;
const Building* Building::FORESTERS_GUILD = &buildingimpl::FORESTERS_GUILD;

const Building* Building::BUILDERS_HALL = &buildingimpl::BUILDERS_HALL;

const Building* Building::MECHANICIANS_GUILD = &buildingimpl::MECHANICIANS_GUILD;
const Building* Building::MINERS_GUILD = &buildingimpl::MINERS_GUILD;

const Building* Building::CITY_WALLS = &buildingimpl::CITY_WALLS;

const Building* Building::TRADE_GOODS = &buildingimpl::TRADE_GOODS;
const Building* Building::HOUSING = &buildingimpl::HOUSING;
const Building* Building::MAGE_FORTRESS = &buildingimpl::MAGE_FORTRESS;
const Building* Building::SUMMONING_CIRCLE = &buildingimpl::SUMMONING_CIRCLE;




