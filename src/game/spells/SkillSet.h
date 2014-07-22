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

  size_t size() const;
  const Skill& get(int index) const;
  
  void add(const Skill& skill) { additionalSkills.push_back(&skill); }
  void add(const SpellCast& cast) { spells.push_back(cast); }
  
  void remove(const Spell* spell);
  
  s16 spellsUpkeep();
  
  s16 bonusForProperty(Property property);
  
  cast_list::iterator spellIterator() { return spells.begin(); }
  cast_list::const_iterator spellIterator() const { return spells.begin(); }
  
  bool hasSpellSkill(SkillBase base) const;
  bool hasSkill(SkillBase base) const;
  bool hasSkillEffect(const SkillEffect* effect) const;
  
  template<typename Skill>
  class myiterator //: public /*std::iterator<std::forward_iterator_tag, Skill>/*, std::iterator_traits<iterator<Skill>>,*/
  {
  public:

    myiterator(const SkillSet& parent, s16 current = 0) : parent(parent), current(current) { }
    myiterator(const myiterator<Skill> &other) = default;
    ~myiterator() { }
    
    bool operator==(const myiterator<Skill> o) const { return &parent == &o.parent && current == o.current; }
    bool operator!=(const myiterator<Skill> o) const { return &parent != &o.parent || current != o.current; }
    
    myiterator<Skill>& operator+=(const s16& offset) { current += offset; return (*this); }
    myiterator<Skill>& operator-=(const s16& offset) { current -= offset; return (*this); }
    myiterator<Skill>& operator++() { ++current; return (*this); }
    myiterator<Skill>& operator--() { --current; return (*this); }
    myiterator<Skill> operator++(int) { auto tmp(*this); ++current; return tmp; }
    myiterator<Skill> operator--(int) { auto tmp(*this); --current; return tmp; }
    
    myiterator<Skill>(const s16& offset) { auto old = current; current += offset; auto tmp(*this); current = old; return tmp; }

    Skill& operator*() { return parent.get(current); }
    const Skill& operator*() const { return parent.get(current); }
    //const Skill* operator->() { return parent.get(current); }
    
    typedef s16 difference_type; //almost always ptrdif_t
    typedef Skill value_type; //almost always T
    typedef const Skill& reference; //almost always T& or const T&
    //typedef const Skill* pointer; //almost always T* or const T*
    typedef std::forward_iterator_tag iterator_category;  //usually std::forward_iterator_tag or similar
    
    protected:
      const SkillSet& parent;
      s16 current = 0;
  };
    
  typedef myiterator<const Skill&> const_iterator;

  const_iterator begin() const { return const_iterator(*this); }
  const_iterator end() const { return const_iterator(*this, size()); }
};



#endif
