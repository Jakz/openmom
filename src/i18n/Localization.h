//
//  Localization.h
//  OpenMoM
//
//  Created by Jack on 7/17/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _LOCALIZATION_H_
#define _LOCALIZATION_H_

#include "Common.h"

#include <unordered_map>
#include <string>

enum class UnitID : u16;
enum class SkillBase : u16;

enum class I18 : u16
{
  CITY_HAMLET,
  CITY_VILLAGE,
  CITY_TOWN,
  CITY_CITY,
  CITY_CAPITOL,

  
  MESSAGE_NEW_BUILDING,
  
  RACE_NAME_BARBARIANS,
  RACE_NAME_BEASTMEN,
  RACE_NAME_DARK_ELVES,
  RACE_NAME_DRACONIANS,
  RACE_NAME_DWARVES,
  RACE_NAME_GNOLLS,
  RACE_NAME_HALFLINGS,
  RACE_NAME_HIGH_ELVES,
  RACE_NAME_HIGH_MEN,
  RACE_NAME_KLACKONS,
  RACE_NAME_LIZARDMEN,
  RACE_NAME_NOMADS,
  RACE_NAME_ORCS,
  RACE_NAME_TROLLS,
  
  UNIT_NAME_BARBARIANS,
  UNIT_NAME_BEASTMEN,
  UNIT_NAME_DARK_ELVES,
  UNIT_NAME_DRACONIANS,
  UNIT_NAME_DWARVES,
  UNIT_NAME_GNOLLS,
  UNIT_NAME_HALFLINGS,
  UNIT_NAME_HIGH_ELVES,
  UNIT_NAME_HIGH_MEN,
  UNIT_NAME_KLACKONS,
  UNIT_NAME_LIZARDMEN,
  UNIT_NAME_NOMADS,
  UNIT_NAME_ORCS,
  UNIT_NAME_TROLLS,
  
  WIZARD_NAME_MERLIN,
  WIZARD_NAME_RAVEN,
  WIZARD_NAME_SHAREE,
  WIZARD_NAME_LO_PAN,
  WIZARD_NAME_JAFAR,
  WIZARD_NAME_OBERIC,
  WIZARD_NAME_RJAK,
  WIZARD_NAME_SSS_RA,
  WIZARD_NAME_TAURON,
  WIZARD_NAME_FREYA,
  WIZARD_NAME_HORUS,
  WIZARD_NAME_ARIEL,
  WIZARD_NAME_TLALOC,
  WIZARD_NAME_KALI,
  
  BUILDING_BUILDERS_HALL,
  BUILDING_SMITHY,
  BUILDING_CITY_WALLS,
  BUILDING_SHRINE,
  BUILDING_TEMPLE,
  BUILDING_PARTHENON,
  BUILDING_CATHEDRAL,
  BUILDING_ANIMISTS_GUILD,
  BUILDING_ORACLE,
  BUILDING_ALCHEMISTS_GUILD,
  BUILDING_WIZARDS_GUILD,
  BUILDING_STABLE,
  BUILDING_FANTASTIC_STABLE,
  BUILDING_BARRACKS,
  BUILDING_ARMORY,
  BUILDING_FIGHTERS_GUILD,
  BUILDING_ARMORERS_GUILD,
  BUILDING_WAR_COLLEGE,
  BUILDING_SHIP_WRIGHTS_GUILD,
  BUILDING_SHIP_YARD,
  BUILDING_MARITIME_GUILD,
  BUILDING_MARKETPLACE,
  BUILDING_BANK,
  BUILDING_MERCHANTS_GUILD,
  BUILDING_GRANARY,
  BUILDING_FARMERS_MARKET,
  BUILDING_LIBRARY,
  BUILDING_UNIVERSITY,
  BUILDING_SAGES_GUILD,
  BUILDING_MINERS_GUILD,
  BUILDING_MECHANICIANS_GUILD,
  BUILDING_SAWMILL,
  BUILDING_FORESTERS_GUILD,
  BUILDING_TRADE_GOODS,
  BUILDING_HOUSING,
  BUILDING_MAGE_FORTRESS,
  BUILDING_SUMMONING_CIRCLE,
  
  UNIT_SPEARMEN,
  UNIT_SWORDSMEN,
  UNIT_BOWMEN,
  UNIT_CAVALRY,
  UNIT_SHAMAN,
  UNIT_HALBERDIERS,
  UNIT_PRIESTS,
  UNIT_MAGICIANS,
  UNIT_ENGINEERS,
  UNIT_SETTLERS,
  
  UNIT_BERSERKERS,
  
  HERO_DWARF,
  HERO_ORC_WARRIOR,
  
  UNIT_MAGIC_SPIRIT,
  UNIT_HELL_HOUNDS,
  UNIT_GREAT_DRAKE,
  
  SPELL_BLESS,
  SPELL_ENDURANCE,
  SPELL_GUARDIAN_SPIRIT,
  SPELL_HEALING,
  SPELL_HEROISM,
  SPELL_HOLY_ARMOR,
  SPELL_HOLY_WEAPON,
  SPELL_JUST_CAUSE,
  SPELL_STAR_FIRES,
  SPELL_TRUE_LIGHT,
  
  SPELL_CORRUPTION,
  SPELL_CHANGE_TERRAIN,
  SPELL_RAISE_VOLCANO,
  
  EVENT_BAD_MOON,
  EVENT_RED_CONJUNCTION,
  EVENT_GREEN_CONJUNCTION,
  EVENT_BLUE_CONJUNCTION,
  EVENT_DEPLETION,
  EVENT_DIPLOMATIC_MARRIAGE,
  EVENT_DISJUNCTION,
  EVENT_DONATION,
  EVENT_EARTHQUAKE,
  EVENT_THE_GIFT,
  EVENT_GOOD_MOON,
  EVENT_GREAT_METEOR,
  EVENT_MANA_SHORT,
  EVENT_NEW_MINERALS,
  EVENT_PIRACY,
  EVENT_PLAGUE,
  EVENT_POPULATION_BOOM,
  EVENT_REBELLION,
  
  ITEM_SWORD,
  ITEM_MACE,
  ITEM_AXE,
  ITEM_BOW,
  ITEM_STAFF,
  ITEM_WAND,
  ITEM_AMULET,
  ITEM_RING,
  ITEM_CLOAK,
  ITEM_GAUNTLET,
  ITEM_HELM,
  ITEM_ORB,
  ITEM_SHIELD,
  ITEM_CHAIN_MAIL,
  ITEM_PLATE_MAIL,
  
  PLACEHOLDER
};

namespace std
{
  template<>
  struct hash<I18>
  {
    std::size_t operator()(const I18& k) const { return static_cast<u16>(k); }
  };
  
  template<>
  struct hash<UnitID>
  {
    std::size_t operator()(const UnitID& k) const { return static_cast<u16>(k); }
  };
  
  template<>
  struct hash<SkillBase>
  {
    std::size_t operator()(const SkillBase& k) const { return static_cast<u16>(k); }
  };
  
  template<>
  struct hash<TileType>
  {
    std::size_t operator()(const TileType& k) const { return static_cast<u16>(k); }
  };
}

class i18n
{
  private:
    static std::unordered_map<I18, const std::string> data;
    static std::unordered_map<UnitID, const std::string> units;
    static std::unordered_map<SkillBase, const std::string> skills;
    static std::unordered_map<I18, I18> buildingDescs;
    static std::unordered_map<TileType, std::vector<std::string> > surveyorDescs;
  
  public:
    static const std::string& s(I18 ident) { return data[ident]; }
    static const std::string& s(UnitID unit) { return units[unit]; }
    static const std::string& s(SkillBase skill) { return skills[skill]; }
  
    static I18 buildingDesc(I18 b) { return buildingDescs[b]; }
    static const std::vector<std::string> surveyorDesc(TileType type) { return surveyorDescs[type]; }

    static constexpr const I18 CITY_SIZE_NAMES[] { I18::CITY_HAMLET, I18::CITY_VILLAGE, I18::CITY_TOWN, I18::CITY_CITY, I18::CITY_CAPITOL };

};


#endif
