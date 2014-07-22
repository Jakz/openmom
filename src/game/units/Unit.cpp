#include "Unit.h"

#include "Combat.h"
#include "Localization.h"

#include <numeric>

using namespace std;

float HitPoints::percentHealth() const
{
  return accumulate(data.begin(), data.end(), 0);
}

void HitPoints::healAll()
{
  data.assign(unit.getProperty(Property::FIGURES), unit.getProperty(Property::HIT_POINTS));
}

void HitPoints::applyDamage(const combat::Damage& damage)
{
  switch (damage.type) {
    case combat::Damage::Type::SINGLE: applyDamage(static_cast<const combat::DamageSingle&>(damage).amount); break;
    case combat::Damage::Type::EACH_SAME: applySameDamageToEachFigure(static_cast<const combat::DamageEachSame&>(damage).amount); break;
    case combat::Damage::Type::EACH_DIFFERENT: applyDifferentDamageToEachFigure(static_cast<const combat::DamageEachDifferent&>(damage).amounts); break;
    case combat::Damage::Type::EACH_FATAL: killFigures(static_cast<const combat::DamageEachFatal&>(damage).amounts); break;
  }
}

void HitPoints::applyDamage(s16 dmg)
{
  while (dmg > 0 && isAlive())
  {
    s16 v = std::min(dmg, data[0]);
    
    data[0] -= v;
    dmg -= v;
    
    if (data[0] <= 0)
      data.erase(data.begin());
    
    if (aliveCount() == 0)
      return;
  }
}

void HitPoints::applySameDamageToEachFigure(s16 dmg)
{
  for_each(data.begin(), data.end(), [&](s16& hp){ hp -= dmg; });
  remove_if(data.begin(), data.end(), [](s16& hp) { return hp <= 0; });
}

void HitPoints::applyDifferentDamageToEachFigure(const hit_points& dmgs)
{
  for (int i = 0; i < dmgs.size(); ++i)
    data[i] -= dmgs[i];
  
  remove_if(data.begin(), data.end(), [](s16& hp) { return hp <= 0; });
}

void HitPoints::killFigures(const std::vector<bool>& indices)
{
  s16 current = 0;
  remove_if(data.begin(), data.end(), [&](s16& hp) { return indices[current++]; });
}




const std::string Hero::title() const
{
  return "";
}