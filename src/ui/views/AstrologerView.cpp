//
//  ResearchView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "AstrologerView.h"

#include "Gfx.h"
#include "Texture.h"
#include "Buttons.h"
#include "Font.h"

#include "BookView.h"
#include "Spells.h"
#include "LocalPlayer.h"
#include "Game.h"
#include "World.h"

#include "Army.h"

#include "GfxData.h"
#include "Localization.h"

#include "ViewManager.h"

static const s16 SECTION_Y[] = { 34, 83, 132 };
static const s16 TEXT_X = 13;
static const s16 BAR_X = 51;
static const s16 BAR_WIDTH = 200;
static const s16 ROW_DELTA = 7;

static constexpr Color BAR_SHADOW_COLOR = { 24, 24, 44 };

AstrologerView::AstrologerView(ViewManager* gvm) : ViewWithQueue(gvm)
{

}

void AstrologerView::activate()
{
  setup();
}

Point AstrologerView::dialogBase()
{
  SpriteInfo bg = LSI(RELOAD, 1);
  return Point(WIDTH/2 - bg.sw()/2, HEIGHT/2 - bg.sh()/2);
}

void AstrologerView::drawRow(int section, int row, const std::string& caption, float percent, PlayerColor color)
{
  Point base = dialogBase().delta(TEXT_X, SECTION_Y[section] + row*ROW_DELTA);
  
  label(caption, FontFaces::Tiny::GOLD_COMBAT, base, ALIGN_LEFT); // TODO: correct font palette
  
  s16 barWidth = BAR_WIDTH*percent;
  Color ccolor = GfxData::playerGfxSpec(color).astrologer.color;
  
  Rect barRect = Rect(base.delta(BAR_X, 2), Size(barWidth, 2));
  Rect shadowRect = Rect(barRect.origin.x, barRect.origin.y+1, barRect.size.w+1, 2);
  
  rect(shadowRect, BAR_SHADOW_COLOR);
  rect(barRect, ccolor);
}

void AstrologerView::draw()
{
  
}

void AstrologerView::setup()
{
  sprite(LSI(RELOAD, 1), dialogBase());
  
  static const char* section_titles[] = { "view_astrologer_army_strength", "view_astrologer_magic_power", "view_astrologer_spell_research" };
  
  label(i18n::s("view_astrologer_title"), FontFaces::Serif::GOLD_SHADOW, Point(WIDTH/2-1, 19), ALIGN_CENTER); // TODO: correct font palette
  
  for (int i = 0; i < sizeof(section_titles)/sizeof(section_titles[0]); ++i)
    label(i18n::s(section_titles[i]), FontFaces::Medium::BLUE_MAGIC, Point(WIDTH/2-1, SECTION_Y[i] - 2), ALIGN_CENTER); // TODO: correct font palette

  
  /* TODO: compute real values */
  static const char* names[] = { "Dracon", "Raven", "Kali", "Sharee", "Freya" };
  static const PlayerColor colors[] = { PlayerColor::YELLOW, PlayerColor::GREEN, PlayerColor::BLUE, PlayerColor::RED, PlayerColor::PURPLE };
  
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 5; ++j)
      drawRow(i, j, names[j], Math::rand(0.4f)+0.6f, colors[j]);
}

bool AstrologerView::mouseReleased(u16 x, u16 y, MouseButton b)
{
  if (b == MouseButton::BUTTON_LEFT)
    gvm->closeOverview();
  
  return true;
}
