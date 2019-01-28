#include "SkillDraw.h"

#include "Unit.h"

#include "Gfx.h"
#include "GfxData.h"

#include "Font.h"
#include "Localization.h"
#include "Items.h"

#include "GUISettings.h"

enum_simple_map<items::Class, SpriteInfo, 6> itemSprites = {
  { items::Class::MELEE, LBXI(ITEMISC, 27) },
  { items::Class::RANGED, LBXI(ITEMISC, 28) },
  { items::Class::MELEE_STAFF, LBXI(ITEMISC, 29) },
  { items::Class::STAFF_WAND, LBXI(ITEMISC, 30) },
  { items::Class::ARMOR, LBXI(ITEMISC, 32 ) },
  { items::Class::MISC, LBXI(ITEMISC, 31 ) }
};

SkillDraw::SkillDraw(Point coord) : page(0), totalPages(0), base(coord)
{
  //grid = new ClickableGrid(coord.x - 2, coord.y - 1, CELL_WIDTH - 7, CELL_HEIGHT, ROWS, TOTAL / ROWS, 7, 0);
}

void SkillDraw::setPosition(coord_t x, coord_t y)
{
  base = Point(x, y);
  //grid->setXY(x-2, y-1);
}

ClickableGrid* SkillDraw::createClickable()
{
  return new ClickableGrid(base.x - 2, base.y - 1, CELL_WIDTH - 10, CELL_HEIGHT, ROWS, TOTAL / ROWS, 10, 0);
}

void SkillDraw::reset(const Unit* unit)
{
  page = 0;
  entries.clear();
  
  /* add experience if present */
  const Level* level = unit->getExperienceLevel();
  if (level)
    entries.emplace_back(level, unit->getExperience());
  
  /* add items if hero */
  if (unit->type() == Productable::Type::HERO)
  {
    const Hero* hero = unit->asHero();
    
    for (size_t i = 0; i < items::Item::MAX_SLOTS; ++i)
      entries.emplace_back(hero->spec->as<HeroSpec>()->items[i], hero->items()[i]);
    
    /* add 4 filler items to go next page */
    for (size_t i = 0; i < 4; ++i) entries.emplace_back();
  }

  /* add ranged if enabled */
  // TODO: in production view no ammo is shown?
  if (GUISettings::showAmmoInSkillList)
  {
    auto ranged = unit->getRangedInfo();
    if (ranged.type != Ranged::NONE)
    {
      bool useMana = unit->skills()->hasSimpleEffect(SimpleEffect::Type::USE_MANA_POOL_FOR_RANGED_ATTACKS);
      entries.emplace_back(ranged, useMana); //TODO: should use getActualRangedInfo()?
    }
  }
  
  /* add skills */
  for (const Skill* skill : *unit->skills())
    if (!skill->isHidden())
      entries.emplace_back(skill);
  
  totalPages = Math::roundWithMod(entries.size(), 8);
  
  //TODO: sort according to order in real game
}

void SkillDraw::reset(const UnitSpec *spec)
{
  page = 0;
  entries.clear();
  
  for (const Skill* skill : spec->skills)
    entries.emplace_back(skill);
  
  totalPages = Math::roundWithMod(entries.size(), TOTAL);
}

void SkillDraw::drawSkill(size_t index, SpriteInfo sprite, const std::string& text, coord_t sx, coord_t sy)
{
  coord_t x = spX(index,sx);
  coord_t y = spY(index,sy);
  
  Gfx::draw(LSI(UNITVIEW,5).relative(index), x-1, y-1);
  Gfx::draw(sprite, x, y);
  Fonts::drawString(text, FontFaces::Medium::TEAL_STROKE, x+18, y+4, ALIGN_LEFT);
}

void SkillDraw::drawEntry(const Entry &entry, size_t index)
{
  if (entry.type == Entry::Type::EXPERIENCE)
  {
    drawSkill(index, entry.xp.level->visuals.icon, fmt::format("{} ({} xp)", entry.xp.level->visuals.name, entry.xp.value), base.x, base.y);
  }
  else if (entry.type == Entry::Type::ITEM)
  {
    const items::Item* item = entry.item.data;
    
    if (!item)
    {
      /* draw empty slot with background for type */
      drawSkill(index, itemSprites[entry.item.type], "", base.x, base.y);
    }
    else
    {
      auto itemGfx = GfxData::itemGfxSpec(item->type(), item->gfx());
      
      /* draw item on empty slot */
      drawSkill(index, LSI(SPECIAL, 3), item->name(), base.x, base.y);
      drawSkill(index, itemGfx, "", base.x, base.y);
      
      /* draw item glow if present */
      if (item->school() != School::NO_SCHOOL)
      {
        coord_t x = spX(index, base.x);
        coord_t y = spY(index, base.y);
        Gfx::drawGlow(itemGfx, x, y, item->school());
      }
    }
  }
  else if (entry.type == Entry::Type::SKILL)
  {
    const Skill* skill = entry.skill;
    drawSkill(index, skill->icon(), skill->name(), base.x, base.y);
  }
  else if (entry.type == Entry::Type::AMMO)
  {
    // TODO: manage mana spent invece of ammo
    // TODO: use custom icons for boulder, ranged magical and such ?

    const char* type = "";
    switch (entry.ranged.type)
    {
      //TODO: localize
      case Ranged::ARROW: type = "Arrows"; break;
      case Ranged::BULLET: type = "Bullets"; break;
      case Ranged::ROCK: type = "Boulders"; break;
      case Ranged::NONE: assert(false); break;
      default: type = "Spells"; break;
    }

    if (entry.ranged.useManaPool)
      drawSkill(index, LSI(SPECIAL2, 25), fmt::format("{} {}mp", type, entry.ranged.value), base.x, base.y);
    else
      drawSkill(index, LSI(SPECIAL2, 25), fmt::format("{} x{}", type, entry.ranged.value), base.x, base.y);
  }
}

void SkillDraw::draw()
{
  for (size_t i = 0; i < TOTAL; ++i)
    if (i + TOTAL*page < entries.size())
      drawEntry(entries[i + TOTAL*page], i);
}
