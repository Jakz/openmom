#ifndef _COMBAT_H_
#define _COMBAT_H_

#include "Common.h"
#include "Unit.h"

class HitPoints;

namespace combat
{
  class Damage
  {
  public:
    const enum class Type {
      SINGLE,
      EACH_SAME,
      EACH_DIFFERENT,
      EACH_FATAL
    } type;
    Damage(Type type) : type(type) { }
    virtual void apply() = 0;
  };
  
  class DamageSingle : public Damage
  {
  public:
    DamageSingle(s16 amount) : Damage(Type::SINGLE), amount(amount) { }
    const s16 amount;
  };
  
  class DamageEachSame : public Damage
  {
  public:
    DamageEachSame(s16 amount) : Damage(Type::EACH_SAME), amount(amount) { }
    const s16 amount;
  };
  
  class DamageEachDifferent : public Damage
  {
  public:
    DamageEachDifferent(hit_points&& amounts) : Damage(Type::EACH_SAME), amounts(amounts) { }
    const hit_points amounts;
  };
  
  class DamageEachFatal : public Damage
  {
  public:
    DamageEachFatal(std::vector<bool>&& amounts) : Damage(Type::EACH_FATAL), amounts(amounts) { }
    const std::vector<bool> amounts;
  };
}

#endif
