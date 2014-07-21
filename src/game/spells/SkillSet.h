#ifndef _SKILL_SET_H_
#define _SKILL_SET_H_

#include "Common.h"
#include "Skill.h"
#include "Spells.h"

#include <vector>

class Unit;

class SkillSet
{
  private:
    const Unit& unit;
  
    const skill_list& nativeSkills;
    skill_list additionalSkills;
    cast_list spells;
  
  
  public:
    SkillSet(Unit& unit);
  
    size_t size() const { return nativeSkills.size() + additionalSkills.size() + spells.size(); }
    const Skill& get(int index);
};

#endif
