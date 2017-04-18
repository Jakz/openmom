//
//  SpellBookView.h
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _SPELL_BOOK_VIEW_H_
#define _SPELL_BOOK_VIEW_H_

#include "View.h"
#include "BookView.h"

enum class SpellKind : u8;

class ViewManager;

class SpellBookView : public View
{
private:
  enum
  {
    PREV_PAGE,
    NEXT_PAGE,
    
    BUTTON_COUNT
  };
  
  BookView *book;
  bool combatMode;

  void draw() override;
  void drawPost() override { }
  
  void drawPage(u16 index);
  void startCast(const ResearchStatus& spell);
  
public:
  SpellBookView(ViewManager* gvm);
  
  void activate() override;
  void deactivate() override { }
};

#endif
