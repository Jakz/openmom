#pragma once

#include "DrawQueue.h"
#include "Gfx.h"

class BasicDrawEntry : public DrawElement<BasicDrawEntry>
{
  public: virtual void draw() const = 0;
};

class BasicSpriteEntry : public BasicDrawEntry
{
private:
  const Point point;
  const SpriteInfo info;
public:
  BasicSpriteEntry(SpriteInfo info, Point point) : info(info), point(point) { }
  
  void draw() const override
  {
    Gfx::draw(info, point);
  }
};