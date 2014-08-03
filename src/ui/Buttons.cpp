//
//  Buttons.cpp
//  OpenMoM
//
//  Created by Jack on 7/19/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "Buttons.h"

#include "Gfx.h"
#include "Font.h"

void Clickable::draw()
{
  Gfx::rect(x, y, w, h, Gfx::color(255, 0, 0));
}

void SimpleButton::draw()
{
  if (isVisible())
    Gfx::draw(normalCoords, x, y);
}

void OffsetButton::draw()
{
  if (isVisible())
  {
    if (pressed)
      Gfx::draw(normalCoords, x+1, y+1);
    else
      Gfx::draw(normalCoords, x, y);
  }
}

void BistateButton::draw()
{
  if (isVisible())
  {
    const SpriteInfo &info = pressed ? pressedCoords : normalCoords;
    Gfx::draw(info, x, y);
  }
}

void TristateButton::draw()
{
  if (isVisible())
  {
    const SpriteInfo &info = active ? (pressed ? pressedCoords : normalCoords) : inactiveCoords;
    Gfx::draw(info, x, y);
  }
}

LabeledSimpleButton::LabeledSimpleButton(const std::string name, u16 x, u16 y, SpriteInfo info, std::string label, FontFace font) : OffsetButton(name,x,y,info), label(label), font(font)
{
  textX = x + Texture::get(info.texture)->sw(info.x,info.y)/2 - 4;
  textY = y + Texture::get(info.texture)->sh(info.x,info.y)/2 - Fonts::fontFor(font).h/2 + 1;
}

void LabeledSimpleButton::draw()
{
  OffsetButton::draw();
  if (!pressed)
    Fonts::drawString(label, font, textX, textY, ALIGN_CENTER);
  else
    Fonts::drawString(label, font, textX+1, textY+1, ALIGN_CENTER);
}


template<typename T>
void RadioButton<T>::draw()
{
  if (isVisible())
  {
    const SpriteInfo &info = pressed ? pressedCoords : normalCoords;
    
    if (group->getCurrent() == this)
    {
      SpriteInfo info2 = SpriteInfo(info.texture, info.x+toggledOffset[0], info.y+toggledOffset[1]);
      Gfx::draw(info2, x, y);
    }
		else
		  Gfx::draw(info, x, y);
  }
}