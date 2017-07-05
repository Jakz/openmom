#ifndef _SKILL_DRAW_H_
#define _SKILL_DRAW_H_

#include "common/Common.h"
#include "Unit.h"
#include "Buttons.h"

#include <string>

class SkillDraw
{
private:
  Point base;
  
  u16 page;
  u16 totalPages;
  
  u16 spX(s16 i, s16 sx) { return sx + (i < 4 ? 0 : 107); }
  u16 spY(s16 i, s16 sy) { return sy + (17*(i%4)); }
      
public:
  SkillDraw(Point coord = Point(0,0)) : page(0), totalPages(0), base(coord) { }
  
  void reset(const Unit* unit)
  {
    page = 0;
    s16 a = unit->skills()->size();
    totalPages = a/8 + (a%8 == 0 ? 0 : 1) + (unit->type() == Productable::Type::HERO ? 1 : 0);
  }
  
  void setPosition(u16 x, u16 y) { base = Point(x,y); }
  
  u16 pages() { return totalPages; }
  bool showTopArrow() { return page > 0; }
  bool showBottomArrow() { return totalPages > 0 && page < totalPages - 1;}
  void nextPage() { ++page; }
  void prevPage() { --page; }
  bool isFirst() { return page == 0; }
    
  void openHelpForSkill(const Unit* unit, int i);
  void drawSkill(s16 index, SpriteInfo sprite, const std::string& text, s16 sx, s16 sy);
  void draw(const Unit* unit);
  void draw(const UnitSpec* spec);
};

#endif
