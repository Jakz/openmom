#include "SkillSet.h"

#include "Unit.h"


SkillSet::SkillSet(Unit& unit) : unit(unit), nativeSkills(unit.spec.skills) { }

const Skill& SkillSet::get(int index)
{
  /*size_t nc = nativeSkills.size(), ac = additionalSkills.size(),
  
  if (index < nativeSkills.size())
    return static_cast<const Skill&>(*nativeSkills[index]);
  else if (index < nativeSkills.size() + additionalSkills.size())
    return additionalSkills[index - nai]*/
  
  return Skills::LARGE_SHIELD;
}