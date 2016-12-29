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

void NormalButton::draw()
{
  /* if not visible don't draw anything */
  if (!isVisible())
    return;
  
  /* if not active */
  if (!isActive())
  {
    /* if a sprite for inactive gfx is specified use it, otherwise assert false
       since it's required
     */
    if (gfx.inactive.isPresent())
      Gfx::draw(gfx.inactive, x, y);
    else { assert(false); }
    return;
  }

  if (pressed)
  {
    /* if a sprite for pressed gfx is specified use it */
    if (gfx.pressed.isPresent())
      Gfx::draw(gfx.pressed, x, y);
    /* otherwise it's an offset button so draw normal gfx ofsetted by 1 */
    else if (gfx.normal.isPresent())
    {
      if (shouldOffsetNormal)
        Gfx::draw(gfx.normal, x+1, y+1);
      else
        Gfx::draw(gfx.normal, x, y);
    }
  }
  else if (gfx.normal.isPresent())
    Gfx::draw(gfx.normal, x, y);
  
  /* if there is a label draw it accordingly */
  if (labelGfx.isPresent())
  {
    if (pressed)
      Fonts::drawString(labelGfx->label, labelGfx->font, labelGfx->position.x+1, labelGfx->position.y+1, ALIGN_CENTER);
    else
      Fonts::drawString(labelGfx->label, labelGfx->font, labelGfx->position.x, labelGfx->position.y, ALIGN_CENTER);
  }
}

void NormalButton::setPosition(u16 x, u16 y)
{
  Button::setPosition(x, y);
  /* update label position if it was present */
  if (labelGfx.isPresent())
    labelGfx->position = ScreenCoord(x + gfx.normal->sw()/2, y + gfx.normal->sh()/2 - labelGfx->font->sh()/2);
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
