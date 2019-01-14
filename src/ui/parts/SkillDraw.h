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
  
  size_t page;
  size_t totalPages;
  
  u16 spX(coord_t i, coord_t sx) { return sx + (i < 4 ? 0 : 107); }
  u16 spY(coord_t i, coord_t sy) { return sy + (17*(i%4)); }
      
public:
  SkillDraw(Point coord = Point(0,0)) : page(0), totalPages(0), base(coord) { }
  
  void reset(const Unit* unit)
  {
    page = 0;
    s16 a = unit->skills()->size();
    totalPages = a/8 + (a%8 == 0 ? 0 : 1) + (unit->type() == Productable::Type::HERO ? 1 : 0);
  }
  
  void setPosition(coord_t x, coord_t y) { base = Point(x,y); }
  
  size_t pages() { return totalPages; }
  bool showTopArrow() { return totalPages > 1; }
  bool showBottomArrow() { return totalPages > 1; }
  void nextPage() { page = (page + 1) % totalPages; }
  void prevPage() { page = page == 0 ? (totalPages-1) : page - 1; }
  bool isFirst() { return page == 0; }
    
  void openHelpForSkill(const Unit* unit, int i);
  void drawSkill(s16 index, SpriteInfo sprite, const std::string& text, coord_t sx, coord_t sy);
  void draw(const Unit* unit);
  void draw(const UnitSpec* spec);
};

#endif
