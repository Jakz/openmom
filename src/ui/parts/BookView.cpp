#include "BookView.h"

#include "Player.h"
#include "Localization.h"

using namespace std;


void BookView::populate(const Player *player, SpellBook::Type type)
{
  bookPages.clear();
  
  auto totalPool = player->book()->bookSpells(type);
  
  vector<vector<ResearchStatus>> spellsByKind;
  spellsByKind.resize(KIND_COUNT, vector<ResearchStatus>());
  
  for (auto s : totalPool)
    spellsByKind[s.spell->kind].push_back(s);
  
  std::unique_ptr<Page> page = nullptr; // TODO: should be done avoiding heap allocations
  
  for (s16 i = 0; i < KIND_COUNT; ++i)
  {
    for (auto s : spellsByKind[i])
    {
      // if current page doesn't exist or is of a different kind or is already full create another one
      if (!page || page->getTitle() != i18n::SPELL_KIND_NAMES[i] || page->isFull())
      {
        if (page && page->actualSize() > 0)
        {
          printf("Pushing page %s\n", i18n::c(i18n::SPELL_KIND_NAMES[i]));
          bookPages.push_back(*page);

          // if new page is of different kind we must specify the name so that it will be printed
          if (page->getTitle() != i18n::SPELL_KIND_NAMES[i])
          {
            page.reset(new Page(pageSize, i18n::SPELL_KIND_NAMES[i]));
          }
          else
            page.reset(new Page(pageSize, I18::EMPTY));
        }
        else if (!page)
          page.reset(new Page(pageSize, i18n::SPELL_KIND_NAMES[i]));
      }
      
      page->put(s);
    }
  }
  
  if (page && page->actualSize() > 0) bookPages.push_back(*page);
  
  if (bookPages.size() % 2 != 0) bookPages.push_back(Page(pageSize, I18::EMPTY));
  
  page.reset(nullptr);
  
  if (type == SpellBook::Type::RESEARCH)
  {
    I18 names[] = {I18::SPELL_KIND_RESEARCH, I18::SPELL_KIND_SPELLS};
    
    auto availableForResearch = player->book()->availableForResearch();
    
    for (s16 i = 0; i < pageSize; ++i)
    {
      if (i % pageSize == 0)
      {
        if (page) bookPages.push_back(*page);
        page.reset(new Page(pageSize, names[i/pageSize], true));
      }
      
      if (i < availableForResearch.size())
        page->put(availableForResearch[i]);
    }
    
    bookPages.push_back(*page);
  }

  currentPage = 0; //bookPages.size()-2;
  assert(bookPages.size() % 2 == 0);
}
