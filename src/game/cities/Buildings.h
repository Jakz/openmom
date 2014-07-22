#ifndef _BUILDINGS_H_
#define _BUILDINGS_H_

#include "Common.h"

#include <string>

enum class BuildingID : u8
{
  BUILDERS_HALL,
  SMITHY,
  CITY_WALLS,
  SHRINE,
  TEMPLE,
  PARTHENON,
  CATHEDRAL,
  ANIMISTS_GUILD,
  ORACLE,
  ALCHEMISTS_GUILD,
  WIZARDS_GUILD,
  STABLE,
  FANTASTIC_STABLE,
  BARRACKS,
  ARMORY,
  FIGHTERS_GUILD,
  ARMORERS_GUILD,
  WAR_COLLEGE,
  SHIP_WRIGHTS_GUILD,
  SHIP_YARD,
  MARITIME_GUILD,
  MARKETPLACE,
  BANK,
  MERCHANTS_GUILD,
  GRANARY,
  FARMERS_MARKET,
  LIBRARY,
  UNIVERSITY,
  SAGES_GUILD,
  MINERS_GUILD,
  MECHANICIANS_GUILD,
  SAWMILL,
  FORESTERS_GUILD,
  TRADE_GOODS,
  HOUSING,
  MAGE_FORTRESS,
  SUMMONING_CIRCLE
};

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
    static const Building buildings[];
  
  public:
    const BuildingID ident;
    const I18 name;
    const u16 cost;
    const Upkeep upkeep;
  
    Building(BuildingID ident, I18 name, u16 cost, u16 gupkeep, u16 mupkeep) : ident(ident), name(name), cost(cost), upkeep(Upkeep(gupkeep,mupkeep)) { }
  
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
  
    static const Building* get(BuildingID b) { return &buildings[static_cast<u8>(b)]; }
};

#endif
