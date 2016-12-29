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
#include "GfxData.h"
#include "Texture.h"
#include "CommonDraw.h"
#include "View.h"
#include "ViewManager.h"

MirrorView::MirrorView(ViewManager* gvm) : View(gvm)
{
  
}

void MirrorView::draw()
{
  Gfx::draw(LSI(BACKGRND, 4), 70, 2); // mirror bg
  Gfx::draw(GfxData::wizardGfxSpec(player->wizard.ident).getGemmedPortrait(player->color), 81, 13);
  Fonts::drawString(player->name, FontFaces::Serif::YELLOW_SHADOW, 171, 12, ALIGN_CENTER);
  
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
  
  // TODO: font is tiny + small mixed
  Fonts::drawString(Fonts::format("%u GP",player->totalGoldPool()), FontFaces::Tiny::YELLOW_STROKE, 98, 76, ALIGN_CENTER);
  Fonts::drawString(Fonts::format("%u MP",player->totalManaPool()), FontFaces::Tiny::YELLOW_STROKE, 224, 76, ALIGN_CENTER);
}

void MirrorView::mousePressed(u16 x, u16 y, MouseButton b)
{
  gvm->closeOverview();
}

void MirrorView::keyPressed(KeyboardCode key, KeyboardKey kkey, KeyboardMod mod)
{
  if (key == SDL_SCANCODE_RETURN)
    gvm->closeOverview();
}
