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

void ButtonGfx::draw(u16 x, u16 y, bool isActive, bool isPressed) const
{
  /* if not active */
  if (!isActive)
  {
    /* if a sprite for inactive gfx is specified use it, otherwise assert false
     since it's required
     */
    if (inactive.isPresent())
      Gfx::draw(inactive, palette, x, y);
    else { assert(false); }
    return;
  }
  
  if (isPressed)
  {
    /* if a sprite for pressed gfx is specified use it */
    if (pressed.isPresent())
      Gfx::draw(pressed, palette, x, y);
    /* otherwise it's an offset button so draw normal gfx ofsetted by 1 */
    else if (normal.isPresent())
    {
      if (shouldOffsetNormal)
        Gfx::draw(normal, palette, x+1, y+1);
      else
        Gfx::draw(normal, palette, x, y);
    }
  }
  else if (normal.isPresent())
    Gfx::draw(normal, palette, x, y);
}


void Button::draw()
{
  /* if not visible don't draw anything */
  if (!isVisible())
    return;
  
  gfx.draw(x, y, isActive(), pressed);

  /* if there is a label draw it accordingly */
  if (labelGfx.isPresent())
  {
    if (pressed)
      Fonts::drawString(labelGfx->label, labelGfx->font, labelGfx->position.x+1, labelGfx->position.y+1, ALIGN_CENTER);
    else
      Fonts::drawString(labelGfx->label, labelGfx->font, labelGfx->position.x, labelGfx->position.y, ALIGN_CENTER);
  }
}

void Button::setTextInfo(const TextInfo& info)
{
  labelGfx = info;
  setPosition(x, y);
}

void Button::setLabel(const std::string& string)
{
  assert(labelGfx.isPresent());
  labelGfx->label = string;
}

void Button::setPosition(u16 x, u16 y)
{
  Clickable::setPosition(x, y);
  /* update label position if it was present */
  //TODO: verify offset fomula which is not precise
  if (labelGfx.isPresent())
    labelGfx->position = ScreenCoord(x + gfx.normal->sw()/2, y + gfx.normal->sh()/2 - labelGfx->font->sh()/2);
}

template<typename T>
void RadioButton<T>::draw()
{
  if (!isVisible()) return;
  
  bool isSelected = group->getCurrent() == this;
  
  if (isSelected)
    Gfx::draw(pressed ? toggledGfx.pressed : toggledGfx.normal, x, y);
  else
    Button::draw();
}
