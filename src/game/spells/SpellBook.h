#ifndef _SPELLBOOK_H_
#define _SPELLBOOK_H_

#include "common/Common.h"
#include "Data.h"
#include "Spells.h"

#include <map>
#include <array>
#include <numeric>

class Player;

struct SpellComparatorByResearch
{
  bool operator() (const Spell *s1, const Spell* s2) const
  {
    value_t delta = s1->mana.researchCost - s2->mana.researchCost;
    
    if (delta != 0) return delta < 0;
    else return s1->school < s2->school;
  }
};

struct SpellComparatorByManaCost
{
  bool operator() (const Spell *s1, const Spell* s2) const
  {
    value_t delta = s1->mana.manaCost - s2->mana.manaCost;
    
    if (delta != 0) return delta < 0;
    else return s1->school < s2->school;
  }
};

struct SpellComparatorByManaCostCombat
{
  bool operator() (const Spell *s1, const Spell* s2) const
  {
    value_t delta = s1->mana.combatManaCost - s2->mana.combatManaCost;
    
    if (delta != 0) return delta < 0;
    else return s1->school < s2->school;
  }
};

using spell_map =  std::map<const Spell*, bool, SpellComparatorByResearch>;

class SpellBook
{
private:
  spell_map spells;
  Player& player;
  
  const Spell* currentCast;
  const Spell* currentResearch;
  value_t manaToCast;
  value_t manaToResearch;
  
  enum_simple_map<School, s8, 6> books;
  //std::array<s8, 6> books;
  
public:
  enum Type
  {
    OVERLAND,
    COMBAT,
    RESEARCH
  };
  
  SpellBook(Player& player) : player(player), currentCast(nullptr), currentResearch(nullptr), manaToCast(0), manaToResearch(0), books(0, { {School::CHAOS, 1}, { School::LIFE, 3}, { School::NATURE, 3} })
  {
    
  }
  
  value_t totalBooks() const {return std::accumulate(books.begin(), books.end(), 0); }
  
  School predominantSchool() const
  {
    School predominant = School::SCHOOL_FIRST;
    value_t count = 0;

    for (School school : Data::schoolsWithoutArcane())
    {
      if (books[school] > count)
      {
        predominant = school;
        count = books[school];
      }
    }

    //TODO: if two school have same amount of books which one is chosen?
   
    return predominant;
  }
  
  value_t booksForSchool(School school) const { return books[school]; }
  
  value_t baseCastingSkill() { return 100; /* TODO: forced */ }
  
  void startCast(const Spell* spell);
  
  void discoverSpell(const Spell* spell, bool discovered = true)
  {
    spells.emplace(spell, discovered);
  }
  
  bool spendManaForCast(s32 mana)
  {
    bool finished = mana >= manaToCast;
    value_t valueSpent = std::min(mana, manaToCast);
    manaToCast -= valueSpent;
    LOGG("spell-cast", "spending %d mana, %d mana still needed", valueSpent, manaToCast);
    return finished;
  }
  
  void startResearch(const Spell* spell)
  {
    currentResearch = spell;
    manaToResearch = currentResearch->mana.researchCost;
  }
  
  void combatCast(const Spell* spell)
  {
    currentCast = spell;
  }
  
  void cancelCast()
  {
    currentCast = nullptr;
    manaToCast = 0;
  }
  
  const Spell* getCurrentCast() const { return currentCast; }
  const Spell* getCurrentResearch() const { return currentResearch; }
  
  value_t getManaToCast() const { return manaToCast; }
  
  value_t turnsToCompleteResearch(const Spell* spell) const;
  value_t turnsToCompleteResearch() const ;
  
  const Spell* advanceResearch();
  
  size_t size() { return spells.size(); }
  
  void fillPool();
  
  const std::vector<ResearchStatus> availableForResearch() const;
  const std::vector<ResearchStatus> discoveredSpells(Type type) const;

};

#endif
