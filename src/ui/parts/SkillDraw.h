#ifndef _SKILL_DRAW_H_
#define _SKILL_DRAW_H_

#include "Common.h"
#include "Unit.h"
#include "Buttons.h"

#include <string>

class SkillDraw
{
private:
  static const u16 SKILLPANE_X = 39;
  static const u16 SKILLPANE_Y = 114;
  
  static const u16 SKILLPANE_PROD_X = 85;
  static const u16 SKILLPANE_PROD_Y = 107;
  
  u16 page;
  u16 totalPages;
  
  u16 spX(s16 i, s16 sx) { return sx + (i < 4 ? 0 : 107); }
  u16 spY(s16 i, s16 sy) { return sy + (17*(i%4)); }
  
public:
  SkillDraw() : page(0), totalPages(0) { }
  
  void reset(const Unit* unit)
  {
    page = 0;
    s16 a = unit->skills()->size();
    totalPages = a/8 + (a%8 == 0 ? 0 : 1) + (unit->type() == Productable::Type::HERO ? 1 : 0);
  }
  
  u16 pages() { return totalPages; }
  bool showTopArrow() { return page > 0; }
  bool showBottomArrow() { return totalPages > 0 && page < totalPages - 1;}
  void nextPage() { ++page; }
  void prevPage() { --page; }
  bool isFirst() { return page == 0; }
  
  Clickable* clickableForIndex(s16 i) { return new Clickable(spX(i,SKILLPANE_X),spY(i,SKILLPANE_Y),96,16,BUTTON_RIGHT); }
  
  void openHelpForSkill(const Unit* unit, int i);
  void drawSkill(s16 index, s16 sprite, const std::string& text, s16 sx, s16 sy);
  void draw(const Unit* unit);
  void draw(const UnitSpec* spec);
};

#endif
