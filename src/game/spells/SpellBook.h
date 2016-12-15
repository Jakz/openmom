#ifndef _SPELLBOOK_H_
#define _SPELLBOOK_H_

#include "common/Common.h"
#include "Spells.h"

#include <map>
#include <array>
#include <numeric>

class Player;

struct SpellComparatorByResearch : public std::binary_function<const Spell*, const Spell*, bool>
{
  bool operator() (const Spell *s1, const Spell* s2) const
  {
    s32 delta = s1->mana.researchCost - s2->mana.researchCost;
    
    if (delta != 0) return delta < 0;
    else return s1->school < s2->school;
  }
};

struct SpellComparatorByManaCost : public std::binary_function<const Spell*, const Spell*, bool>
{
  bool operator() (const Spell *s1, const Spell* s2) const
  {
    s32 delta = s1->mana.manaCost - s2->mana.manaCost;
    
    if (delta != 0) return delta < 0;
    else return s1->school < s2->school;
  }
};

struct SpellComparatorByManaCostCombat : public std::binary_function<const Spell*, const Spell*, bool>
{
  bool operator() (const Spell *s1, const Spell* s2) const
  {
    s32 delta = s1->mana.combatManaCost - s2->mana.combatManaCost;
    
    if (delta != 0) return delta < 0;
    else return s1->school < s2->school;
  }
};

typedef std::map<const Spell*, bool, SpellComparatorByResearch> spell_map;

class SpellBook
{
private:
  static u16 guaranteedSpellAmountForRarity(SpellRarity rarity, u16 books);
  static u16 researchableSpellAmountForRarity(SpellRarity rarity, School school, u16 books);
  
  spell_map spells;
  Player& player;
  
  const Spell* currentCast;
  const Spell* currentResearch;
  s32 manaToCast;
  s32 manaToResearch;
  
  std::array<s8, 6> books;
  
public:
  enum Type
  {
    OVERLAND,
    COMBAT,
    RESEARCH
  };
  
  SpellBook(Player& player) : player(player), currentCast(nullptr), currentResearch(nullptr), manaToCast(0), manaToResearch(0)
  {
    books = {0,1,0,3,3,0}; // TODO: hardcoded
  
  }
  
  s16 totalBooks() const {return std::accumulate(books.begin(), books.end(), 0); }
  
  s16 booksForSchool(School school) const { return books[school]; }
  
  s16 baseCastingSkill() { return 100; /* TODO: forced */ }
  
  void startCast(const Spell* spell);
  
  void discoverSpell(const Spell* spell, bool discovered = true)
  {
    spells.emplace(spell, discovered);
  }
  
  bool spendManaForCast(s32 mana)
  {
    bool finished = mana >= manaToCast;
    manaToCast -= std::min(mana, manaToCast);
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
  
  s32 getManaToCast() const { return manaToCast; }
  
  s16 turnsToCompleteResearch(const Spell* spell) const;
  s16 turnsToCompleteResearch() const ;
  
  const Spell* advanceResearch();
  
  s16 size() { return spells.size(); }
  
  void fillPool();
  
  const std::vector<ResearchStatus> availableForResearch() const;
  const std::vector<ResearchStatus> bookSpells(Type type) const;

};

#endif
