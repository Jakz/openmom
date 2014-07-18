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


}