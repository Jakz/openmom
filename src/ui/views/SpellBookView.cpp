//
//  SpellBookView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "SpellBookView.h"

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

static const u16 topLeftX[] = {32,164}, topLeftY[]= {31,31};
static const u16 ROW_WIDTH = 115, ROW_HEIGHT = 22;

SpellBookView::SpellBookView(ViewManager* gvm) : View(gvm), book(new BookView(6)), combatMode(false)
{
  buttons.resize(BUTTON_COUNT);
  
  buttons[PREV_PAGE] = new SimpleButton("Prev page", 29, 14, SpriteInfo(TextureID::SPELL_BOOK_CORNERS, 0, 0));
  buttons[NEXT_PAGE] = new SimpleButton("Next page", 274, 14, SpriteInfo(TextureID::SPELL_BOOK_CORNERS, 0, 1));
  
  
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
  
  areas.push_back((new Clickable(175,165,18,29))->setAction([gvm](){ gvm->closeOverview(); }));


  
  for (u16 k = 0; k < 2; ++k)
    for (u16 i = 0; i < 6;++i)
      areas.push_back((new Clickable(topLeftX[k]-2,topLeftY[k]+22*i-2,125,22))->setAction([k,i,this](){
        startCast(book->getEntry(k*6 + i));
      }));
}

void SpellBookView::activate()
{
  combatMode = player->getCombat();
  book->populate(player, combatMode ? SpellBook::Type::COMBAT : SpellBook::Type::OVERLAND);
  buttons[NEXT_PAGE]->showIf(book->hasNextPage());
  buttons[PREV_PAGE]->showIf(book->hasPrevPage());
}

void SpellBookView::startCast(const ResearchStatus &spell)
{
  if (!spell.spell) return;
  
  if (!combatMode)
    g->startCast(player, spell.spell);
  else
    g->castCombatspell(player, spell.spell);
  
  gvm->closeOverview();
}

void SpellBookView::drawPage(u16 index)
{
  const BookView::Page* page = book->current(index);
  
  if (!page)
    return;
  
  Fonts::drawString(i18n::s(page->getTitle()), FontFaces::Serif::BROWN, topLeftX[index]+52, topLeftY[index]-16, ALIGN_CENTER);
  
  for (int i = 0; i < 6; ++i)
  {
    const ResearchStatus& entry = page->at(i);
    
    if (entry.spell)
    {
      const Spell* spell = entry.spell;
      
      int curY = topLeftY[index]+ROW_HEIGHT*i;
      
      int clip, clip2;
      
      if (spell != player->book()->getCurrentCast())
      {
        clip = Fonts::drawString(i18n::s(spell->name), FontFaces::Small::BROWN, topLeftX[index]-1, curY-1, ALIGN_LEFT);
        clip2 = Fonts::drawString(Fonts::format("%d MP",g->spellMechanics.actualManaCost(player, spell, combatMode)), FontFaces::Small::BROWN, topLeftX[index]+ROW_WIDTH-1, curY-1, ALIGN_RIGHT);
      }
      else
      {
        clip = Fonts::drawString(i18n::s(spell->name), FontFaces::Small::BLINK_WHITE_GREY, topLeftX[index]-1, curY-1, ALIGN_LEFT);
        clip2 = Fonts::drawString(Fonts::format("%d MP",g->spellMechanics.actualManaCost(player, spell, combatMode)), FontFaces::Small::BLINK_WHITE_GREY, topLeftX[index]+ROW_WIDTH-1, curY-1, ALIGN_RIGHT);
      }
      
      Gfx::drawClipped(TextureID::SPELL_BOOK_DUMMY_TEXT, topLeftX[index]+clip+4, curY, clip+4, ROW_HEIGHT*i, -4-clip2, 6);
      
      
      s16 turns;
      bool instant = false;
      
      if (!combatMode)
      {
        // if we're not in combat mode mana symbols are used to show how many turns are needed for the cast
        if (player->getAvailableMana() > 0)
        {
          turns = g->spellMechanics.actualManaCost(player, spell, combatMode) / player->getAvailableMana();
          instant = turns == 0 ? true : false;
          if (turns == 0)
            turns = 1;
        }
        else
          turns = 40;
      }
      else
      {
        // in combat mode symbols are used to show how many casts of the spell are allowed according to cast skill
        // TODO: real calculation of spell skill
        turns = 100 / g->spellMechanics.actualManaCost(player, spell, combatMode);
      }
      
      
      
      // draw mana symbols on spell entry
      if (!instant)
      {
        s16 toDo = std::min((int)turns, 20);
        for (int j = 0; j < toDo; ++j)
          Gfx::draw(TextureID::SPELL_BOOK_SCHOOL_ICONS, 0, spell->school, topLeftX[index]+1+6*j, curY+6+1);
        Gfx::drawClipped(TextureID::SPELL_BOOK_DUMMY_TEXT, topLeftX[index]+1+toDo*6+4, curY+6, 1+toDo*6+4, ROW_HEIGHT*i+6, 0, 6);
        
        if (turns > 20)
        {
          toDo = std::min(turns-20, 20);
          for (int j = 0; j < toDo; ++j)
            Gfx::draw(TextureID::SPELL_BOOK_SCHOOL_ICONS, 0, spell->school, topLeftX[index]+1+6*j, curY+12+1);
          Gfx::drawClipped(TextureID::SPELL_BOOK_DUMMY_TEXT, topLeftX[index]+1+toDo*6+4, curY+12, 1+toDo*6+4, ROW_HEIGHT*i+6, 0, 6);
        }
        else
          Gfx::drawClipped(TextureID::SPELL_BOOK_DUMMY_TEXT, topLeftX[index], curY+12, 0, ROW_HEIGHT*i+6, 0, 6);
      }
      else
      {
        Gfx::draw(TextureID::SPELL_BOOK_SCHOOL_ICONS, 0, spell->school, topLeftX[index]+1, curY+6+1);
        // TODO: localize
        int w = 0;
        if (!combatMode)
          w = Fonts::drawString("Instant", FontFaces::Small::BROWN, topLeftX[index]+8, curY+6, ALIGN_LEFT);
        Gfx::drawClipped(TextureID::SPELL_BOOK_DUMMY_TEXT, topLeftX[index]+9+w+4, curY+6, 9+w+4, ROW_HEIGHT*i+6, 0, 6);
        Gfx::drawClipped(TextureID::SPELL_BOOK_DUMMY_TEXT, topLeftX[index], curY+12, 0, ROW_HEIGHT*i+6, 0, 6);
      }
    }
  }
}

void SpellBookView::draw()
{
  Gfx::draw(TextureID::SPELL_BOOK_BACKDROP, 16, 11);
  drawPage(0);
  drawPage(1);
}