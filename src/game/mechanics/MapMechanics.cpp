//
//  MapMechanics.cpp
//  OpenMoM
//
//  Created by Jack on 7/22/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "MapMechanics.h"

#include "Util.h"
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

ManaNode* MapMechanics::generateManaNode(World* world, const Position& position, const School school) const
{
  float mana = Util::randomIntUpTo(position.plane == ARCANUS ? 10 : 20);
  
  if (g.settings.group(SETTING_MAGIC_POWER).value() == MAGIC_POWER_POWERFUL)
    mana += mana*0.5;
  if (g.settings.group(SETTING_MAGIC_POWER).value() == MAGIC_POWER_WEAK)
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


s16 MapMechanics::baseMovementCost(TileType type) const
{
  switch (type) {
    case TILE_GRASS:
    case TILE_WATER:
    case TILE_SHORE:
      return 1;
    case TILE_DESERT:
    case TILE_FOREST:
    case TILE_HILL:
    case TILE_RIVER:
    case TILE_TUNDRA:
    case TILE_RIVER_MOUTH:
      return 2;
    case TILE_MOUNTAIN:
    case TILE_SWAMP:
    case TILE_VOLCANO:
      return 3;
			//default:
			//	return 0;
			// TODO: river mouth / shore?
  }
}

s16 MapMechanics::specificMovementCost(World* world, const Position& position, const Army* army) const
{
  Tile* t = world->get(position);
  if (army->hasMovement(Effects::NON_CORPOREAL) && t->hasRoad)
    return baseMovementCost(t->type);
  else if (t->hasRoad && t->hasEnchantedRoad)
    return 0;
  else if (t->hasRoad)
    return 1;
  
  return baseMovementCost(t->type);
}

//TODO: check behavior
const movement_list MapMechanics::movementTypeOfArmy(const unit_list& units) const
{  
  movement_list movements;
  
  if (units.size() == 1)
  {
    Unit* unit = units.front();
    for (const MovementEffect* const effect : Effects::MOVEMENT_EFFECTS)
    {
      if (unit->skills.hasSkillEffect(effect))
        movements.insert(effect);
    }
    
    return movements;
  }
  else
  {
    movement_list toRemoveMovements, finalMovements;

    for (const MovementEffect* const effect : Effects::MOVEMENT_EFFECTS)
    {
      for (const Unit* unit : units)
      {
        bool hasMovement = unit->skills.hasSkillEffect(effect);
        
        if (hasMovement && effect->shared)
          movements.insert(effect);
        else if (!hasMovement && !effect->shared)
          toRemoveMovements.insert(effect);
      }
    }
    
    for (auto effect : movements)
      if (toRemoveMovements.find(effect) == toRemoveMovements.end())
        finalMovements.insert(effect);
    
    return finalMovements;
  }
}



