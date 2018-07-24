#ifndef _BOOK_VIEW_H_
#define _BOOK_VIEW_H_

#include "common/Common.h"
#include "Spells.h"
#include "SpellBook.h"

#include <numeric>
#include <vector>

class BookView
{
public:
  
  class Page
  {
  private:
    std::vector<ResearchStatus> pages;
    I18 title;
    bool research;
    u16 size;
    
  public:
    Page(u16 size, I18 title, bool research = false) : title(title), size(size), research(research)
    {
      pages.resize(size, ResearchStatus(nullptr));
    }
    
    u16 actualSize() const { return std::accumulate(pages.begin(), pages.end(), 0, [](u16 c, const ResearchStatus& rs) { return c + (rs.spell != nullptr ? 1 : 0); }); }
    bool isFull() const { return actualSize() == size; }
    const ResearchStatus& at(u16 i) const { return pages[i]; }
    bool isResearch() const { return research; }
    I18 getTitle() const { return title; }
    
    void put(ResearchStatus& rs) {
      for (int i = 0; i < size; ++i)
        if (!pages[i].spell)
        {
          pages[i] = rs;
          return;
        }
    }
  };
  
private:
  size_t currentPage;
  std::vector<Page> bookPages;
  const size_t pageSize;

public:
  BookView(size_t pageSize) : currentPage(0), pageSize(pageSize) { }

  void populate(const Player* player, SpellBook::Type type);
  
  bool hasNextPage() const { return currentPage < bookPages.size() - 2; }
  bool hasPrevPage() const { return currentPage > 0; }
  
  void nextPage() { currentPage += 2; }
  void prevPage() { currentPage -= 2; }
  
  const Page* current() const { return bookPages.size() > currentPage ? &bookPages[currentPage] : nullptr; }
  const Page* current(s32 i) const { return bookPages.size() > currentPage+i ? &bookPages[currentPage+i] : nullptr; }
  
  const Page& get(size_t i) { return bookPages[i]; }
  
  const ResearchStatus& getEntry(size_t i) { return bookPages.at(currentPage + i/pageSize).at(i%pageSize); }
};


#endif
