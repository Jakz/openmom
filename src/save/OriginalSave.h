#pragma once

#include "Common.h"

namespace osave
{
  template<typename T>
  struct bit_mask
  {
    using utype = typename std::underlying_type<T>::type;
    utype value;
    
    bool isSet(T flag) const { return value & static_cast<utype>(flag); }
    void set(T flag) { value |= static_cast<utype>(flag); }
    void reset(T flag) { value &= ~static_cast<utype>(flag); }
  };
  
  
  enum class HeroAbility : u32
  {
    LEADER         = 0x00000001,
    LEADER_S       = 0x00000002,
    LEGENDARY      = 0x00000008,
    LEGENDARY_S    = 0x00000010,
    BLADEMASTER    = 0x00000040,
    BLADEMASTER_S  = 0x00000080,
    ARMSMASTER     = 0x00000200,
    ARMSMASTER_S   = 0x00000400,
    CONSTITUTION   = 0x00001000,
    CONSTITUTION_S = 0x00002000,
    MIGHT          = 0x00008000,
    MIGHT_S        = 0x00010000,
    ARCANE_POWER   = 0x00040000,
    ARCANE_POWER_S = 0x00080000,
    SAGE           = 0x00200000,
    SAGE_S         = 0x00400000,
    PRAYERMASTER   = 0x01000000,
    PRAYERMASTER_S = 0x02000000,
    AGILITY_S      = 0x04000000,
    LUCKY          = 0x08000000,
    CHARMED        = 0x10000000,
    NOBLE          = 0x20000000,
    FEMALE         = 0x40000000,
    AGILITY        = 0x80000000
  };
  
  
  struct HeroStats
  {
    s16 level;
    bit_mask<HeroAbility> abilities;
    u8 castingSkill;
    u8 spells[4];
    u8 unused;
  } __attribute__((__packed__));
  
  
  enum class LandSize : u16
  {
    SMALL = 0,
    MEDIUM = 1,
    LARGE = 2
  };
  
  enum class MagicPower : u16
  {
    WEAK = 0,
    NORMAL = 1,
    POWERFUL = 2
  };
  
  enum class Difficulty : u16
  {
    TUTORIAL = 0,
    EASY = 1,
    NORMAL = 2,
    HARD = 3,
    IMPOSSIBLE = 4
  };
  
  struct OverallGameData
  {
    u16 wizardsCount;
    LandSize landSize;
    MagicPower magicPower;
    Difficulty difficulty;
    u16 cityCount;
    s16 unitCount;
    s16 turn;
    s16 currentActiveUnit;
    /* TODO: things missing */
  };
  
  struct Wizard
  {
    u8 portrait;
    char name[20];
  };

}
