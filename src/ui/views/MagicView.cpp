//
//  MagicView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "MagicView.h"

#include "LocalPlayer.h"
#include "Diplomacy.h"
#include "City.h"

#include "Gfx.h"
#include "Texture.h"
#include "Buttons.h"
#include "Font.h"

#include "ViewManager.h"

#include "Localization.h"

using namespace std;

MagicView::MagicView(ViewManager* gvm) : View(gvm)
{
  buttons.resize(BUTTON_COUNT);
  
  buttons[ALCHEMY] = TristateButton::build("Alchemy", 234, 183, TextureID::MAGIC_BUTTONS, 0)->setAction([gvm](){ gvm->switchOverview(VIEW_ALCHEMY); });
  buttons[OK] = TristateButton::build("Ok", 294, 183, TextureID::MAGIC_BUTTONS, 1)->setAction([gvm](){ gvm->switchView(VIEW_MAIN); });

}

void MagicView::activate()
{
  float total = player->getManaGain();
  
  for (int i = 0; i < DESTS; ++i)
    dests[i].v = player->manaRatio(i) / total;
}

void MagicView::draw()
{
  Gfx::draw(TextureID::MAGIC_BACKDROP, 0, 0);
  
  //draw gems of enemy wizards
  s16 c = 0;
  for (auto r : *player->getRelations())
  {
    if (r.second->isDiscovered())
    {
      if (r.first->isAlive())
      {
        u16 col = r.first->color;
        u16 wiz = r.first->wizard.ident;
        
        Gfx::draw(TextureID::MAGIC_GEMS, 0, col, 23+77*c, 3);
        Gfx::draw(TextureID::MAGIC_PORTRAITS, 0, wiz, 28+77*c, 8);
      }
      else
        Gfx::draw(TextureID::MAGIC_GEMS, 0, 6, 23+77*c, 3);
    }
    else
      Gfx::draw(TextureID::MAGIC_GEMS, 0, 5, 23+77*c, 3);
    
    ++c;
  }
  
  while (c < 4)
    Gfx::draw(TextureID::MAGIC_GEMS, 0, 5, 23+77*c++, 3);
  
  // draw global spells
  s16 yy = 60;
  for (auto cast : player->getSpells())
  {
    Fonts::drawString(i18n::s(cast.spell->name), FontFaces::Medium::BLACK, 169, yy, ALIGN_LEFT);
    yy += 8;
  }
  
  // draw mana wands
  for (int i = 0; i < 3; ++i)
  {
    s16 ix = 47;
    s16 sx = 28 + ix*i, sy = 82;
    float percent = dests[i].v;
    
    // if percent for wand is 1.0 then we just draw full wand bar
    if (dests[i].locked)
      Gfx::drawClipped(TextureID::MAGIC_MANA_WANDS, sx-1, sy-1, 16*(i*2 + 1), 0, 16, 72);
    else
      // head of the wand is drawn
      Gfx::drawClipped(TextureID::MAGIC_MANA_WANDS, sx-1, sy-1, 16*(i*2), 0, 16, 22);
    
    s16 max = 72-22;
    s16 emptyPixels = (int)(max*(1-percent));
    s16 fullPixels = max-emptyPixels;
    // wand is drawn according to proportion between full and empty
    Gfx::drawClipped(TextureID::MAGIC_MANA_WANDS, sx-1, sy-1+22, 16*(i*2), 22, 16, emptyPixels);
    Gfx::drawClipped(TextureID::MAGIC_MANA_WANDS, sx-1, sy-1+22+emptyPixels, 16*(i*2+1), 22+emptyPixels, 16, fullPixels);
  }
  
  // draw ratio values
  // TODO: check if alignment is correct from right
  Fonts::setFace(FontFaces::Medium::BLUE_MAGIC);
  
  Fonts::drawString("MP", 45, 159, ALIGN_RIGHT);
  Fonts::drawString(to_string(player->manaRatio(0)), 45-14, 159, ALIGN_RIGHT);
  
  if (player->book()->getCurrentResearch())
  {
    Fonts::drawString(to_string(player->researchPoints()), 45+47-14, 159, ALIGN_RIGHT);
    Fonts::drawString("RP", 45+47, 159, ALIGN_RIGHT);
  }
  else
    Fonts::drawString("No Spell", 45+47-3, 159, ALIGN_RIGHT);
  
  Fonts::drawString("SP", 45+47*2, 159, ALIGN_RIGHT);
  Fonts::drawString(to_string(player->manaRatio(2)), 45+47*2-14, 159, ALIGN_RIGHT);
  
  // draw misc text info on lower left
  // TODO: font is slightly different: some pixels on edges are missing
  Fonts::setFace(FontFaces::Small::BLUE_MAGIC);
  Fonts::setHorSpace(1);
  Fonts::drawString("Casting Skill:", 4, 176, ALIGN_LEFT);
  Fonts::drawString(Fonts::format("%d(%d)", player->castingSkill(), player->castingSkillBase()), 83, 176, ALIGN_RIGHT);
  
  Fonts::drawString("Magic Reserve:", 4, 176+8, ALIGN_LEFT);
  Fonts::drawString(to_string(player->totalManaPool()), 83, 176+8, ALIGN_RIGHT);
  
  Fonts::drawString("Power Base:", 4, 176+8*2, ALIGN_LEFT);
  Fonts::drawString(to_string(player->getManaGain()), 83, 176+8*2, ALIGN_RIGHT);
  
  Fonts::drawString("Casting:", 99, 176, ALIGN_LEFT);
  Fonts::drawString("None", 155, 176, ALIGN_LEFT);
  
  Fonts::drawString("Researching:", 99, 176+8, ALIGN_LEFT);
  const Spell* spell = player->book()->getCurrentResearch();
  Fonts::drawString(spell ? i18n::s(spell->name) : "None", 155, 176+8, ALIGN_LEFT);
  
  Fonts::drawString("Summon To:", 99, 176+8*2, ALIGN_LEFT);
  City* summonCity = player->cityWithSummoningCircle();
  Fonts::drawString(summonCity ? summonCity->getName() : "None", 155, 176+8*2, ALIGN_LEFT);
  
  
  /*
   int wx = 0;
   float percent = 0.35f;
   int emptyPixels = (int)(mh*(1-percent));
   Texture.drawClipped(Texture.MAGIC_MANA_WANDS, 27, 81, wx*16, 0, 16, emptyPixels);
   Texture.drawClipped(Texture.MAGIC_MANA_WANDS, 27, 81+emptyPixels, (wx+1)*16, emptyPixels, 16, 72-emptyPixels);*/
  
  //Texture.draw(Texture.MAGIC_MANA_WANDS, 0, 0, 27, 81);
  //Texture.draw(Texture.MAGIC_MANA_WANDS, 0, 2, 27+47, 81);
  //Texture.draw(Texture.MAGIC_MANA_WANDS, 0, 4, 27+47*2, 81);
}

MagicView::WandHoverInfo MagicView::hoveredWand(u16 x, u16 y)
{
  static u16 sx = 28, sy = 82;
  static u16 ix = 47;
  static u16 w = 14, h = 73;
  
  // check for coordinates inside the mana wands and calculate position as a percent
  if (y >= sy && y <= sy + h)
  {
    for (s8 i = 0; i < 3; ++i)
    {
      if (x >= sx+ix*i && x < sx+ix*i + w)
      {
        int dy = y - sy;
        
        if (dy <= 21)
        {
          return {i,0.0f,true};
        }
        else
        {
          float percent = 1.0f - (dy - 22) / (float)(h-21);
          return {i,percent,false};
        }	
      }
    }
  }
  
  return {-1,0.0f,false};
}

void MagicView::mouseReleased(u16 x, u16 y, MouseButton b)
{
  WandHoverInfo hover = hoveredWand(x, y);
  
  if (hover.index != -1)
  {
    if (hover.top)
      dests[hover.index].locked = !dests[hover.index].locked;
    else
      setPercent(hover.index, hover.percent);
  }
}

void MagicView::mouseDragged(u16 x, u16 y, MouseButton b)
{
  WandHoverInfo hover = hoveredWand(x, y);
  
  if (hover.index != -1)
  {
    if (!hover.top)
      setPercent(hover.index, hover.percent);
  }
}

void MagicView::updateValues(u16 index)
{
  int total = player->getManaGain();
  
  // TODO: finire di fixare behavior per bene perché a volte scattano i numeri 1 - 0 - 1 -0
  player->setManaRatios(
    std::round(dests[0].v*total),
    std::round(dests[1].v+ dests[0].v*total) - player->manaRatio(0),
    total - player->manaRatio(0) - player->manaRatio(1)
  );
}

void MagicView::setPercent(u16 index, float v)
{
  bool locked[] = {dests[0].locked, dests[1].locked, dests[2].locked};
  int lcount = (locked[0] ? 1 : 0) + (locked[1] ? 1 : 0) + (locked[2] ? 1 : 0);
  
  if (lcount >= 2)
    return;
  else if (lcount == 0)
  {
    ManaDest* others[2];
    int ci = 0;
    for (int i = 0; i < 3; ++i)
      if (i != index)
        others[ci++] = &dests[i];
    
    float oldValue = dests[index].v;
    float delta = - (v - oldValue);
    float halfDelta = delta / 2.0f;
    
    if (delta > 0)
    {
      if (others[0]->v + halfDelta <= 1.0f && others[1]->v + halfDelta <= 1.0f)
      {
        others[0]->v = others[0]->v + halfDelta;
        others[1]->v = others[1]->v + halfDelta;
      }
      else if (others[0]->v + halfDelta > 1.0f)
      {
        others[0]->v = 1.0f;
        others[1]->v = 1.0f - v;
      }
      else if (others[1]->v + halfDelta > 1.0f)
      {
        others[1]->v = 1.0f;
        others[0]->v = 1.0f - v;
      }
      
      dests[index].v = v;
      updateValues(index);
    }
    else if (delta < 0)
    {
      if (others[0]->v + halfDelta >= 0.0f && others[1]->v + halfDelta >= 0.0f)
      {
        others[0]->v = others[0]->v + halfDelta;
        others[1]->v = others[1]->v + halfDelta;
      }
      else if (others[0]->v + halfDelta < 0.0f)
      {
        others[0]->v = 0.0f;
        others[1]->v = 1.0f - v;
      }
      else if (others[1]->v + halfDelta < 0.0f)
      {
        others[1]->v = 0.0f;
        others[0]->v = 1.0f - v;
      }
    }
    
    dests[index].v = v;
    updateValues(index);
  }
  else if (lcount == 1)
  {
    ManaDest* other = nullptr;
    for (int i = 0; i < 3; ++i)
      if (i != index && !dests[i].locked)
        other = &dests[i];
    
    float oldValue = dests[index].v;
    float delta = - (v - oldValue);
    
    if ((delta > 0 && other->v + delta <= 1.0f) || (delta < 0 && other->v + delta >= 0.0f))
    {
      other->v = other->v + delta;
      dests[index].v = v;
      updateValues(index);
    }
  }
}