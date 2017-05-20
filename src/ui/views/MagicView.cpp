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
#include "GfxData.h"
#include "LBX.h"
#include "Texture.h"
#include "Buttons.h"
#include "Font.h"

#include "ViewManager.h"

#include "Localization.h"

using namespace std;

enum
{
  gem_undiscovered = LBXI(MAGIC, 6),
  gem_shattered = LBXI(MAGIC, 51),
  
  staff_mana_empty = LBXI(MAGIC, 7),
  staff_research_empty = LBXI(MAGIC, 9),
  staff_skill_empty = LBXI(MAGIC, 11),
  
  staff_mana_head = LBXI(MAGIC, 15),
  staff_research_head = LBXI(MAGIC, 16),
  staff_skill_head = LBXI(MAGIC, 17),
  
  staff_mana_full = LBXI(MAGIC, 8),
  staff_research_full = LBXI(MAGIC, 10),
  staff_skill_full = LBXI(MAGIC, 12),
  
  background = LBXI(MAGIC, 0)
};

SpriteInfo wands[][3] = {
  { staff_mana_empty, staff_mana_full, staff_mana_head },
  { staff_research_empty, staff_research_full, staff_research_head },
  { staff_skill_empty, staff_skill_full, staff_skill_head },
};

MagicView::MagicView(ViewManager* gvm) : View(gvm)
{
  buttons.resize(BUTTON_COUNT);
  
  const Palette* palette = lbx::Repository::spriteFor(background)->palette;
  
  //TODO: check positioning of buttons
  buttons[ALCHEMY] = Button::buildBistate("Alchemy", 234, 181, LSI(MAGIC,13))->setAction([gvm](){ gvm->switchOverview(VIEW_ALCHEMY); });
  buttons[ALCHEMY]->setPalette(palette);

  buttons[OK] = Button::buildBistate("Ok", 290, 181, LSI(MAGIC,14))->setAction([gvm](){ gvm->switchView(VIEW_MAIN); });
  buttons[OK]->setPalette(palette);

}

void MagicView::activate()
{
  float total = player->getManaGain();
  
  for (size_t i = 0; i < DESTS; ++i)
    dests[i].v = player->manaRatio(i) / total;
}

void MagicView::draw()
{
  Gfx::draw(background, 0, 0);
  
  //draw gems of enemy wizards
  s16 c = 0;
  for (auto r : *player->getRelations())
  {
    if (r.second->isDiscovered())
    {
      if (r.first->isAlive())
      {
        const auto& wizardGfx = GfxData::wizardGfxSpec(r.first->wizard.ident);
        Gfx::draw(wizardGfx.getGemmedPortrait(r.first->color), 24+77*c, 4);
      }
      else
        Gfx::draw(gem_shattered, 24+77*c, 4);
    }
    else
      Gfx::draw(gem_undiscovered, 24+77*c, 4);
    
    ++c;
  }
  
  while (c < 4)
    Gfx::draw(gem_undiscovered, 24+77*c++, 4);

  // draw global spells
  s16 yy = 60;
  for (auto cast : player->getSpells())
  {
    Fonts::drawString(i18n::s(cast.spell->name), FontFaces::Medium::BLACK, 169, yy, ALIGN_LEFT);
    yy += 8;
  }
  
  // draw mana wands
  const s16 wand_x[] = { 29, 28+47, 28+47*2 };
  const s16 wand_head_adjust[][2] = {{-2,-2},{-1,-4},{-1,-2}};
  for (int i = 0; i < 3; ++i)
  {
    s16 sx = wand_x[i], sy = 83 + 70;
    float percent = dests[i].v;
    
    Gfx::draw(wands[i][0], sx, sy - wands[i][0].sh());
    
    if (dests[i].locked)
      Gfx::draw(wands[i][2], sx + wand_head_adjust[i][0], sy + wand_head_adjust[i][1] - wands[i][0].sh());
    
    s16 max = 51;
    s16 emptyPixels = (int)(max*(1-percent));
    s16 fullPixels = max-emptyPixels;
    Gfx::drawClipped(wands[i][1], sx+4 - (i == 0 ? 1 : 0), sy-fullPixels, 0, emptyPixels, 6, fullPixels);
  }
  
  // draw ratio values
  // TODO: check if alignment is correct from right
  Fonts::setFace(FontFaces::Medium::BLUE_MAGIC);
  
  s16 baseX = 55;
  
  Fonts::drawString("MP", baseX, 159, ALIGN_RIGHT);
  Fonts::drawString(to_string(player->manaRatio(0)), baseX-14, 159, ALIGN_RIGHT);
  
  if (player->book()->getCurrentResearch())
  {
    Fonts::drawString(to_string(player->researchPoints()), baseX+47-14, 159, ALIGN_RIGHT);
    Fonts::drawString("RP", baseX+47, 159, ALIGN_RIGHT);
  }
  else
    Fonts::drawString("No Spell", baseX+47-3, 159, ALIGN_RIGHT);
  
  Fonts::drawString("SP", baseX+47*2, 159, ALIGN_RIGHT);
  Fonts::drawString(to_string(player->manaRatio(2)), baseX+47*2-14, 159, ALIGN_RIGHT);
  
  // draw misc text info on lower left
  // TODO: font is slightly different: some pixels on edges are missing
  baseX = 91;
  
  Fonts::setFace(FontFaces::Small::BLUE_MAGIC);
  Fonts::setHorSpace(1);
  Fonts::drawString("Casting Skill:", 4, 176, ALIGN_LEFT);
  Fonts::drawString(Fonts::format("%d(%d)", player->castingSkill(), player->castingSkillBase()), baseX, 176, ALIGN_RIGHT);
  
  Fonts::drawString("Magic Reserve:", 4, 176+8, ALIGN_LEFT);
  Fonts::drawString(to_string(player->totalManaPool()), baseX, 176+8, ALIGN_RIGHT);
  
  Fonts::drawString("Power Base:", 4, 176+8*2, ALIGN_LEFT);
  Fonts::drawString(to_string(player->getManaGain()), baseX, 176+8*2, ALIGN_RIGHT);
  
  Fonts::drawString("Casting:", 99, 176, ALIGN_LEFT);
  Fonts::drawString("None", 155, 176, ALIGN_LEFT);
  
  Fonts::drawString("Researching:", 99, 176+8, ALIGN_LEFT);
  const Spell* spell = player->book()->getCurrentResearch();
  Fonts::drawString(spell ? i18n::s(spell->name) : "None", 155, 176+8, ALIGN_LEFT);
  
  Fonts::drawString("Summon To:", 99, 176+8*2, ALIGN_LEFT);
  City* summonCity = player->cityWithSummoningCircle();
  Fonts::drawString(summonCity ? summonCity->getName() : "None", 155, 176+8*2, ALIGN_LEFT);
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

bool MagicView::mouseReleased(u16 x, u16 y, MouseButton b)
{
  WandHoverInfo hover = hoveredWand(x, y);
  
  if (hover.index != -1)
  {
    if (hover.top)
      dests[hover.index].locked = !dests[hover.index].locked;
    else
      setPercent(hover.index, hover.percent);
    
    return true;
  }
  
  return false;
}

bool MagicView::mouseDragged(u16 x, u16 y, MouseButton b)
{
  WandHoverInfo hover = hoveredWand(x, y);
  
  if (hover.index != -1)
  {
    if (!hover.top)
      setPercent(hover.index, hover.percent);
    
    return true;
  }
  
  return false;
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
