//
//  Combat.cpp
//  OpenMoM
//
//  Created by Jack on 7/22/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "Combat.h"

#include "Util.h"

#include "CombatMap.h"
#include "Player.h"

#include "Animations.h"

using namespace std;

Combat::Combat(Army* a1, Army* a2, CombatMechanics* mechanics) : players{a1->getOwner(),a2->getOwner()}, selectedUnit(nullptr), current(a1->getOwner()), mechanics(mechanics), _map(new CombatMap(W,H))
{
  for (auto u1 : a1->getUnits())
  {
    CombatUnit* cu1 = new CombatUnit(u1);
    units[0].push_back(cu1);
    allUnits.push_back(cu1);
  }
  
  for (auto u2 : a2->getUnits())
  {
    CombatUnit* cu2 = new CombatUnit(u2);
    units[1].push_back(cu2);
    allUnits.push_back(cu2);
  }
  
  deployUnits();
  
  current->combatTurnBegun();
}

Combat::~Combat()
{
  
}

void Combat::endTurn()
{
  deselect();
  current->combatTurnEnded();
  current = current == players[0] ? players[1] : players[0];
  current->combatTurnBegun();
  
  //TODO: check if (current instanceof LocalPlayer)
  //  current.game.nextLocal();
}

const CombatTile* Combat::tileAt(u16 x, u16 y) { return _map->tileAt(x,y); }

CombatUnit* Combat::unitAtTile(u16 x, u16 y)
{
  auto it = std::find_if(allUnits.begin(), allUnits.end(), [&](const CombatUnit* unit) { return unit->x() == x && unit->y() == y; });
  return it != allUnits.end() ? *it : nullptr;
}

void Combat::attack(CombatUnit *u1, CombatUnit *u2)
{
  u1->setFacing(relativeFacing(u1, u2));
  u2->setFacing(relativeFacing(u2, u1));
  
  //TODO: probably both animations should be pushed on both players
  u1->getOwner()->push(new anims::CombatAttack(u1));
  u2->getOwner()->push(new anims::CombatAttack(u2));
}

Dir Combat::relativeFacing(CombatUnit *u1, CombatUnit *u2)
{
  for (int i = 0; i < 8; ++i)
    if (u1->x()+CombatCoord::dirs(i,u1->y()%2 == 0)[0] == u2->x() && u1->y()+CombatCoord::dirs(i,u1->y()%2 == 0)[1] == u2->y())
      return static_cast<Dir>(i);
  
  return Dir::NORTH;
}

void Combat::deployUnits()
{
  // TODO
  CombatCoord tile = CombatCoord(2, 3);
  
  for (auto* unit : allUnits)
  {
    unit->setPosition(tile);
    tile = tile.neighbour(Dir::SOUTH_EAST);
  }
}

const position_map& Combat::reachable(CombatUnit *unit)
{
  /*visited.clear();
  currents.clear();
  incoming.clear();
  
  s16 moves = unit->moves;
  
  incoming.emplace(CombatPosition(unit), CombatPosition());
  
  while (moves + 1 > 0)
  {
    currents.insert(incoming.begin(), incoming.end());
    incoming.clear();
    
    for (const auto& e : currents)
    {
      visited.insert(e);
      
      for (int j = 0; j < 8; ++j)
      {
        int i = j < 4 ? 1 + j*2 : (j-4)*2;
        int ny = e.first.y;
        int nx = e.first.x;
        nx += dirs(i,ny%2 == 0)[0];
        ny += dirs(i,ny%2 == 0)[1];
      
        if (nx >= 0 && nx < W && ny >= 0 && ny < H)
        {
          if (unitAtTile(nx,ny) && unitAtTile(nx,ny)->getOwner() == unit->getOwner())
            continue;
          
          auto lambda = [&](const pair<CombatPosition,CombatPosition> &p) { return p.first.x == nx && p.first.y == ny; };
          
          if (find_if(visited.begin(), visited.end(), lambda) == visited.end() && find_if(currents.begin(), currents.end(), lambda) == currents.end())
          {
            CombatPosition np = CombatPosition(e.first);
            np.f = i;
            incoming.emplace(CombatPosition(nx,ny), np);
          }
          //else if (v != null && ((v.intValue() & 0xFF000000) >> 24) % 2 == 0)
          // incoming.put(buildPosition(nx,ny), (i << 24) | c.getKey());
          // else if (o != null && ((o.intValue() & 0xFF000000) >> 24) % 2 == 0)
          // incoming.put(buildPosition(nx,ny), (i << 24) | c.getKey());
        }
        
        
      }
      
      currents.clear();
      --moves;
    }
  }
  */
  return visited;
}

void Combat::moveUnit(CombatUnit *unit, u16 x, u16 y)
{
  if (unit && unit != selectedUnit)
    select(unit);
  
  CombatPosition pos = CombatPosition(unit->x(), unit->y());
  
  //           auto lambda = [&](const pair<CombatPosition,CombatPosition> &p) { return p.first.x == nx && p.first.y == ny; };

}
