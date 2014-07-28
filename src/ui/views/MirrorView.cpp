//
//  MirrorView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "MirrorView.h"

#include "LocalPlayer.h"

#include "Font.h"
#include "Gfx.h"
#include "Texture.h"
#include "CommonDraw.h"
#include "View.h"
#include "ViewManager.h"

MirrorView::MirrorView(ViewManager* gvm) : View(gvm)
{
  
}

void MirrorView::draw()
{
  Gfx::draw(TextureID::MAIN_MIRROR_BACKDROP, 70, 2);
  CommonDraw::drawWizardGem(player, 81, 13);
  Fonts::drawString(player->name, FontFace::YELLOW_SERIF_SHADOW, 172, 12, ALIGN_CENTER);
  
  u16 t = 0;
  u16 tot = player->book()->totalBooks();
  for (u16 i = 0; i < SCHOOL_COUNT; ++i)
  {
    int nb = player->book()->booksForSchool(static_cast<School>(i));
    
    for (int j = 0; j < nb; ++j)
    {
      Gfx::draw(TextureID::WIZARD_SPELLBOOKS, (i+j*2+1)%3, i, 160 - (tot*8)/2 + t, 68);
      t += 8;
    }
  }
  
  Fonts::drawString(Fonts::format("%u GP",player->totalGoldPool()), FontFace::YELLOW_TINY_STROKE, 96, 76, ALIGN_CENTER);
  Fonts::drawString(Fonts::format("%u MP",player->totalManaPool()), FontFace::YELLOW_TINY_STROKE, 222, 76, ALIGN_CENTER);
}

void MirrorView::mousePressed(u16 x, u16 y, MouseButton b)
{
  gvm->closeOverview();
}

void MirrorView::keyPressed(KeyboardKey key, KeyboardMod mod)
{
  if (key == SDL_SCANCODE_RETURN)
    gvm->closeOverview();
}