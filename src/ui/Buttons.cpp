//
//  Buttons.cpp
//  OpenMoM
//
//  Created by Jack on 7/19/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "Buttons.h"

#include "Gfx.h"

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
    SpriteInfo &info = pressed ? pressedCoords : normalCoords;
    Gfx::draw(info, x, y);
  }
}

void TristateButton::draw()
{
  if (isVisible())
  {
    SpriteInfo &info = active ? (pressed ? pressedCoords : normalCoords) : inactiveCoords;
    Gfx::draw(info, x, y);
  }
}