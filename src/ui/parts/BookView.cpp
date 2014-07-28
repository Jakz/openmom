#include "BookView.h"

#include "Player.h"
#include "Localization.h"

using namespace std;


void BookView::populate(const Player *player, SpellBook::Type type)
{
  auto totalPool = player->book()->bookSpells(type);
  
  vector<vector<ResearchStatus> > spellsByKind;
  spellsByKind.resize(KIND_COUNT, vector<ResearchStatus>());
  
  for (auto s : totalPool)
    spellsByKind[s.spell->kind].push_back(s);
  
  Page* page = nullptr; // TODO: should be done avoiding heap allocations
  
  for (s16 i = 0; i < KIND_COUNT; ++i)
  {
    for (auto s : spellsByKind[i])
    {
      // if current page doesn't exist or is of a different kind or is already full create another one
      if (!page || page->getTitle() != i18n::SPELL_KIND_NAMES[i] || page->isFull())
      {
        if (page && page->actualSize() > 0)
        {
          bookPages.push_back(*page);
          delete page;

          // if new page is of different kind we must specify the name so that it will be printed
          if (page->getTitle() != i18n::SPELL_KIND_NAMES[i])
          {
            page = new Page(pageSize, i18n::SPELL_KIND_NAMES[i]);
          }
          else
            page = new Page(pageSize, I18::EMPTY);
        }
        else if (!page)
          page = new Page(pageSize, i18n::SPELL_KIND_NAMES[i]);
      }
      
      page->put(s);
    }
  }
  
  if (page) bookPages.push_back(*page);
  
  if (bookPages.size() % 2 != 0) bookPages.push_back(Page(pageSize, I18::EMPTY));
  
  delete page;
  page = nullptr;
  
  if (type == SpellBook::Type::RESEARCH)
  {
    I18 names[] = {I18::SPELL_KIND_RESEARCH, I18::SPELL_KIND_SPELLS};
    
    auto availableForResearch = player->book()->availableForResearch();
    
    for (s16 i = 0; i < pageSize; ++i)
    {
      if (i % pageSize == 0)
      {
        if (page) bookPages.push_back(*page);
        delete page;
        page = new Page(pageSize, names[i/pageSize], true);
      }
      
      if (i < availableForResearch.size())
        page->put(availableForResearch[i]);
    }
    
    bookPages.push_back(*page);
  }
  
  delete page;
  currentPage = bookPages.size()-2;
}