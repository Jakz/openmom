//
//  Combat.cpp
//  OpenMoM
//
//  Created by Jack on 7/22/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "Combat.h"

bool operator<(const CombatUnit &c1, const CombatUnit &c2)
{
  if (c1.y < c2.y) return true;
  else if (c1.x < c2.x) return true;
  else return false;
}