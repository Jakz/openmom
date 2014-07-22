#ifndef _ITEMS_H_
#define _ITEMS_H_

#include "Common.h"

enum class ItemSlots : u8
{
  RANGED,
  BATTLE_MAGE,
  MAGE,
  WARRIOR
};

class Item
{
  
  public:
    const static u32 MAX_SLOTS = 3;
};

#endif
