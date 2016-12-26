//
//  MirrorView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "HelpView.h"

#include "LocalPlayer.h"

#include "Font.h"
#include "Gfx.h"
#include "Texture.h"
#include "CommonDraw.h"
#include "View.h"
#include "ViewManager.h"

using namespace help;

constexpr u16 px = 20;
constexpr u16 py = 20;
constexpr u16 bound = 150;

HelpView::HelpView(ViewManager* gvm) : View(gvm)
{
  
}

void HelpView::activate()
{
  entry = help::Entry(help::Data::at(133));
}

void HelpView::draw()
{
  const auto* p = entry.paragraph;
  
  const auto* titleFace = FontFaces::Serif::BROWN_HELP;
  const auto* textFace = FontFaces::Small::BROWN_HELP;
  
  u16 bx = px, by = py;
  
  Gfx::draw(LSI(HELP, 0), bx, by);
  bx += 17; by += 25;
  
  Fonts::drawString(p->title, titleFace, bx, by, ALIGN_LEFT);
  Fonts::drawStringBounded(p->text, textFace, bx + 2, by + titleFace->ver + titleFace->sh(), bound, ALIGN_LEFT);
  
}

void HelpView::mousePressed(u16 x, u16 y, MouseButton b)
{

}

void HelpView::keyPressed(KeyboardCode key, KeyboardKey kkey, KeyboardMod mod)
{

}
