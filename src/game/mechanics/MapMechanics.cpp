//
//  MapMechanics.cpp
//  OpenMoM
//
//  Created by Jack on 7/22/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "MapMechanics.h"

#include "common/Util.h"
#include "Game.h"
#include "Places.h"
#include "Skill.h"
#include "World.h"
#include "Unit.h"

using namespace std;

static float desertChances[][3] = {{66.7f, 33.3f, 0.0f}, {20.0f, 60.0f, 20.0f}};
static Resource desertResource[] = {Resource::GEMS, Resource::QOURK_CRYSTAL, Resource::CRYSX_CRYSTAL};

static float hillChances[][6] = {{33.3f,16.7f,22.2f,22.2f,5.6f,0.0f},{10,10,10,40,20,10}};
static float mountainChances[][6] = {{22.2f,27.7f,16.7f,16.7f,16.7f,0.0f},{10,10,10,20,30,20}};
static Resource hillsResources[] = {Resource::IRON_ORE,Resource::COAL,Resource::SILVER,Resource::GOLD,Resource::MITHRIL,Resource::ADAMANTIUM};


float MapMechanics::chanceResourceForTile(TileType type, Plane which) const
{
  if (which == ARCANUS)
    return 1/17.0f;
  else if (which == MYRRAN)
    return 1/10.0f;
  
  else return 0.0f;
}

Resource MapMechanics::generateResourceForTile(TileType type, Plane which) const
{
  if (Util::chance(chanceResourceForTile(type,which)))
  {
    switch (type)
    {
      case TILE_FOREST: return Resource::WILD_GAME;
      case TILE_SWAMP: return Resource::NIGHT_SHADE;
      case TILE_DESERT:
      {
        float x = Util::rand(100.0f);
        for (int i = 0; i < sizeof(desertResource)/sizeof(desertResource[0]); ++i)
          if (x > desertChances[which][i])
            x -= desertChances[which][i];
          else return desertResource[i];
        break;
      }
      case TILE_MOUNTAIN:
      {
        float x = Util::rand(100.0f);
        for (int i = 0; i < sizeof(hillsResources)/sizeof(hillsResources[0]); ++i)
        {
          if (x > mountainChances[which][i])
            x -= mountainChances[which][i];
          else return hillsResources[i];
        }
        break;
      }
      case TILE_HILL:
      {
        float x = Util::rand(100.0f);
        for (int i = 0; i < sizeof(hillsResources)/sizeof(hillsResources[0]); ++i)
          if (x > hillChances[which][i])
            x -= hillChances[which][i];
          else return hillsResources[i];
        break;
      }
        
      default: break;
    }
  }
  
  return Resource::NONE;
}

ManaNode* MapMechanics::generateManaNode(World* world, const Position position, const School school) const
{
  float mana = Util::randomIntUpTo(position.plane == ARCANUS ? 10 : 20);
  
  if (g->settings.group(SETTING_MAGIC_POWER).value() == MAGIC_POWER_POWERFUL)
    mana += mana*0.5;
  if (g->settings.group(SETTING_MAGIC_POWER).value() == MAGIC_POWER_WEAK)
    mana /= 2;
  
  /* TODO: handle aura */
  
  return new ManaNode(school, (int)std::floor(mana));
}

u16 MapMechanics::turnsRequiredToBuildRoadOnTile(Tile* tile) const
{
  if (tile->node)
  {
    switch (tile->node->school)
    {
      case CHAOS: return 5;
      case NATURE: return 4;
      case SORCERY: return 3;
      default: return 0;
    }
  }
  else
  {
    switch (tile->type)
    {
      case TILE_DESERT: return 4;
      case TILE_FOREST: return 6;
      case TILE_GRASS: return 3;
      case TILE_HILL: return 6;
      case TILE_MOUNTAIN: return 8;
      case TILE_SWAMP: return 8;
      case TILE_TUNDRA: return 6;
      case TILE_VOLCANO: return 6;
      case TILE_RIVER: return 5;
      //TODO: river mouth
    }
  }
  
  return 0;
}

s16 MapMechanics::movementCost(World* world, const Position& position, const movement_list& movement) const
{
  Tile* t = world->get(position);
  
  bool canMakeUseOfRoads = !movement.contains(MovementType::NON_CORPOREAL) && !movement.contains(MovementType::FLYING);
  
  if (canMakeUseOfRoads && t->hasEnchantedRoad)
    return 0;
  else if (canMakeUseOfRoads && t->hasRoad)
    return 1;
  else if (movement.contains(MovementType::NON_CORPOREAL))
    return 1;
  else if (movement.contains(MovementType::FLYING))
    return 2;
  else
  {
    switch (t->type) {
      case TILE_GRASS:
        return movement.contains(MovementType::MOUNTAINWALK) ? 6 : 2;
      case TILE_DESERT:
      case TILE_WATER:
      case TILE_SHORE:
        return 1;
      case TILE_FOREST:
        return movement.contains(MovementType::FORESTWALK) ? 2 : 4;
      case TILE_RIVER:
      case TILE_RIVER_MOUTH:
      case TILE_TUNDRA:
        return movement.contains(MovementType::SWIMMING) ? 2 : 4;
      case TILE_SWAMP:
        return movement.contains(MovementType::SWIMMING) ? 2 : 6;
      case TILE_HILL:
        return movement.contains(MovementType::MOUNTAINWALK) ? 2 : 6;
      case TILE_MOUNTAIN:
      case TILE_VOLCANO:
        return movement.contains(MovementType::MOUNTAINWALK) ? 2 : 8;
      case TILE_TYPES:
        assert(false);
        return 0;
    }
  }
}

#define ANY_OF(x) any_of(u.begin(), u.end(), [] (const movement_list& l) { return l.find(Effects::x) != l.end(); })
#define NONE_OF(x) any_of(u.begin(), u.end(), [] (const movement_list& l) { return l.find(Effects::x) != l.end(); })
#define ALL_OF(x) any_of(u.begin(), u.end(), [] (const movement_list& l) { return l.find(Effects::x) != l.end(); })


//TODO: check behavior
const movement_list MapMechanics::movementTypeForSetOfEffects(const movement_list_group& u) const
{  
  using Type = MovementType;
  
  movement_list movements;
  
  /* if all units has either SWIMMING or FLYING and no unit has SAILING or WIND_WALKING */
  if (u.all_of([](const movement_list& l) { return l.contains(Type::SWIMMING) || l.contains(Type::FLYING); }) &&
      u.none_of([](const movement_list& l) { return l.contains(Type::SAILING) || l.contains(Type::WINDWALK); }))
    movements.add(Type::SWIMMING);
  
  if (u.any_of(Type::WINDWALK) || u.all_of(Type::FLYING))
    movements.add(Type::FLYING);
  
  if (u.any_of(Type::SAILING))
    movements.add(Type::SAILING);
  
  if (u.any_of(Type::FORESTWALK) &&
      u.none_of([](const movement_list& l){ return l.contains(Type::WINDWALK) || l.contains(Type::SAILING) || l.contains(Type::MOUNTAINWALK); }))
    movements.add(Type::FORESTWALK);
  
  if (u.any_of(Type::MOUNTAINWALK) &&
      u.none_of([](const movement_list& l){ return l.contains(Type::WINDWALK) || l.contains(Type::SAILING) || l.contains(Type::FORESTWALK); }))
    movements.add(Type::MOUNTAINWALK);
  
  if (!movements.contains(Type::FLYING) && !movements.contains(Type::SAILING))
  {
    bool allNonCorporeal = u.all_of(Type::NON_CORPOREAL);
    bool anyPathFinder = u.any_of(Type::PATH_FINDER);
    bool anyMountainerAndForestwalker = u.any_of(Type::MOUNTAINWALK) && u.any_of(Type::FORESTWALK);
    
    if (allNonCorporeal || anyPathFinder || anyMountainerAndForestwalker)
      movements.add(Type::PATH_FINDER);
  }
  
  if (u.all_of(Type::PLANAR_TRAVEL))
    movements.add(Type::PLANAR_TRAVEL);
  
  if (u.any_of(Type::WINDWALK))
    movements.add(Type::WINDWALK);
  
  return movements;
}

const movement_list MapMechanics::movementTypeOfArmy(const unit_list& units) const
{    
  movement_list_group umovements(units.size());
  
  std::transform(units.begin(), units.end(), std::back_inserter(umovements), [] (const Unit* unit) {
    std::vector<const MovementEffect*> effects;
    unit->skills()->findAllEffectsOftype(effects, SkillEffect::Type::MOVEMENT);
    return movement_list(effects);
  });
  
  return movementTypeForSetOfEffects(umovements);
}



