#pragma once

#include "DrawQueue.h"
#include "Gfx.h"
#include "Font.h"

struct BasicDrawEntrySorter;

class BasicDrawEntry : public DrawElement<BasicDrawEntry, BasicDrawEntrySorter>
{
private:
  s32 _z;
public:
  BasicDrawEntry() : _z(0) { }
  
  virtual void draw() const = 0;
  
  s32 z() const { return _z; }
  void setZ(s32 z) { _z = z; }
};

struct BasicDrawEntrySorter
{
public:
  bool operator()(const std::unique_ptr<BasicDrawEntry>& e1, const std::unique_ptr<BasicDrawEntry>& e2) const
  {
    return e1->z() < e2->z();
  }
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

enum TextAlign : u8;

class BasicTextEntry : public BasicDrawEntry
{
private:
  const TextInfo info;
  TextAlign align;
  
public:
  BasicTextEntry(const std::string& text, Point point) : info(text, point), align(ALIGN_LEFT) { }
  
  void draw() const override
  {
    if (info.font)
      Fonts::drawString(info.label, info.font, info.position, align);
    else
      Fonts::drawString(info.label, info.position.x, info.position.y, align);
  }
};

using BasicDrawQueue = DrawQueue<BasicDrawEntry, BasicDrawEntrySorter>;
