//
//  GfxData.h
//  OpenMoM
//
//  Created by Jack on 7/21/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _GFX_DATA_H_
#define _GFX_DATA_H_

#include "common/Common.h"

#include "Items.h"

#include <unordered_map>

enum class Ranged : u16;
enum class Property : u8;
enum class RaceID : u8;
enum class HouseType : u8;
enum class MeleeType;

class HeroSpec;
class UnitSpec;
class Spell;

enum class CombatProjectile : sprite_ref
{
  LIGHTING = LBXI(CMBMAGIC, 0),
  ARROW = LBXI(CMBMAGIC, 8),
  FIREBALL = LBXI(CMBMAGIC, 16),
  ILLUSION = LBXI(CMBMAGIC, 24),
  ROCKS = LBXI(CMBMAGIC, 32),
  SLING = LBXI(CMBMAGIC, 40),
  DEATHBOLT = LBXI(CMBMAGIC, 48),
  ICEBOLT = LBXI(CMBMAGIC, 56),
  SCATTER = LBXI(CMBMAGIC, 64),
};

struct UnitGfxSpec
{
  I18 name;
  SpriteInfo still;
  SpriteInfo fullFigure;
  
  union
  {
    struct
    {
      SpriteInfo summonFigure;
    } fantastic;
    struct
    {
      SpriteInfo portrait;
    } hero;
  };
  
  bool isFlyingFigure;

  UnitGfxSpec(I18 name, SpriteInfo still, SpriteInfo fullFigure, bool isFlyingFigure = false) : name(name), still(still), fullFigure(fullFigure), isFlyingFigure(isFlyingFigure) { }
  UnitGfxSpec(I18 name, SpriteInfo still, SpriteInfo fullFigure, SpriteInfo summonFigure, bool isFlyingFigure = false) : name(name), still(still), fullFigure(fullFigure), fantastic({summonFigure}), isFlyingFigure(isFlyingFigure) { }

};

struct WizardGfxSpec
{
  I18 name;
  SpriteInfo portraitSmall;
  SpriteInfo portraitLarge;
  
  SpriteInfo gemmedPortrait;
  SpriteInfo researchPose;
  SpriteInfo summonPose;
  
  struct
  {
    SpriteInfo mood;
    SpriteInfo speaking;
    SpriteInfo fadeIn;
  } diplomacy;
  
  SpriteInfo getGemmedPortrait(PlayerColor color) const;
};

struct PlayerGfxSpec
{
  SpriteInfo gem;
  SpriteInfo unitBack;
  SpriteInfo nodeAura;

  struct
  {
    SpriteInfo pact;
    SpriteInfo alliance;
    SpriteInfo war;
  } diplomacy;
  
  struct
  {
    SpriteInfo flag;
    Color color;
  } cartographer;
  
  struct
  {
    Color color;
  } astrologer;
  
  struct
  {
    
  } units;
};

struct SchoolGfxSpec
{
  Color color;
  SpriteInfo summonPalette;
  SpriteInfo symbol;
  SpriteInfo researchFamiliar;
  SpriteInfo newGameBook; // 3 entries starting from this
  SpriteInfo book;
  SpriteInfo overlandUnitSpellCast;
  
  I18 name;
  
  struct {
    SpriteInfo gfx;
    Point offset;
  } messageMascot;
};


enum class UpkeepSymbol
{
  FOOD,
  WORK,
  GOLD,
  MANA,
  RESEARCH
};

struct UpkeepSymbolSpec
{
  SpriteInfo single;
  SpriteInfo singleBlack;
  SpriteInfo tenTimes;
  SpriteInfo tenTimesBlack;
};

struct RaceGfxSpec
{
  I18 name;
  I18 unitName;
  HouseType houseType;
  SpriteInfo cityWorker;
  SpriteInfo cityFarmer;
  SpriteInfo cityRebel;
};

struct RaceHouseGfxSpec
{
  SpriteInfo outpostEmpty;
  SpriteInfo outpostFull;
  SpriteInfo cityScapeHouse; // 5 relative
  SpriteInfo housingBuilding;
  SpriteInfo combatHouse;
};

struct PlaceGfxSpec
{
  struct
  {
    SpriteInfo icon;
    I18 firstName;
    I18 lastName;
  } enterMessage;
};

struct SkillGfxSpec
{
  SpriteInfo icon;
  help_ref help;
};


template<typename K, typename V, typename M, template<class> class S>
class multi_enum_map
{
public:
  using mapped_type = V;
  using key_type = K;
  using value_type = std::pair<key_type, mapped_type>;
  using map_t = std::unordered_map<key_type, mapped_type>;
  using iterator = typename map_t::const_iterator;
  
private:
  template<typename T> static constexpr K build_key(T value, M master) {
    static_assert(sizeof(M) + sizeof(T) <= sizeof(K), "Size of master enum + specific enum must be small enough");
    return (static_cast<K>(master) << (sizeof(K)*8)/2) | static_cast<K>(value);
  }
  
  template<typename T> static constexpr K build_key(T value) {
    return build_key(value, S<T>::value);
  }
  
  struct initializer_key
  {
    key_type key;
    template<typename T> initializer_key(M master, T value) : key(build_key(value, master)) { }
    template<typename T> initializer_key(T value) : key(build_key(value)) { }
  };
  
private:
  
  map_t data;
  
public:
  multi_enum_map() = default;
  multi_enum_map(std::initializer_list<std::pair<initializer_key, V>> data)
  {
    for (const auto& entry : data)
      this->data[entry.first.key] = entry.second;
  }
  
  template<typename T> iterator find(T value) const { return data.find(build_key(value)); }
  template<typename T> iterator find(M master, T value) const { return data.find(build_key(value, master)); }
  
  template<typename T> const V& operator[](T value) { return data[build_key(value)]; }
};

enum class multi_enum_master_key
{
  ITEM_SLOTS_IN_SKILL_VIEW,
  ITEM_SLOTS_IN_ITEMS_VIEW
};

template<typename T> struct multi_enum_specializer : std::false_type { };
template<> struct multi_enum_specializer<items::Class> { static constexpr multi_enum_master_key value = multi_enum_master_key::ITEM_SLOTS_IN_SKILL_VIEW; };

/* 
multi_enum_map<u64, Value, Master, enum_specializer> map = {
 { Foo::FOO, { 1.0f, 2.0f } },
 { Bar::BAR, { 5.0f, 7.0f } },
 { Foo::BAR, { 10.0f, 7.0f } }
 };
 */


template<typename K, typename V, size_t SIZE = 0> using gfx_map =
  typename std::conditional<(std::is_enum<K>::value && SIZE > 0),
    enum_simple_map<K, V, SIZE>,
    std::unordered_map<K, V/*, typename std::conditional<std::is_enum<K>::value, enum_hash, std::hash<K>::type>*/>
  >::type;

class UnitPropGfxMap
{
public:
  struct Data
  {
    SpriteInfo blackShadow;
    SpriteInfo blueShadow;
    SpriteInfo blueBoxed;
    SpriteInfo goldBoxed;
  };
  
private:
  enum class Type
  {
    SWORD,
    SWORD_MAGIC,
    SWORD_MITHRIL,
    SWORD_ADAMANTIUM,
    SHIELD,
    RESIST,
    HITS,
    
    RANGED_BOW,
    RANGED_ROCK,
    RANGED_MAGIC,
    
    MOVEMENT_FOOT,
    MOVEMENT_WATER,
    MOVEMENT_FLYING,
    
    COUNT,
  };
  
  gfx_map<Type, Data, static_cast<size_t>(Type::COUNT)> map;
  
public:
  UnitPropGfxMap();
  template<typename T> const Data& operator[](T property) const;
};


class GfxData
{
private:
  static const gfx_map<PlayerColor, PlayerGfxSpec, 6> playerSpecs;
  static const gfx_map<RaceID, RaceGfxSpec, 14> raceSpecs;
  static const gfx_map<HouseType, RaceHouseGfxSpec, 3> raceHouseSpecs;
  
  static const gfx_map<School, SchoolGfxSpec, 6> schoolSpecs;
  static const gfx_map<UpkeepSymbol, UpkeepSymbolSpec, 5> upkeepSymbolSpec;
  
  static const gfx_map<PlaceType, PlaceGfxSpec, 10> placeSpecs;
    
  static const UnitPropGfxMap unitPropSpecs;
  
  static constexpr s8 RANGED_INDEX[] = {-1,6,5,6,14,15,16,17,18};
  static constexpr s8 PROPERTY_INDEX[] = {-1,-1,-1,-1,9,22,14,15,16,17,18};
  
  static std::unordered_map<const Spell*, sprite_ref> specialSpellGfxEffects;
  
  template<typename K, typename V> static gfx_map<K,V>& containerFor();
  
public:
  static s8 rangedGfxIndex(Ranged ranged) { return RANGED_INDEX[static_cast<u8>(ranged)]; }
  static s8 propertyGfxIndex(Property property) { return PROPERTY_INDEX[static_cast<u8>(property)]; }
  
  static SpriteInfo itemGfxSpec(items::TypeID type, int index);
  
  static const PlayerGfxSpec& playerGfxSpec(PlayerColor color) { return playerSpecs.find(color)->second; }
  static const RaceHouseGfxSpec& raceHouseGfxSpec(HouseType race) { return raceHouseSpecs.find(race)->second; }

  static const SchoolGfxSpec& schoolGfxSpec(School school) { return schoolSpecs.find(school)->second; }
  static const UpkeepSymbolSpec& upkeepGfxSpec(UpkeepSymbol symbol) { return upkeepSymbolSpec.find(symbol)->second; }
  static const PlaceGfxSpec& placeGfxSpec(PlaceType type) { return placeSpecs.find(type)->second; }
  
  static SpriteInfo specialSpellGfxEffect(const Spell* spell);
  
  static const UnitPropGfxMap& propGfx() { return unitPropSpecs; }
  
  static const RaceGfxSpec& raceGfxSpec(const Race* race) { return gfxDataFor<const Race*, RaceGfxSpec>(race); }
  static const UnitGfxSpec& unitGfx(const UnitSpec* spec) { return gfxDataFor<const UnitSpec*, UnitGfxSpec>(spec); }
  static const WizardGfxSpec& wizardGfx(const Wizard* spec) { return gfxDataFor<const Wizard*, WizardGfxSpec>(spec); }

  
  template<typename K, typename V> static const V& gfxDataFor(K k) { return containerFor<K,V>().find(k)->second; }
  template<typename K, typename V> static void registerData(K k, V v) { containerFor<K,V>().insert(std::make_pair(k,v)); }
  
  friend class Data;
};

#endif
