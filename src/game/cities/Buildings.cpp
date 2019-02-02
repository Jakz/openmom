#include "Buildings.h"

#include "i18n/Localization.h"
#include "ui/GfxData.h"

const std::string& Building::productionName() const { return i18n::s(GfxData::buildingGfx(this).name); }

const Building* Building::BARRACKS = nullptr;
const Building* Building::ARMORY = nullptr;
const Building* Building::FIGHTERS_GUILD = nullptr;
const Building* Building::ARMORERS_GUILD = nullptr;
const Building* Building::WAR_COLLEGE = nullptr;

const Building* Building::SMITHY = nullptr;

const Building* Building::STABLE = nullptr;
const Building* Building::ANIMISTS_GUILD = nullptr;
const Building* Building::FANTASTIC_STABLE = nullptr;

const Building* Building::SHIP_WRIGHTS_GUILD = nullptr;
const Building* Building::SHIP_YARD = nullptr;
const Building* Building::MARITIME_GUILD = nullptr;

const Building* Building::SAWMILL = nullptr;

const Building* Building::LIBRARY = nullptr;
const Building* Building::SAGES_GUILD = nullptr;
const Building* Building::ORACLE = nullptr;

const Building* Building::ALCHEMISTS_GUILD = nullptr;
const Building* Building::UNIVERSITY = nullptr;
const Building* Building::WIZARDS_GUILD = nullptr;

const Building* Building::SHRINE = nullptr;
const Building* Building::TEMPLE = nullptr;
const Building* Building::PARTHENON = nullptr;
const Building* Building::CATHEDRAL = nullptr;

const Building* Building::MARKETPLACE = nullptr;
const Building* Building::BANK = nullptr;
const Building* Building::MERCHANTS_GUILD = nullptr;

const Building* Building::GRANARY = nullptr;
const Building* Building::FARMERS_MARKET = nullptr;
const Building* Building::FORESTERS_GUILD = nullptr;

const Building* Building::BUILDERS_HALL = nullptr;

const Building* Building::MECHANICIANS_GUILD = nullptr;
const Building* Building::MINERS_GUILD = nullptr;

const Building* Building::CITY_WALLS = nullptr;

const Building* Building::TRADE_GOODS = nullptr;
const Building* Building::HOUSING = nullptr;
const Building* Building::MAGE_FORTRESS = nullptr;
const Building* Building::SUMMONING_CIRCLE = nullptr;



