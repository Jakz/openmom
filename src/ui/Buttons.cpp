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
    SpriteInfo info = pressed ? pressedCoords : normalCoords;
    Gfx::draw(info, x, y);
  }
}

//TODO: verify offset fomula which is not precise
BistateLabeledButton::BistateLabeledButton(const std::string name, u16 x, u16 y, SpriteInfo normal, SpriteInfo pressed, std::string label, const FontSpriteSheet* font) : BistateButton(name, x, y, normal, pressed),
label(label), font(font), textPosition(x + normal.sw()/2, y + normal.sh()/2 - font->sh()/2)
{

}

void BistateLabeledButton::setPosition(u16 x, u16 y)
{
  this->x = x;
  this->y = y;
  textPosition = ScreenCoord(x + normalCoords.sw()/2, y + normalCoords.sh()/2 - font->sh()/2);
}

void BistateLabeledButton::draw()
{
  BistateButton::draw();
  
  if (isVisible())
  {
    if (pressed)
      Fonts::drawString(label, font, textPosition.x+1, textPosition.y+1, ALIGN_CENTER);
    else
      Fonts::drawString(label, font, textPosition.x, textPosition.y, ALIGN_CENTER);
  }
}

void TristateButton::draw()
{
  if (isVisible())
  {
    SpriteInfo info = active ? (pressed ? pressedCoords : normalCoords) : inactiveCoords;
    Gfx::draw(info, x, y);
  }
}

LabeledSimpleButton::LabeledSimpleButton(const std::string name, u16 x, u16 y, SpriteInfo info, std::string label, const FontSpriteSheet* font) : OffsetButton(name,x,y,info), label(label), font(font)
{
  textX = x + info.sw()/2 - 5;
  textY = y + info.sh()/2 - font->sh()/2;
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
      //TODO: not generic for lbx/non lbx
      SpriteInfo info2 = SpriteInfo(info.texture(), info.x()+toggledOffset[0], info.y()+toggledOffset[1]);
      Gfx::draw(info2, x, y);
    }
		else
		  Gfx::draw(info, x, y);
  }
}
