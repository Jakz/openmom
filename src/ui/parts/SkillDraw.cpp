#include "SkillDraw.h"

#include "Gfx.h"
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

void SkillDraw::drawSkill(s16 index, SpriteInfo sprite, const std::string& text, s16 sx, s16 sy)
{
  s16 x = spX(index,sx);
  s16 y = spY(index,sy);
  
  Gfx::draw(LSI(UNITVIEW,5).relative(index), x-1, y-1);
  Gfx::draw(sprite, x, y);
  Fonts::drawString(text, FontFaces::Medium::TEAL_STROKE, x+18, y+4, ALIGN_LEFT);
}

void SkillDraw::draw(const Unit* unit)
{
  s16 curOffset = 0;
  
  const Level* level = unit->getExperienceLevel();
  
  if (level && page == 0)
    drawSkill(curOffset++, level->visuals.icon, i18n::s(level->visuals.name)+Fonts::format(" (%u xp)",unit->getExperience()), base.x, base.y);
  
  // if the unit is a hero and we are on first page we should draw the experience and the items
  if (unit->type() == Productable::Type::HERO && page == 0)
  {
    const Hero* hero = static_cast<const Hero*>(unit);
    
    for (int i = 0; i < 3; ++i, ++curOffset)
    {
      if (!hero->itemAt(i))
      {
        const items::Class itemType = hero->getSpec()->items.types[i];
        drawSkill(curOffset, itemSprites[itemType], "", base.x, base.y);
      }
    }
  }
  else
  {
    s16 p = unit->type() == Productable::Type::HERO ? page - 1 : page;

    for (int i = p*8; i < unit->skills()->size(); ++i)
    {
      const Skill* s = unit->skills()->get(i);
      
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
