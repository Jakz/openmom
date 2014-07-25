#include "CityScape.h"

#include "Texture.h"
#include "Buildings.h"
#include "City.h"
#include "Gfx.h"

using namespace std;

unordered_map<const Building*, CityScape::BuildingSpecs> CityScape::specs = {
  {Building::MAGE_FORTRESS,      BuildingSpecs(SpriteInfo(TextureID::CITY_FORTRESS,   -1, -1), 0, 2, 3)},
  {Building::BUILDERS_HALL,      BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1, 0,  0), 27, 2, 3)},
  {Building::SMITHY,             BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_2, 4, -1), 25, 2, 2)},
  {Building::SHRINE,             BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  2, 0), 32, 3, 2)}, /*24*/
  {Building::TEMPLE,             BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  2, 1), 36, 3, 2)},
  {Building::PARTHENON,          BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  2, 2), 40, 3, 3)},
  {Building::CATHEDRAL,          BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  4, 0), 34, 3, 3)},
  {Building::ANIMISTS_GUILD,     BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  0, 3), 24, 2, 2)},
  {Building::ORACLE,             BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  1, 3), 25, 2, 2)},
  {Building::ALCHEMISTS_GUILD,   BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_2,  9,-1), 14, 1, 1)},
  {Building::WIZARDS_GUILD,      BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_2, 10,-1), 17, 2, 2)},
  {Building::STABLE,             BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_2,  5,-1), 40, 3, 3)},
  {Building::FANTASTIC_STABLE,   BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_2,  6,-1), 40, 3, 3)},
  {Building::BARRACKS,           BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_2,  2,-1), 30, 2, 3)},
  {Building::ARMORY,             BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  0, 1), 25, 2, 2)},
  {Building::FIGHTERS_GUILD,     BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_2,  1,-1), 35, 3, 2)},
  {Building::ARMORERS_GUILD,     BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_2,  0,-1), 36, 3, 2)},
  {Building::WAR_COLLEGE,        BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  0, 2), 36, 3, 2)},
  {Building::SHIP_WRIGHTS_GUILD, BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  0, 4), 22, 2, 3)},
  {Building::SHIP_YARD,          BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  1, 0), 32, 2, 3)},
  {Building::MARITIME_GUILD,     BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_2,  7,-1), 30, 2, 3)},
  {Building::MARKETPLACE,        BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  2, 3), 25, 2, 2)},
  {Building::BANK,               BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  2, 4), 26, 2, 2)},
  {Building::MERCHANTS_GUILD,    BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  3, 0), 25, 2, 2)},
  {Building::GRANARY,            BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  3, 1), 17, 2, 2)},
  {Building::FARMERS_MARKET,     BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  3, 2), 24, 2, 2)},
  {Building::LIBRARY,            BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  1, 1), 36, 3, 2)},
  {Building::UNIVERSITY,         BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  1, 4), 36, 3, 2)},
  {Building::SAGES_GUILD,        BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  1, 2), 25, 2, 2)},
  {Building::MINERS_GUILD,       BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  3, 4), 23, 2, 2)},
  {Building::MECHANICIANS_GUILD, BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  3, 3), 21, 2, 2)},
  {Building::SAWMILL,            BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_2,  8,-1), 25, 2, 3)},
  {Building::FORESTERS_GUILD,    BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  4, 1), 25, 2, 2)},
  
  {Building::HOUSING,            BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  4, 4), 40, 2, 2)},
  {Building::TRADE_GOODS,        BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  4, 3), 40, 2, 2)},
  {Building::CITY_WALLS,         BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  5, 2), 40, 2, 2)},
  
  {Building::SUMMONING_CIRCLE,   BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  4, 2), 25, 3, 2)}
};

void CityScape::draw(const City *city, LocalPlayer *player)
{
  Plane p = city->getPosition().plane;
  Gfx::draw(TextureID::CITY_BACKGROUND, 4, p, 3, 101);
  
  // draw correct far sight according to city features: grasslands, hills, mountain */
  if (city->hasPlacement(CITY_BY_MOUNTAIN))
    Gfx::draw(TextureID::CITY_LANDSCAPE, 2+3*p, 0, 3, 101);
  else if (city->hasPlacement(CITY_BY_HILL))
    Gfx::draw(TextureID::CITY_LANDSCAPE, 1+3*p, 0, 3, 101);
  else
    Gfx::draw(TextureID::CITY_LANDSCAPE, 0+3*p, 0, 3, 101);
  
  // draw correct far spell effects for cloud of darkness / heavenly light / etc
//  if (city->hasSpell(Spells::CLOUD_OF_SHADOW))   // TODO: MISSING SPELL
  //  Gfx::draw(TextureID::CITY_LANDSCAPE, 6+3*p, 0, 3, 101);
  
  
  Gfx::draw(TextureID::CITY_ROADS, 0, 0, 5, 118);
  
  HouseType ht = city->race.houseType;
  
  // TODO requiring CityLayout
}

s16 CityScape::buildingHeight(const Building *building)
{
  return Texture::get(specs[building].info.texture).h;
}

void CityScape::drawBuilding(const Building *building, s16 x, s16 y)
{
  const BuildingSpecs& spec = specs[building];
  
  if (spec.animated)
    Gfx::drawAnimated(spec.info.texture, spec.info.x, x, y - buildingHeight(building), 0);
  else
    Gfx::draw(spec.info, x, y - buildingHeight(building));
}

void CityScape::drawBuildingCentered(const Building *building, s16 x, s16 y)
{
  if (building == Building::TRADE_GOODS || building == Building::HOUSING || building == Building::CITY_WALLS)
    drawBuilding(building, x, y);
  else
  {
    const BuildingSpecs& spec = specs[building];
    
    if (spec.animated)
      Gfx::drawAnimated(spec.info.texture, spec.info.x, x + (40 - spec.pixelWidth)/2, y - buildingHeight(building), 0);
    else
      Gfx::draw(spec.info, x + (40 - spec.pixelWidth)/2, y - buildingHeight(building));
  }
}