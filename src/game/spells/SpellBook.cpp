#include "SpellBook.h"

#include "Player.h"
#include "SpellMechanics.h"
#include "Game.h"

#include <random>

using namespace std;

s16 SpellBook::turnsToCompleteResearch(const Spell* spell) const
{
  s16 turns = spell->mana.researchCost / player.researchPoints();
  return turns + (spell->mana.researchCost % player.researchPoints() != 0 ? 1 : 0);
}

s16 SpellBook::turnsToCompleteResearch() const
{
  s16 turns = manaToResearch / player.researchPoints();
  return turns + (manaToResearch % player.researchPoints() != 0 ? 1 : 0);
}

const Spell* SpellBook::advanceResearch()
{
  if (currentResearch)
  {
    manaToResearch -= player.researchPoints();
    // TODO: exceeding research point are invested or not?
    if (manaToResearch < 0)
    {
      const Spell* discovered = currentResearch;
      discoverSpell(discovered);
      currentResearch = nullptr;
      manaToResearch = 0;
      return discovered;
    }
  }
  
  return nullptr;
}

void SpellBook::startCast(const Spell* spell)
{
  currentCast = spell;
  manaToCast = player.game()->spellMechanics.actualManaCost(&player, spell, false);
}

void SpellBook::fillPool()
{
  auto& mechanics = player.game()->spellMechanics;
  
  s8 currentStatus[SCHOOL_COUNT][(size_t)SpellRarity::COUNT] = {};
  s8 requiredStatus[SCHOOL_COUNT][(size_t)SpellRarity::COUNT] = {};
  
  for (auto s : spells)
    ++currentStatus[s.first->school][(size_t)s.first->rarity];
  
  for (int i = 0; i < SCHOOL_COUNT; ++i)
    for (int j = 0; j < (size_t)SpellRarity::COUNT; ++j)
    {
      requiredStatus[i][j] = mechanics.researchableSpellAmountForRarity(static_cast<SpellRarity>(j), static_cast<School>(i), booksForSchool(static_cast<School>(i)));
      requiredStatus[i][j] -= currentStatus[i][j];
    }
  
  for (int i = 0; i < SCHOOL_COUNT; ++i)
    for (int j = 0; j < (size_t)SpellRarity::COUNT; ++j)
    {
      if (requiredStatus[i][j] > 0)
      {
        spell_list totspells = Spells::spellsByRarityAndSchool(static_cast<SpellRarity>(j), static_cast<School>(i));
        
        auto nend = remove_if(totspells.begin(), totspells.end(), [this](const Spell* spell){ return spells.find(spell) != spells.end(); });
        totspells.erase(nend, totspells.end());
        
        random_device rd;
        mt19937 gen(rd());
        shuffle(totspells.begin(), totspells.end(), gen);
        
        auto it = totspells.begin();
        int k = 0;
        while (k < requiredStatus[i][j] && it != totspells.end())
        {
          spells.emplace(*it, false);
          ++it; ++k;
        }
      }
    }
}

const vector<ResearchStatus> SpellBook::availableForResearch() const
{
  vector<ResearchStatus> available;
  
  auto it = spells.begin();
  while (available.size() != 8 && it != spells.end())
  {
    if (!it->second)
      available.push_back(ResearchStatus(it->first, true)); // TODO: why true? shouldn't it be false?
    ++it;
  }
  
  return available;
}

const vector<ResearchStatus> SpellBook::discoveredSpells(Type type) const
{
  vector<ResearchStatus> rspells;
  
  for (auto it : spells)
  {
    switch (type) {
      case RESEARCH:
      {
        rspells.push_back(ResearchStatus(it.first, it.second));
        SpellComparatorByResearch c = SpellComparatorByResearch();
        sort(rspells.begin(), rspells.end(), [&](const ResearchStatus &s1, const ResearchStatus& s2) { return c(s1.spell, s2.spell); });
        break;
      }
      case OVERLAND:
      {
        if (it.second && it.first->canBeCastInOverland())
          rspells.push_back(ResearchStatus(it.first, it.second));
        SpellComparatorByManaCost c = SpellComparatorByManaCost();
        sort(rspells.begin(), rspells.end(), [&](const ResearchStatus &s1, const ResearchStatus& s2) { return c(s1.spell, s2.spell); });
        break;
      }
      case COMBAT:
      {
        if (it.second && it.first->canBeCastInCombat())
          rspells.push_back(ResearchStatus(it.first, it.second));
        SpellComparatorByManaCostCombat c = SpellComparatorByManaCostCombat();
        sort(rspells.begin(), rspells.end(), [&](const ResearchStatus &s1, const ResearchStatus& s2) { return c(s1.spell, s2.spell); });
        break;
      }
    }
  }
  
  return rspells;
}





