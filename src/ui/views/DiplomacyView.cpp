//
//  ResearchView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "DiplomacyView.h"

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

DiplomacyView::DiplomacyView(ViewManager* gvm) : ViewWithQueue(gvm), eyesColorLevel(0), timer(3)
{

}

void DiplomacyView::activate()
{
  eyesColorLevel = 4;
  
  wizard = Data::wizard("freya");
  mood = Mood::HAPPY;
  phase = Phase::FADING_IN;
  
  timer.reset();
  timer.mark(Gfx::fticks*2);
  
  setup();
}

void DiplomacyView::setup()
{
  /* bg */
  sprite(LSI(DIPLOMAC, 0), {0, 0});
  
  /* eyes */
  sprite(LSI(DIPLOMAC, 2).relative(eyesColorLevel), { 64, 58 });
  sprite(LSI(DIPLOMAC, 13).relative(eyesColorLevel), { 232, 58 });
}

static int frame = 0;

void DiplomacyView::draw()
{
  timer.set(Gfx::fticks*2);
  
  const Point base = Point(107, 13);
  
  const auto& gfx = GfxData::wizardGfx(wizard).diplomacy;

  if (timer >= gfx.fadeIn.count() && phase == Phase::FADING_IN)
  {
    phase = Phase::SPEAKING;
    timer.resetCounter();
  }
  else if (timer >= gfx.speaking.count() && phase == Phase::SPEAKING)
  {
    phase = Phase::WAIT;
    timer.resetCounter();
  }
  
  switch (phase)
  {
    case Phase::FADING_IN:
    {
      Gfx::draw(gfx.fadeIn.frame(timer), base);
      break;
    }
      
    case Phase::SPEAKING:
    {
      Gfx::draw(gfx.speaking.frame(timer % gfx.speaking.count()), base);
      break;
    }
      
    case Phase::WAIT:
    {
      SpriteInfo info;
      
      switch (mood)
      {
        case Mood::HAPPY: info = gfx.mood.frame(0); break;
        case Mood::ANGRY: info = gfx.mood.frame(1); break;
        case Mood::NEUTRAL: info = gfx.mood.frame(2); break;
      }
      
      Gfx::draw(info, base);
      break;
    }
      
    default:
    {
      assert(false);
    }
  }
  
  Gfx::draw(LSI(BACKGRND, 18), base - Point(12,12));
}

bool DiplomacyView::mouseReleased(u16 x, u16 y, MouseButton b)
{
  ++frame;
  return true;
  
  if (b == MouseButton::BUTTON_LEFT)
    gvm->switchView(VIEW_MAIN);
  
  return true;
}
