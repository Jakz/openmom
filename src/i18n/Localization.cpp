#include "Localization.h"

using namespace std;

unordered_map<I18, const string> i18n::data;

void i18n::load()
{
  data.emplace(I18_RACE_NAME_BARBARIANS, "Barbarians");
  data.emplace(I18_RACE_NAME_BEASTMEN, "Beastmen");
  data.emplace(I18_RACE_NAME_DARK_ELVES, "Dark Elves");
  data.emplace(I18_RACE_NAME_DRACONIANS, "Draconians");
  data.emplace(I18_RACE_NAME_DWARVES, "Dwarves");
  data.emplace(I18_RACE_NAME_GNOLLS, "Gnolls");
  data.emplace(I18_RACE_NAME_HALFLINGS, "Halflings");
  data.emplace(I18_RACE_NAME_HIGH_ELVES, "High Elves");
  data.emplace(I18_RACE_NAME_HIGH_MEN, "High Men");
  data.emplace(I18_RACE_NAME_KLACKONS, "Klackons");
  data.emplace(I18_RACE_NAME_LIZARDMEN, "Lizardmen");
  data.emplace(I18_RACE_NAME_NOMADS, "Nomads");
  data.emplace(I18_RACE_NAME_ORCS, "Orcs");
  data.emplace(I18_RACE_NAME_TROLLS, "Trolls");
  
  data.emplace(I18_UNIT_NAME_BARBARIANS, "Barbarian");
  data.emplace(I18_UNIT_NAME_BEASTMEN, "Beastman");
  data.emplace(I18_UNIT_NAME_DARK_ELVES, "Dark Elf");
  data.emplace(I18_UNIT_NAME_DRACONIANS, "Draconian");
  data.emplace(I18_UNIT_NAME_DWARVES, "Dwarven");
  data.emplace(I18_UNIT_NAME_GNOLLS, "Gnoll");
  data.emplace(I18_UNIT_NAME_HALFLINGS, "Halfling");
  data.emplace(I18_UNIT_NAME_HIGH_ELVES, "High Elf");
  data.emplace(I18_UNIT_NAME_HIGH_MEN, "High Man");
  data.emplace(I18_UNIT_NAME_KLACKONS, "Klackon");
  data.emplace(I18_UNIT_NAME_LIZARDMEN, "Lizardman");
  data.emplace(I18_UNIT_NAME_NOMADS, "Nomad");
  data.emplace(I18_UNIT_NAME_ORCS, "Orc");
  data.emplace(I18_UNIT_NAME_TROLLS, "Troll");

  data.emplace(I18_WIZARD_NAME_MERLIN, "Merlin");
  data.emplace(I18_WIZARD_NAME_RAVEN, "Raven");
  data.emplace(I18_WIZARD_NAME_SHAREE, "Sharee");
  data.emplace(I18_WIZARD_NAME_LO_PAN, "Lo Pan");
  data.emplace(I18_WIZARD_NAME_JAFAR, "Jafar");
  data.emplace(I18_WIZARD_NAME_OBERIC, "Oberic");
  data.emplace(I18_WIZARD_NAME_RJAK, "Rjak");
  data.emplace(I18_WIZARD_NAME_SSS_RA, "Sss'Ra");
  data.emplace(I18_WIZARD_NAME_TAURON, "Tauron");
  data.emplace(I18_WIZARD_NAME_FREYA, "Freya");
  data.emplace(I18_WIZARD_NAME_HORUS, "Horus");
  data.emplace(I18_WIZARD_NAME_ARIEL, "Ariel");
  data.emplace(I18_WIZARD_NAME_TLALOC, "Tlaloc");
  data.emplace(I18_WIZARD_NAME_KALI, "Kali");
  
  data.emplace(I18_BUILDING_BUILDERS_HALL,"Builders Hall");
  data.emplace(I18_BUILDING_SMITHY,"Smithy");
  data.emplace(I18_BUILDING_CITY_WALLS,"City Walls");
  data.emplace(I18_BUILDING_SHRINE,"Shrine");
  data.emplace(I18_BUILDING_TEMPLE,"Temple");
  data.emplace(I18_BUILDING_PARTHENON,"Parthenon");
  data.emplace(I18_BUILDING_CATHEDRAL,"Cathedral");
  data.emplace(I18_BUILDING_ANIMISTS_GUILD,"Animists Guild");
  data.emplace(I18_BUILDING_ORACLE,"Oracle");
  data.emplace(I18_BUILDING_ALCHEMISTS_GUILD,"Alchemists Guild");
  data.emplace(I18_BUILDING_WIZARDS_GUILD,"Wizards Guild");
  data.emplace(I18_BUILDING_STABLE,"Stable");
  data.emplace(I18_BUILDING_FANTASTIC_STABLE,"Fantastic Stable");
  data.emplace(I18_BUILDING_BARRACKS,"Barracks");
  data.emplace(I18_BUILDING_ARMORY,"Armory");
  data.emplace(I18_BUILDING_FIGHTERS_GUILD,"Fighters Guild");
  data.emplace(I18_BUILDING_ARMORERS_GUILD,"Armorers Guild");
  data.emplace(I18_BUILDING_WAR_COLLEGE,"War College");
  data.emplace(I18_BUILDING_SHIP_WRIGHTS_GUILD,"Ship Wrights Guild");
  data.emplace(I18_BUILDING_SHIP_YARD,"Ship Yard");
  data.emplace(I18_BUILDING_MARITIME_GUILD,"Maritime Guild");
  data.emplace(I18_BUILDING_MARKETPLACE,"Marketplace");
  data.emplace(I18_BUILDING_BANK,"Bank");
  data.emplace(I18_BUILDING_MERCHANTS_GUILD,"Merchants Guild");
  data.emplace(I18_BUILDING_GRANARY,"Granary");
  data.emplace(I18_BUILDING_FARMERS_MARKET,"Farmers Market");
  data.emplace(I18_BUILDING_LIBRARY,"Library");
  data.emplace(I18_BUILDING_UNIVERSITY,"University");
  data.emplace(I18_BUILDING_SAGES_GUILD,"Sages Guild");
  data.emplace(I18_BUILDING_MINERS_GUILD,"Miners Guild");
  data.emplace(I18_BUILDING_MECHANICIANS_GUILD,"Mechanicians Guild");
  data.emplace(I18_BUILDING_SAWMILL,"Sawmill");
  data.emplace(I18_BUILDING_FORESTERS_GUILD,"Forester's Guild");
  data.emplace(I18_BUILDING_TRADE_GOODS,"Trade Goods");
  data.emplace(I18_BUILDING_HOUSING,"Housing");
  data.emplace(I18_BUILDING_MAGE_FORTRESS,"Fortress");
  data.emplace(I18_BUILDING_SUMMONING_CIRCLE,"Summoning Circle");
  
  data.emplace(I18_PLACEHOLDER, "Placeholder");
}