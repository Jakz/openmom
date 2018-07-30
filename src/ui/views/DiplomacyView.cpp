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
  SpriteInfo fadeInGfx = LSI(DIPLOMAC, 51);
  SpriteInfo speakingGfx = LSI(DIPLOMAC, 37);
  
  if (timer >= fadeInGfx.count() && phase == Phase::FADING_IN)
  {
    phase = Phase::SPEAKING;
    timer.resetCounter();
  }
  else if (timer >= speakingGfx.count() && phase == Phase::SPEAKING)
  {
    phase = Phase::WAIT;
    timer.resetCounter();
  }
  
  switch (phase)
  {
    case Phase::FADING_IN:
    {
      Gfx::draw(fadeInGfx.frame(timer), base);
      break;
    }
      
    case Phase::SPEAKING:
    {
      Gfx::draw(speakingGfx.frame(timer % speakingGfx.count()), base);
      break;
    }
      
    case Phase::WAIT:
    {
      Gfx::draw(speakingGfx.frame(frame/*speakingGfx.count()-1*/), base);
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
