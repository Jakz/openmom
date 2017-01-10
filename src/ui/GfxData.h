//
//  GfxData.h
//  OpenMoM
//
//  Created by Jack on 7/21/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _GFX_DATA_H_
#define _GFX_DATA_H_

#include "Common.h"

#include "Items.h"

#include <unordered_map>

enum class Ranged : u16;
enum class Property : u8;
enum class RaceID : u8;
enum class HouseType : u8;

class HeroSpec;
class UnitSpec;
class Spell;

class TileGfxSpec
{
private:
  
  const std::vector<Color> colors[PLANE_COUNT];
  
public:
  TileGfxSpec(u8 row, u8 count, Color c1, Color c2) : row(row), count(count), colors{{c1},{c2}} { }
  TileGfxSpec(u8 row, u8 count, std::initializer_list<Color> c1, std::initializer_list<Color> c2) : row(row), count(count), colors{c1,c2} { }
  
  
  u8 row;
  u8 count;
  
  Color minimapColor(Plane plane, u16 i = 0) const
  {
    if (colors[plane].size() > 1)
      return colors[plane][i];
    else
      return colors[plane][0];
  }
  
  
};

struct UnitGfxSpec
{
  SpriteInfo still;
  SpriteInfo fullFigure;
  SpriteInfo summonFigure;
  bool isFlyingFigure;
  
  UnitGfxSpec(SpriteInfo still, SpriteInfo fullFigure, bool isFlyingFigure = false) : still(still), fullFigure(fullFigure), isFlyingFigure(isFlyingFigure) { }
  UnitGfxSpec(SpriteInfo still, SpriteInfo fullFigure, SpriteInfo summonFigure, bool isFlyingFigure = false) : still(still), fullFigure(fullFigure), summonFigure(summonFigure), isFlyingFigure(isFlyingFigure) { }

};

struct WizardGfxSpec
{
  SpriteInfo portraitSmall;
  SpriteInfo portraitLarge;
  SpriteInfo diplomacyMood;
  SpriteInfo gemmedPortrait;
  SpriteInfo researchPose;
  SpriteInfo summonPose;
  
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
};

struct UnitPropGfxSpec
{
  enum class Type
  {
    SWORD,
    MITHRIL_SWORD,
    MAGIC_SWORD,
    ADAMANTIUM_SWORD,
    RANGED_MAGIC,
    RANGED_BOW,
    RANGED_ROCK,
    RANGED_BREATH,
    RANGED_THROWN,
    SHIELDS,
    HITS,
    RESIST,
    TO_HIT,
    ARCANE,
    CHAOS,
    DEATH,
    LIFE,
    NATURE,
    SORCERY,
    FIGURES,
    TO_DEF,
    UNKNOWN,
    SHIELDS_RANGED
  };
  
  SpriteInfo standard;
  SpriteInfo bonus;
  SpriteInfo borderless;
};

using SkillGfxSpec = SpriteInfo;

template<typename K, typename V, size_t SIZE>
class enum_simple_map
{
public:
  static constexpr size_t size = SIZE;
  using init_element = std::pair<K, V>;
  
  struct dummy_pair
  {
    const V& second;
    const dummy_pair* operator->() const { return this; }
  };
  
private:
  std::array<V, size> data;
public:
  enum_simple_map(const std::initializer_list<init_element>& elements)
  {
    assert(elements.size() == size);
    size_t i = 0;
    for (auto it = elements.begin(); it != elements.end(); ++i, ++it)
    {
      assert(static_cast<size_t>(it->first) < size);
      data[static_cast<size_t>(it->first)] = it->second;
    }
  }
  
  const V& operator[](K key) const { return data[static_cast<size_t>(key)]; }
  dummy_pair find(K key) const { return { operator[](key) }; }
};

template<typename K, typename V, size_t SIZE = 0> using gfx_map = typename std::conditional<(std::is_enum<K>::value && SIZE > 0), enum_simple_map<K, V, SIZE>, std::unordered_map<K,V>>::type;


class GfxData
{
private:
  static gfx_map<WizardID, WizardGfxSpec, 14> wizardSpecs;
  static gfx_map<PlayerColor, PlayerGfxSpec, 6> playerSpecs;
  static gfx_map<RaceID, RaceGfxSpec, 14> raceSpecs;
  static gfx_map<HouseType, RaceHouseGfxSpec, 3> raceHouseSpecs;

  
  static gfx_map<const UnitSpec*, UnitGfxSpec> unitSpecs;
  static gfx_map<const UnitSpec*, SpriteInfo> heroPortraits;
  
  static gfx_map<School, SchoolGfxSpec, 6> schoolSpecs;
  static gfx_map<UpkeepSymbol, UpkeepSymbolSpec, 5> upkeepSymbolSpec;
  
  static const TileGfxSpec specs[];
  static constexpr s8 RANGED_INDEX[] = {-1,6,5,6,14,15,16,17,18};
  static constexpr s8 PROPERTY_INDEX[] = {-1,-1,-1,-1,9,22,14,15,16,17,18};
  
  static std::unordered_map<const Spell*, sprite_ref> specialSpellGfxEffects;
  
  template<typename K, typename V> static gfx_map<K,V>& containerFor();
  template<typename K, typename V> static void registerData(K k, V v) { containerFor<K,V>().insert(std::make_pair(k,v)); }
  
  
public:
  static const TileGfxSpec& tileGfxSpec(TileType type) { return specs[type]; }
  static s8 rangedGfxIndex(Ranged ranged) { return RANGED_INDEX[static_cast<u8>(ranged)]; }
  static s8 propertyGfxIndex(Property property) { return PROPERTY_INDEX[static_cast<u8>(property)]; }
  
  static SpriteInfo itemGfxSpec(items::TypeID type, int index);
  
  static const PlayerGfxSpec& playerGfxSpec(PlayerColor color) { return playerSpecs.find(color)->second; }
  static const WizardGfxSpec& wizardGfxSpec(WizardID wizard) { return wizardSpecs.find(wizard)->second; }
  static const RaceGfxSpec& raceGfxSpec(RaceID race) { return raceSpecs.find(race)->second; }
  static const RaceHouseGfxSpec& raceHouseGfxSpec(HouseType race) { return raceHouseSpecs.find(race)->second; }

  static const SchoolGfxSpec& schoolGfxSpec(School school) { return schoolSpecs.find(school)->second; }
  static const UpkeepSymbolSpec& upkeepGfxSpec(UpkeepSymbol symbol) { return upkeepSymbolSpec.find(symbol)->second; }

  
  static const UnitGfxSpec& unitGfxSpec(const UnitSpec* spec);
  static SpriteInfo heroGfxSpec(const UnitSpec* spec);
  
  static SpriteInfo specialSpellGfxEffect(const Spell* spell);
  
  template<typename K, typename V> static const V& gfxDataFor(K k) { return containerFor<K,V>(k); }
  
  friend class Data;
};

#endif
