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
  ~BasicDrawEntry() { }
  
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
  Point point;
  SpriteInfo info;
public:
  BasicSpriteEntry(SpriteInfo info, Point point) : info(info), point(point) { }
  
  void draw() const override
  {
    Gfx::draw(info, point);
  }
  
  void setPosition(const Point& point) { this->point = point; }
};

enum TextAlign : u8;

class BasicTextEntry : public BasicDrawEntry
{
private:
  TextInfo info;
  TextAlign align;
  
public:
  BasicTextEntry(const std::string& text, const FontSpriteSheet* face, Point point, TextAlign align) : info(text, face, point), align(align) { }
  BasicTextEntry(const std::string& text, Point point) : info(text, point), align(ALIGN_LEFT) { }
  
  void draw() const override
  {
    if (info.font)
      Fonts::drawString(info.label, info.font, info.position, align);
    else
      Fonts::drawString(info.label, info.position.x, info.position.y, align);
  }
  
  void setFace(const FontSpriteSheet* face) { info.font = face; }
  void setPosition(const Point& point) { info.position = point; }
  
  const Point& getPosition() const { return info.position; }
};

class BasicRectEntry : public BasicDrawEntry
{
private:
  const Rect rect;
  Color color;
  
public:
  BasicRectEntry(const Rect& rect, Color color) : rect(rect), color(color) { }
  BasicRectEntry(int_type x, int_type y, int_type w, int_type h, Color color) : BasicRectEntry(Rect(x,y,w,h), color) { }
  
  void draw() const override
  {
    Gfx::fillRect(rect, color);
  }
};

using BasicDrawQueue = DrawQueue<BasicDrawEntry, BasicDrawEntrySorter>;
