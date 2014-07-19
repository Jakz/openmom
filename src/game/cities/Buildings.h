#ifndef _BUILDINGS_H_
#define _BUILDINGS_H_

#include "Common.h"

#include <string>

enum I18 : u16;

enum BuildingID : u8
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
    enum Type
    {
      TRADE_GOODS = 0,
      HOUSING,
      BUILDING,
      UNIT,
      HERO
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
    const I18 name;
    const u16 cost;
    const Upkeep upkeep;
  
    Building(I18 name, u16 cost, u16 gupkeep, u16 mupkeep) : name(name), cost(cost), upkeep(Upkeep(gupkeep,mupkeep)) { }
  
    const std::string& productionName() const override;
    u16 productionCost() const override { return cost; }
    const Upkeep& productionUpkeep() const override { return upkeep; }
    Productable::Type productionType() const override { return BUILDING; }
  
    struct ComparatorByName : public std::binary_function<const Building*, const Building*, bool>
    {
      bool operator() (const Building *b1, const Building* b2)
      {
        return b1->productionName().compare(b2->productionName()) < 0;
      }
    };
  
    const Building* get(BuildingID b) { return &buildings[b]; }
};

#endif
