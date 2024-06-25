//
//  Localization.h
//  OpenMoM
//
//  Created by Jack on 7/17/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _LOCALIZATION_H_
#define _LOCALIZATION_H_

#include "common/Common.h"

#include <unordered_map>
#include <string>

enum class RaceID : u8;
enum class SkillBase : u16;

enum class I18 : u32
{
  CITY_HAMLET,
  CITY_VILLAGE,
  CITY_TOWN,
  CITY_CITY,
  CITY_CAPITOL,
  
  
  MESSAGE_NEW_BUILDING,
  MESSAGE_OUTPOST_GROWN_TO_CITY,
  
  SURVEYOR_CITY_FORBID_DISTANCE,
  SURVEYOR_CITY_FORBID_WATER,
  SURVEYOR_MAX_POPULATION,
  SURVEYOR_PRODUCTION_BONUS,

  HERO_DWARF,
  HERO_ORC_WARRIOR,
  
  UNIT_MAGIC_SPIRIT,
  UNIT_HELL_HOUNDS,
  UNIT_GREAT_DRAKE,
  
  SPELL_KIND_SUMMONING,
  SPELL_KIND_SPECIAL,
  SPELL_KIND_CITY,
  SPELL_KIND_ENCHANTMENT,
  SPELL_KIND_UNIT_SPELL,
  SPELL_KIND_COMBAT_SPELL,
  SPELL_KIND_RESEARCH, // used for BookView
  SPELL_KIND_SPELLS,
  
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
  
  RELATION_HATRED,
  RELATION_TROUBLED,
  RELATION_TENSE,
  RELATION_RESTLESS,
  RELATION_UNEASY,
  RELATION_NEUTRAL,
  RELATION_RELAXED,
  RELATION_CALM,
  RELATION_PEACEFUL,
  RELATION_FRIENDLY,
  RELATION_HARMONY,
  
  UI_UNIT_DETAIL_MOVES,
  UI_UNIT_DETAIL_UPKEEP,
  
  EMPTY,
  
  PLACEHOLDER,
  
  FIRST_AVAILABLE_INDEX
};

namespace lbx { class Repository; }

class i18n
{
public:
  template<typename T> using strings_table = std::unordered_map<T, std::string, enum_hash>;
  using reverse_strings_table = std::unordered_map<std::string, I18>;

private:
  static I18 customMappingFreeIndex;
  
  static strings_table<SkillBase> skills;
  static std::unordered_map<TileType, std::vector<std::string>, enum_hash> surveyorDescs;
  
  static reverse_strings_table& getMapping();
  static strings_table<I18>& getData();
  
public:
  static void mapCustomEntry(std::string key, std::string value);
  static I18 keyForString(const std::string& key);
  
  static const char* c(I18 ident) { return getData()[ident].c_str(); }
  static const std::string& s(I18 ident) { return getData()[ident]; }
  static const std::string& s(const std::string& key) { return s(keyForString(key)); }
  static const std::string& s(SkillBase skill) { return skills[skill]; }
    
  static const std::vector<std::string> surveyorDesc(TileType type) { return surveyorDescs[type]; }
  
  static const enum_simple_map<School, I18, 6> schools;
  static constexpr const I18 CITY_SIZE_NAMES[] { I18::CITY_HAMLET, I18::CITY_VILLAGE, I18::CITY_TOWN, I18::CITY_CITY, I18::CITY_CAPITOL };
  static constexpr I18 SPELL_KIND_NAMES[] = { I18::SPELL_KIND_SUMMONING, I18::SPELL_KIND_SPECIAL, I18::SPELL_KIND_CITY, I18::SPELL_KIND_ENCHANTMENT, I18::SPELL_KIND_UNIT_SPELL, I18::SPELL_KIND_COMBAT_SPELL };
  
#if defined(DEBUG)
  static std::vector<std::string> unlocalizedEntries();
#endif
  
  friend class lbx::Repository;
};

//TODO: this constructs a std::string which is not efficient, we should find a way to invoke keyForString on string_view type
inline I18 operator"" _i18k(const char* key, size_t size) { return i18n::keyForString(key); }
inline std::string operator"" _i18(const char* key, size_t size) { return i18n::s(key); }


struct i18n_cached_key
{
private:
  std::string ident;
  I18 key;
  
public:
  i18n_cached_key(const std::string& ident) : ident(ident), key(i18n::keyForString(ident)) { }
};

#include "common/fmt/format.h"

namespace fmt
{
  template <>
  struct formatter<I18> : formatter<string_view>
  {
    template <typename CTX>
    auto format(I18 i18, CTX& ctx)
    {
      return formatter<string_view>::format(i18n::s(i18), ctx);
    }
  };
}

#endif
