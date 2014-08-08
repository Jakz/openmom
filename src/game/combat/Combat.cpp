//
//  Combat.cpp
//  OpenMoM
//
//  Created by Jack on 7/22/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "Combat.h"

#include "Util.h"

#include "Player.h"

#include "Animations.h"

bool operator<(const CombatUnit &c1, const CombatUnit &c2)
{
  if (c1.y < c2.y) return true;
  else if (c1.x < c2.x) return true;
  else return false;
}

Combat::Combat(Army* a1, Army* a2) : players{a1->getOwner(),a2->getOwner()}, selectedUnit(nullptr), current(a1->getOwner())
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
  
  for (int i = 0; i < W; ++i)
    for (int j = 0; j < H; ++j)
      tiles[i][j] = Util::randomIntUpTo(32);
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

CombatUnit* Combat::unitAtTile(u16 x, u16 y)
{
  auto it = std::find_if(allUnits.begin(), allUnits.end(), [&](const CombatUnit* unit) { return unit->x == x && unit->y == y; });
  return it != allUnits.end() ? *it : nullptr;
}

void Combat::attack(CombatUnit *u1, CombatUnit *u2)
{
  u1->facing = relativeFacing(u1, u2);
  u2->facing = relativeFacing(u2, u1);
  
  //TODO: probably both animations should be pushed on both players
  u1->getOwner()->push(new anims::CombatAttack(u1));
  u2->getOwner()->push(new anims::CombatAttack(u2));
}

s16 Combat::relativeFacing(CombatUnit *u1, CombatUnit *u2)
{
  for (int i = 0; i < 8; ++i)
    if (u1->x+dirs(i,u1->y%2 == 0)[0] == u2->x && u1->y+dirs(i,u1->y%2 == 0)[1] == u2->y)
      return i;
  
  return -1;
}

void Combat::moveUnit(CombatUnit *unit, u16 x, u16 y)
{
  if (unit && unit != selectedUnit)
    select(unit);
  
  CombatPosition pos = CombatPosition(unit);
}