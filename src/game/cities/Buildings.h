#ifndef _BUILDINGS_H_
#define _BUILDINGS_H_

#include "Common.h"

#include <string>

class Productable
{
  public:
    enum class Type : u8
    {
      TRADE_GOODS = 0,
      HOUSING,
      BUILDING,
      UNIT,
      HERO,
      SUMMON
    };
  
  virtual const std::string& productionName() const = 0;
  virtual u16 productionCost() const = 0;
  virtual const Upkeep& productionUpkeep() const = 0;
  virtual Type productionType() const = 0;
  
};

class Building : public Productable
{
  private:
  
  public:
    const I18 name;
    const I18 desc;
    const u16 cost;
    const Upkeep upkeep;
  
    Building(I18 name, I18 desc, u16 cost, u16 gupkeep, u16 mupkeep) : name(name), desc(desc), cost(cost), upkeep(Upkeep(gupkeep,mupkeep)) { }
  
    const std::string& productionName() const override;
    u16 productionCost() const override { return cost; }
    const Upkeep& productionUpkeep() const override { return upkeep; }
    Productable::Type productionType() const override { return Type::BUILDING; }
  
    struct ComparatorByName : public std::binary_function<const Productable*, const Productable*, bool>
    {
      bool operator() (const Productable *b1, const Productable* b2)
      {
        return b1->productionName().compare(b2->productionName()) < 0;
      }
    };
  
  static const Building* BUILDERS_HALL;
  static const Building* SMITHY;
  static const Building* CITY_WALLS;
  static const Building* SHRINE;
  static const Building* TEMPLE;
  static const Building* PARTHENON;
  static const Building* CATHEDRAL;
  static const Building* ANIMISTS_GUILD;
  static const Building* ORACLE;
  static const Building* ALCHEMISTS_GUILD;
  static const Building* WIZARDS_GUILD;
  static const Building* STABLE;
  static const Building* FANTASTIC_STABLE;
  static const Building* BARRACKS;
  static const Building* ARMORY;
  static const Building* FIGHTERS_GUILD;
  static const Building* ARMORERS_GUILD;
  static const Building* WAR_COLLEGE;
  static const Building* SHIP_WRIGHTS_GUILD;
  static const Building* SHIP_YARD;
  static const Building* MARITIME_GUILD;
  static const Building* MARKETPLACE;
  static const Building* BANK;
  static const Building* MERCHANTS_GUILD;
  static const Building* GRANARY;
  static const Building* FARMERS_MARKET;
  static const Building* LIBRARY;
  static const Building* UNIVERSITY;
  static const Building* SAGES_GUILD;
  static const Building* MINERS_GUILD;
  static const Building* MECHANICIANS_GUILD;
  static const Building* SAWMILL;
  static const Building* FORESTERS_GUILD;
  static const Building* TRADE_GOODS;
  static const Building* HOUSING;
  static const Building* MAGE_FORTRESS;
  static const Building* SUMMONING_CIRCLE;
  
  static const size_t COUNT; // TODO: ugly
  static const Building* buildings[];
};

#endif
