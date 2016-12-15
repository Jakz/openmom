//
//  ResearchView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "ResearchView.h"

#include "Gfx.h"
#include "Texture.h"
#include "Buttons.h"
#include "Font.h"

#include "BookView.h"
#include "Spells.h"
#include "LocalPlayer.h"
#include "Game.h"

#include "Localization.h"

#include "ViewManager.h"

ResearchView::ResearchView(ViewManager* gvm) : View(gvm), book(new BookView(4))
{
  buttons.resize(BUTTON_COUNT);
  
  buttons[PREV_PAGE] = new SimpleButton("Prev page", 15, 9, SpriteInfo(TextureID::SPELL_BOOK_CORNERS, 0, 0));
  buttons[NEXT_PAGE] = new SimpleButton("Next page", 288, 9, SpriteInfo(TextureID::SPELL_BOOK_CORNERS, 0, 1));
  
  
  buttons[PREV_PAGE]->setAction([this]() {
    book->prevPage();
    buttons[NEXT_PAGE]->show();
    buttons[PREV_PAGE]->showIf(book->hasPrevPage());
  });
  
  buttons[NEXT_PAGE]->setAction([this]() {
    book->nextPage();
    buttons[PREV_PAGE]->show();
    buttons[NEXT_PAGE]->showIf(book->hasNextPage());
  });
  
  u16 sx = 22, sy = 24;
  u16 dy = 37, dx = 148;
  u16 ww = 123, hh = 36;
  
  for (u16 i = 0; i < 8; ++i)
    areas.push_back((new Clickable(sx + (i/4)*dx, sy + (i%4)*dy, ww, hh))->setAction([this,i](){ indexClicked(i); }));
  
  auto backToMap = [gvm](){ gvm->switchView(VIEW_MAIN); };
  areas.push_back((new Clickable(0,10, 5, 189))->setAction(backToMap));
  areas.push_back((new Clickable(315,10, 5, 189))->setAction(backToMap));
  areas.push_back((new Clickable(6,180, 308, 19))->setAction(backToMap));
}

void ResearchView::activate()
{
  book->populate(player, SpellBook::Type::RESEARCH);
  buttons[NEXT_PAGE]->showIf(book->hasNextPage());
  buttons[PREV_PAGE]->showIf(book->hasPrevPage());
}

void ResearchView::draw()
{
  // backdrop
  Gfx::draw(TextureID::RESEARCH_BACKDROP, 0, 0);
  
  // prevpage / nextpage buttons
  
  int sx = 24, sy = 27;
  int dy = 37, dx = 148;
  
  const BookView::Page* pages[] = { book->current(), book->current(1) };
  
  for (int i = 0; i < 8; ++i)
  {
    const BookView::Page* page = pages[i/4];
    
    if (!page)
      continue;
    
    
    const ResearchStatus& status = page->at(i%4);
    
    if (status.spell)
    {
      int tx = sx + dx*(i/4);
      int ty = sy + dy*(i%4);
      
      const Spell* s = status.spell;
      int turns = 0;
      int manaCost = 0;
      
      const FontSpriteSheet* fonts[2];
      const Palette* map = nullptr;
      
      if (!status.discovered || page->isResearch())
      {
        turns = player->book()->turnsToCompleteResearch(s);
        
        if (status.spell == player->book()->getCurrentResearch())
        {
          map = FontFaces::Palettes::BLINK_WHITE_BLUE;
          turns = player->book()->turnsToCompleteResearch();
        }
        else
        {
          map = nullptr;
          turns = player->book()->turnsToCompleteResearch(s);
        }
      }
      else
      {
        manaCost = s->mana.manaCost != -1 ? s->mana.manaCost : s->mana.combatManaCost;
        turns = manaCost / player->castingSkill() + (manaCost % player->castingSkill() != 0 ? 1 : 0);
      }
      
      if (status.discovered)
      {
        fonts[0] = FontFaces::Serif::DARK_BROWN; fonts[1] = FontFaces::Tiny::BROWN;
      }
      else
      {
        fonts[0] = FontFaces::Crypt::SERIF_BROWN; fonts[1] = FontFaces::Crypt::TINY_BROWN;
      }
      
      Fonts::drawString(i18n::s(s->name), fonts[0], tx, ty, ALIGN_LEFT);
      
      if (!status.discovered || page->isResearch())
        Fonts::drawStringBounded(Fonts::format("Research Cost:%d (%d turns)", s->mana.researchCost, turns), fonts[1], tx, ty+11, 125, ALIGN_LEFT, map);
      else
        Fonts::drawStringBounded(Fonts::format("Casting Cost:%d (%d turns)", manaCost, turns), fonts[1], tx, ty+11, 125, ALIGN_LEFT, map);
      
      //TODO: real descs
      std::string descs[] = {
        "Increases target unit's melee attack strength by 1",
        "Dispels enemy spells on one unit in combat.",
        "Moves the location of your Summoning Circle to another city.",
        "Research Cost:190 (64 turns)\nEndows a unit with the ability to walk on water.",
        "Fully reveals terrain, terrain special, and cities on all squares in a targeted 'map window.'",
        "Dispels all enemy spells in the taget map square.",
        "Reveals the spell currently being cast by opponent wizards.",
        "Summons a lesser hero to lead your army."
      };
      
      
      Fonts::drawStringBounded(descs[i], FontFaces::Tiny::BROWN, tx, ty+11+6, 125, ALIGN_LEFT, map);
    }
  }
}

void ResearchView::indexClicked(u16 index)
{
  const BookView::Page* pages[] = { book->current(), book->current(1) };
  const ResearchStatus& status = pages[index/4]->at(index%4);
  
  if (status.spell)
  {
    if (!status.discovered && !player->book()->getCurrentResearch())
    {
      // TODO: start researching
      
    }
  }
}

