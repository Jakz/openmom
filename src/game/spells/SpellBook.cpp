#include "SpellBook.h"

#include "Player.h"
#include "SpellMechanics.h"
#include "game/Game.h"

#include <random>

using namespace std;

u16 SpellBook::guaranteedSpellAmountForRarity(SpellRarity rarity, u16 books)
{
  // no benefits from more than 11 books
  if (books > 11)
    books = 11;
  
  if (rarity == RARITY_COMMON)
    return books - 1;
  else if (rarity == RARITY_UNCOMMON && books == 11)
    return 2;
  else if (rarity == RARITY_RARE && books == 11)
    return 1;
  else
    return 0;
}

u16 SpellBook::researchableSpellAmountForRarity(SpellRarity rarity, School school, u16 books)
{
  static u8 common[] = {0,3,5,6,7,8,9,10,10,10,10,10};
  static u8 uncommon[] = {0,1,2,3,4,5,6,8,10,10,10,10};
  static u8 rare[] = {0,0,1,2,3,4,5,6,7,9,10,10};
  static u8 very_rare[] ={0,0,0,1,2,3,4,5,6,7,10,10};
  
  // no benefits from more than 11 books
  if (books > 11)
    books = 11;
  
  u8* ptr = nullptr;
  
  switch (rarity)
  {
    case RARITY_COMMON: ptr = common; break;
    case RARITY_UNCOMMON: ptr = uncommon; break;
    case RARITY_RARE: ptr = rare; break;
    case RARITY_VERY_RARE: ptr = very_rare; break;
    default: return 0;
  }
  
  return ptr[books];
}

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
  s8 currentStatus[SCHOOL_COUNT][RARITY_COUNT] = {};
  s8 requiredStatus[SCHOOL_COUNT][RARITY_COUNT] = {};
  
  for (auto s : spells)
    ++currentStatus[s.first->school][s.first->rarity];
  
  for (int i = 0; i < SCHOOL_COUNT; ++i)
    for (int j = 0; j < RARITY_COUNT; ++j)
    {
      requiredStatus[i][j] = researchableSpellAmountForRarity(static_cast<SpellRarity>(j), static_cast<School>(i), booksForSchool(static_cast<School>(i)));
      requiredStatus[i][j] -= currentStatus[i][j];
    }
  
  for (int i = 0; i < SCHOOL_COUNT; ++i)
    for (int j = 0; j < RARITY_COUNT; ++j)
    {
      if (requiredStatus[i][j] > 0)
      {
        spell_list totspells = Spells::spellsByRarityAndSchool(static_cast<SpellRarity>(j), static_cast<School>(i));
        
        remove_if(totspells.begin(), totspells.end(), [this](const Spell* spell){ return spells.find(spell) != spells.end(); });
        
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

const vector<ResearchStatus> SpellBook::bookSpells(Type type) const
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





