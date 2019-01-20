#ifndef _SKILL_DRAW_H_
#define _SKILL_DRAW_H_

#include "common/Common.h"
#include "common/Util.h"

#include "Buttons.h"

#include <string>

class SkillDraw
{
public:
  struct Entry //TODO: replace with std::variant
  {
    enum class Type
    {
      EXPERIENCE,
      ITEM,
      SKILL,
      FILLER
    } type;
    
    union
    {
      const Skill* skill;
      
      struct
      {
        const items::Item* data;
        items::Class type;
      } item;
      
      struct
      {
        const Level* level;
        value_t value;
      } xp;
    };
    
    Entry(const Skill* skill) : type(Type::SKILL), skill(skill) { }
    Entry(items::Class type, const items::Item* item) : type(Type::ITEM), item({item, type}) { }
    Entry(const Level* level, value_t xp) : type(Type::EXPERIENCE), xp({level, xp}) { }
    Entry() : type(Type::FILLER) { }
  };
  
private:
  
  static constexpr count_t ROWS = 4;
  static constexpr count_t TOTAL = 8;
  
  static constexpr coord_t CELL_WIDTH = 107;
  static constexpr coord_t CELL_HEIGHT = 17;
  
  Point base;
  ClickableGrid grid;
  
  size_t page;
  size_t totalPages;
  
  std::vector<Entry> entries;
  
  u16 spX(size_t i, coord_t sx) { return sx + (i < ROWS ? 0 : CELL_WIDTH); }
  u16 spY(size_t i, coord_t sy) { return sy + (CELL_HEIGHT * (i%ROWS) ); }
  
  void drawEntry(const Entry& entry, size_t index);
  
public:
  SkillDraw(Point coord = Point(0,0));
  
  void reset(const Unit* unit);
  void reset(const UnitSpec* spec);

  void setPosition(coord_t x, coord_t y);
  
  size_t pages() const { return totalPages; }
  bool showTopArrow() const { return totalPages > 1; }
  bool showBottomArrow() const { return totalPages > 1; }
  void nextPage() { page = (page + 1) % totalPages; }
  void prevPage() { page = page == 0 ? (totalPages-1) : page - 1; }
  bool isFirst() const { return page == 0; }
    
  void drawSkill(size_t index, SpriteInfo sprite, const std::string& text, coord_t sx, coord_t sy);
  void draw();
  
  ClickableGrid* clickable() { return &grid; }
};

#endif
