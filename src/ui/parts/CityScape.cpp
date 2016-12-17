#include "CityScape.h"

#include "Texture.h"
#include "Buildings.h"
#include "City.h"
#include "Gfx.h"
#include "Util.h"

#include <set>

using namespace std;


unordered_map<const Building*, CityLayout::BuildingSpecs> CityLayout::specs = {
  {Building::MAGE_FORTRESS,      BuildingSpecs(SpriteInfo(TextureID::CITY_FORTRESS,    0, -1),  0, 2, 3)},
  {Building::BUILDERS_HALL,      BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1, 0,  0), 27, 2, 3)},
  {Building::SMITHY,             BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_2, 4, -1), 25, 2, 2)},
  {Building::SHRINE,             BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  0, 2), 32, 3, 2)}, /*24*/
  {Building::TEMPLE,             BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  1, 2), 36, 3, 2)},
  {Building::PARTHENON,          BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  2, 2), 40, 3, 3)},
  {Building::CATHEDRAL,          BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  0, 4), 34, 3, 3)},
  {Building::ANIMISTS_GUILD,     BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  3, 0), 24, 2, 2)},
  {Building::ORACLE,             BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  3, 1), 25, 2, 2)},
  {Building::ALCHEMISTS_GUILD,   BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_2,  9,-1), 14, 1, 1)},
  {Building::WIZARDS_GUILD,      BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_2, 10,-1), 17, 2, 2)},
  {Building::STABLE,             BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_2,  5,-1), 40, 3, 3)},
  {Building::FANTASTIC_STABLE,   BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_2,  6,-1), 40, 3, 3)},
  {Building::BARRACKS,           BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_2,  2,-1), 30, 2, 3)},
  {Building::ARMORY,             BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  0, 1), 25, 2, 2)},
  {Building::FIGHTERS_GUILD,     BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_2,  1,-1), 35, 3, 2)},
  {Building::ARMORERS_GUILD,     BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_2,  0,-1), 36, 3, 2)},
  {Building::WAR_COLLEGE,        BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  2, 0), 36, 3, 2)},
  {Building::SHIP_WRIGHTS_GUILD, BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  4, 0), 22, 2, 3)},
  {Building::SHIP_YARD,          BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  0, 1), 32, 2, 3)},
  {Building::MARITIME_GUILD,     BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_2,  7,-1), 30, 2, 3)},
  {Building::MARKETPLACE,        BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  3, 2), 25, 2, 2)},
  {Building::BANK,               BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  4, 2), 26, 2, 2)},
  {Building::MERCHANTS_GUILD,    BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  0, 3), 25, 2, 2)},
  {Building::GRANARY,            BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  1, 3), 17, 2, 2)},
  {Building::FARMERS_MARKET,     BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  2, 3), 24, 2, 2)},
  {Building::LIBRARY,            BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  1, 1), 36, 3, 2)},
  {Building::UNIVERSITY,         BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  4, 1), 36, 3, 2)},
  {Building::SAGES_GUILD,        BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  2, 1), 25, 2, 2)},
  {Building::MINERS_GUILD,       BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  4, 3), 23, 2, 2)},
  {Building::MECHANICIANS_GUILD, BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  3, 3), 21, 2, 2)},
  {Building::SAWMILL,            BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_2,  8,-1), 25, 2, 3)},
  {Building::FORESTERS_GUILD,    BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  1, 4), 25, 2, 2)},
  
  {Building::HOUSING,            BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  4, 4), 40, 2, 2)},
  {Building::TRADE_GOODS,        BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  3, 4), 40, 2, 2)},
  {Building::CITY_WALLS,         BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  2, 5), 40, 2, 2)},
  
  {Building::SUMMONING_CIRCLE,   BuildingSpecs(SpriteInfo(TextureID::CITY_BUILDINGS_1,  2, 4), 25, 3, 2)}
};

map<const City*, CityLayout*> CityLayout::layouts;


static constexpr s16 river_and_ocean_tiles[2][PLANE_COUNT] = { {3, 4}, {115, 116} };


void CityLayout::draw(const City *city, LocalPlayer *player)
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
  
  const CityLayout* layout = layouts[city];
  
  for (auto &p : layout->positions)
  {
    if (p.building)
      drawBuilding(p.building, p.x, p.y);
    else
      Gfx::draw(TextureID::CITY_HOUSES, ht, p.house, p.x, p.y - Texture::get(TextureID::CITY_HOUSES)->sh());
  }
  
  if (city->hasPlacement(CITY_BY_SEA))
    Gfx::drawAnimated(LBXSpriteInfo(LBXID::CITYSCAP, river_and_ocean_tiles[city->getPosition().plane][1]), 4, 100);
  else if (!city->hasPlacement(CITY_BY_SEA) && city->hasPlacement(CITY_BY_RIVER))
    Gfx::drawAnimated(LBXSpriteInfo(LBXID::CITYSCAP, river_and_ocean_tiles[city->getPosition().plane][0]), 4, 100, 0, 5);
  
  //draw walls
  //TODO: other kinds of walls + city wall spell
  if (city->hasBuilding(Building::CITY_WALLS))
    Gfx::drawAnimated(TextureID::CITY_WALLS, 0, 3, 183, 0);
  /*if (c->hasSpell(Spells::WALL_OF_DARKNESS))   TODO MISSING SPELL
  Gfx::drawAnimated(TextureID::CITY_WALLS, 3, 3, 183, 0);*/
}

s16 CityLayout::buildingHeight(const Building *building)
{
  return Texture::get(specs[building].info.texture)->sh();
}

void CityLayout::drawBuilding(const Building *building, s16 x, s16 y)
{
  const BuildingSpecs& spec = specs[building];
  
  if (spec.animated)
    Gfx::drawAnimated(spec.info.texture, spec.info.x, x, y - buildingHeight(building), 0);
  else
    Gfx::draw(spec.info, x, y - buildingHeight(building));
}

void CityLayout::drawBuildingCentered(const Building *building, s16 x, s16 y)
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


void CityLayout::deploy()
{
  positions.clear();
  zones = getZones();
  
  set<const Building*> buildings;
  
  for (auto b : city->buildings)
    if (b != Building::CITY_WALLS)
      buildings.insert(b);
  
  if (buildings.find(Building::MAGE_FORTRESS) != buildings.end())
  {
    positions.push_back(createPosition(zones[6], 0, 0, Building::MAGE_FORTRESS));
    zones.erase(zones.begin()+6);
    buildings.erase(Building::MAGE_FORTRESS);
  }
  if (buildings.find(Building::SHIP_WRIGHTS_GUILD) != buildings.end())
  {
    positions.push_back(createPosition(zones[4], 0, 0, Building::SHIP_WRIGHTS_GUILD));
    zones.erase(zones.begin()+6);
  }
  else if (buildings.find(Building::MARITIME_GUILD) != buildings.end())
  {
    positions.push_back(createPosition(zones[4], 0, 0, Building::MARITIME_GUILD));
    zones.erase(zones.begin()+6);
  }
  if (buildings.find(Building::SHIP_YARD) != buildings.end())
  {
    positions.push_back(createPosition(zones[4], 0, 0, Building::SHIP_YARD));
    zones.erase(zones.begin()+6);
  }
  
  vector<const Building*> vbuildings;
  for_each(buildings.begin(), buildings.end(), [&](const Building* b) { vbuildings.push_back(b); });
  sort(vbuildings.begin(), vbuildings.end(), GFX_COMPARATOR);
  
  for (auto b : vbuildings)
  {
    auto suitable = findSuitable(b);
    
    if (suitable.empty())
    {
      deploy();
      return;
    }
    
    LayoutZone zone = suitable[Util::randomIntUpTo(static_cast<u32>(suitable.size()))];
    placeAndSplit(b, zone);
  }
  
  // TODO: lower when building amount increases
  int houses = 20 + Util::randomIntUpTo(10);
  for (int i = 0; i < houses; ++i)
  {
    placeAndSplit(nullptr, zones[Util::randomIntUpTo(static_cast<u32>(zones.size()))]);
  }

  sort(positions.begin(), positions.end());
}

CityLayout::LayoutPosition CityLayout::createPosition(LayoutZone &zone, s16 ox, s16 oy, const Building *building)
{
  if (building)
    return LayoutPosition(zone.x + ox*U*2 - oy*U - (specs[building].depth-1)*U, zone.y + oy*U + (specs[building].depth-1)*U, building);
  else
    return LayoutPosition(zone.x + ox*U*2 - oy*U, zone.y + oy*U, Util::randomIntUpTo(5));
}

const vector<CityLayout::LayoutZone> CityLayout::findSuitable(const Building *building)
{
  vector<LayoutZone> zn;
  copy_if(zones.begin(), zones.end(), back_inserter(zn), [&](const LayoutZone& z) { return z.w >= specs[building].width && z.h >= specs[building].depth; });
  return zn;
}

void CityLayout::placeAndSplit(const Building *b, CityLayout::LayoutZone &z)
{
  s16 bw = b ? specs[b].width : 1;
  s16 bh = b ? specs[b].depth : 1;

  if (bw == z.w && bh == z.h)
    positions.push_back(createPosition(z,0,0,b));
  else
  {
    if (z.w > bw && z.h > bh)
    {
      positions.push_back(createPosition(z,0,0,b));
      zones.push_back(LayoutZone(z.x + bw*U*2, z.y, z.w - bw, bh, false, false));
      zones.push_back(LayoutZone(z.x - bh*U, z.y + bh*U, z.w, z.h - bh, false, false));
    }
    else if (z.w > bw)
    {
      positions.push_back(createPosition(z,0,0,b));
      zones.push_back(LayoutZone(z.x + bw*U*2, z.y, z.w - bw, z.h, false, false));
    }
    else
    {
      positions.push_back(createPosition(z,0,0,b));
      zones.push_back(LayoutZone(z.x + bw*U*2 - bh*U, z.y + bh*U, z.h, z.h - bh, false, false));
    }
  }
  
  zones.erase(find(zones.begin(), zones.end(), z)); // TODO who knows if it works
}


