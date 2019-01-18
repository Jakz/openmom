#ifndef _SKILL_DRAW_H_
#define _SKILL_DRAW_H_

#include "common/Common.h"
#include "common/Util.h"

#include "Buttons.h"

#include <string>

class SkillDraw
{
private:
  
  static constexpr count_t ROWS = 4;
  static constexpr count_t TOTAL = 8;
  
  static constexpr coord_t CELL_WIDTH = 107;
  static constexpr coord_t CELL_HEIGHT = 17;
  
  Point base;
  ClickableGrid grid;
  
  size_t page;
  size_t totalPages;
  
  u16 spX(coord_t i, coord_t sx) { return sx + (i < ROWS ? 0 : CELL_WIDTH); }
  u16 spY(coord_t i, coord_t sy) { return sy + (CELL_HEIGHT * (i%ROWS) ); }
  
public:
  SkillDraw(Point coord = Point(0,0));
  
  void reset(const Unit* unit);
  void setPosition(coord_t x, coord_t y);
  
  size_t pages() const { return totalPages; }
  bool showTopArrow() const { return totalPages > 1; }
  bool showBottomArrow() const { return totalPages > 1; }
  void nextPage() { page = (page + 1) % totalPages; }
  void prevPage() { page = page == 0 ? (totalPages-1) : page - 1; }
  bool isFirst() const { return page == 0; }
    
  void openHelpForSkill(const Unit* unit, int i);
  void drawSkill(s16 index, SpriteInfo sprite, const std::string& text, coord_t sx, coord_t sy);
  void draw(const Unit* unit);
  void draw(const UnitSpec* spec);
  
  const ClickableGrid* clickable() const { return &grid; }
};

#endif
