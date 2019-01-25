#ifndef _SKILL_SET_H_
#define _SKILL_SET_H_

#include "common/Common.h"
#include "Skill.h"
#include "Spells.h"

#include <vector>

class Unit;

/*class SkillSetFetcher
{
  const Unit* unit();
  const Army* army();
  const combat::Combat* combat();
  const Player* player();
};*/

class SkillProvider
{
  virtual size_t size();
  virtual const Skill* at(size_t index);
};

class SkillSet
{
private:
  const Unit* unit;

  skill_list nativeSkills;
  cast_list spells;


public:
  SkillSet(const Unit* unit);
  SkillSet(skill_init_list skills) : nativeSkills(skills), unit(nullptr) { }

  size_t size() const;
  const Skill* get(size_t index) const;
  const Skill* operator[](size_t index) const { return get(index); }
  
  void add(const Skill* skill) { nativeSkills.push_back(skill); }
  void add(const SpellCast& cast) { spells.push_back(cast); }
  
  void remove(const Spell* spell);
  
  value_t spellsUpkeep() const;
  
  value_t bonusForProperty(Property property) const;
  value_t bonusForPlayerAttribute(PlayerAttribute attribute) const;
  
  cast_list::iterator spellIterator() { return spells.begin(); }
  cast_list::const_iterator spellIterator() const { return spells.begin(); }
  
  void purgeCombatSpells()
  {
    auto nend = std::remove_if(spells.begin(), spells.end(), [] (const cast_list::value_type& spell) { return spell.castInCombat; });
    spells.erase(nend, spells.end());
  }
  
  bool hasSpell(const Spell* spell) const;
  bool hasSkill(const Skill* skill) const;
  bool hasSimpleEffect(SimpleEffect::Type type) const;
    
  bool has(const std::function<bool(const SkillEffect*)>& predicate) const;
  
  bool has(MovementType type) const;
  
  void forEachEffect(std::function<void(const SkillEffect*)> lambda) const;
  
  template<typename T> void findAllEffectsOftype(T& effects, SkillEffect::Type type) const
  {
    skills::findAllEffectsOftype(*this, effects, type);
  }
  
  School glowEffect() const;
  
  template<typename Skill>
  class iterator //: public /*std::iterator<std::forward_iterator_tag, Skill>/*, std::iterator_traits<iterator<Skill>>,*/
  {
  public:
    using difference_type = ptrdiff_t;
    using value_type = Skill*;
    using reference = const Skill&;
    using pointer = const Skill*;
    using iterator_category = std::bidirectional_iterator_tag;

    iterator(const SkillSet& parent, s16 current = 0) : parent(&parent), current(current), size(parent.size()) { }
    iterator(const iterator<Skill> &other) = default;
    ~iterator() { }
    
    bool operator==(const iterator<Skill> o) const { return parent == o.parent && current == o.current; }
    bool operator!=(const iterator<Skill> o) const { return parent != o.parent || current != o.current; }
    
    iterator<Skill>& operator+=(const s16& offset) { current += offset; return (*this); }
    iterator<Skill>& operator-=(const s16& offset) { current -= offset; return (*this); }
    iterator<Skill>& operator++() { ++current; return (*this); }
    iterator<Skill>& operator--() { --current; return (*this); }
    iterator<Skill> operator++(int) { auto tmp(*this); ++current; return tmp; }
    iterator<Skill> operator--(int) { auto tmp(*this); --current; return tmp; }
    
    difference_type operator-(const iterator<Skill>& other) const { return current - other.current; }
    
//    iterator<Skill>(const s16& offset) { auto old = current; current += offset; auto tmp(*this); current = old; return tmp; }

    Skill operator*() { return parent->get(current); }
    const Skill operator*() const { return parent->get(current); }
    //const Skill* operator->() { return parent.get(current); }
    

    
    protected:
      const SkillSet* parent;
      s16 current = 0;
      s16 size;
  };
    
  using const_iterator = iterator<const Skill*>;

  const_iterator begin() const { return const_iterator(*this); }
  const_iterator end() const { return const_iterator(*this, size()); }
};



#endif
