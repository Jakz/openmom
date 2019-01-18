#include "SkillDraw.h"

#include "Unit.h"

#include "Gfx.h"
#include "GfxData.h"

#include "Font.h"
#include "Localization.h"
#include "Items.h"

enum_simple_map<items::Class, SpriteInfo, 6> itemSprites = {
  { items::Class::MELEE, LBXI(ITEMISC, 27) },
  { items::Class::RANGED, LBXI(ITEMISC, 28) },
  { items::Class::MELEE_STAFF, LBXI(ITEMISC, 29) },
  { items::Class::STAFF_WAND, LBXI(ITEMISC, 30) },
  { items::Class::ARMOR, LBXI(ITEMISC, 32 ) },
  { items::Class::MISC, LBXI(ITEMISC, 31 ) }
};

void SkillDraw::openHelpForSkill(const Unit* unit, int i)
{
  // TODO
  /*Skill s = u.getAbility(i);
  LocalGame.i.currentPlayer.send(new HelpSkillMessage(s));
  GameViewManager.instance.switchOverView(ViewType.MESSAGE);*/
  
  // TODO manage skills which are not real skills (xp, items, etc)
}

SkillDraw::SkillDraw(Point coord) : page(0), totalPages(0), base(coord)
{
  grid = ClickableGrid(coord.x, coord.y, CELL_WIDTH - 4, CELL_HEIGHT, ROWS, TOTAL / ROWS);
}

void SkillDraw::setPosition(coord_t x, coord_t y)
{
  base = Point(x, y);
  grid.setXY(x, y);
}

void SkillDraw::reset(const Unit* unit)
{
  page = 0;
  size_t a = unit->skills()->size();
  totalPages = Math::roundWithMod(a, 8) + (unit->type() == Productable::Type::HERO ? 1 : 0);
}

void SkillDraw::drawSkill(s16 index, SpriteInfo sprite, const std::string& text, coord_t sx, coord_t sy)
{
  coord_t x = spX(index,sx);
  coord_t y = spY(index,sy);
  
  Gfx::draw(LSI(UNITVIEW,5).relative(index), x-1, y-1);
  Gfx::draw(sprite, x, y);
  Fonts::drawString(text, FontFaces::Medium::TEAL_STROKE, x+18, y+4, ALIGN_LEFT);
}

void SkillDraw::draw(const Unit* unit)
{
  int curOffset = 0;
  
  const Level* level = unit->getExperienceLevel();
  
  if (level && page == 0)
    drawSkill(curOffset++, level->visuals.icon, fmt::format("{} ({} xp)", level->visuals.name, unit->getExperience()), base.x, base.y);
  
  /* if the unit is a hero and we are on first page we should draw the experience and the items */
  if (unit->type() == Productable::Type::HERO && page == 0)
  {
    const Hero* hero = static_cast<const Hero*>(unit);
    
    for (int i = 0; i < items::Item::MAX_SLOTS; ++i, ++curOffset)
    {
      const items::Item* item = hero->items()[i];
      
      if (!item)
      {
        /* draw empty slot with background for type */
        const items::Class itemType = hero->getSpec()->items.types[i];
        drawSkill(curOffset, itemSprites[itemType], "", base.x, base.y);
      }
      else
      {
        auto itemGfx = GfxData::itemGfxSpec(item->type(), item->gfx());
        
        /* draw item on empty slot */
        drawSkill(curOffset, LSI(SPECIAL, 3), item->name(), base.x, base.y);
        drawSkill(curOffset, itemGfx, "", base.x, base.y);
        
        /* draw item glow if present */
        if (item->school() != School::NO_SCHOOL)
        {
          coord_t x = spX(curOffset, base.x);
          coord_t y = spY(curOffset, base.y);
          Gfx::drawGlow(itemGfx, x, y, item->school());
        }
      }
    }
  }
  else
  {
    size_t p = unit->type() == Productable::Type::HERO ? page - 1 : page;
    const auto* skills = unit->skills();
    const size_t size = skills->size();

    for (size_t i = p*8; i < size; ++i)
    {
      const Skill* s = skills->get(i);
      
      if (!s->isHidden())
        drawSkill(curOffset++, s->icon(), s->name(), base.x, base.y);
    }
  }
}

void SkillDraw::draw(const UnitSpec* spec)
{
  s16 curOffset = 0;
  
  for (auto skill : spec->skills)
    drawSkill(curOffset++, skill->icon(), skill->name(), base.x, base.y);
}
