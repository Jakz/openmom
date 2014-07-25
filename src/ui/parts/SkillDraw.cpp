#include "SkillDraw.h"

#include "Gfx.h"
#include "Font.h"
#include "Localization.h"
#include "Items.h"

void SkillDraw::openHelpForSkill(const Unit* unit, int i)
{
  // TODO
  /*Skill s = u.getAbility(i);
  LocalGame.i.currentPlayer.send(new HelpSkillMessage(s));
  GameViewManager.instance.switchOverView(ViewType.MESSAGE);*/
  
  // TODO manage skills which are not real skills (xp, items, etc)
}

void SkillDraw::drawSkill(s16 index, s16 sprite, const std::string& text, s16 sx, s16 sy)
{
  s16 x = spX(index,sx);
  s16 y = spX(index,sy);
  
  Gfx::draw(TextureID::UNIT_SKILL_ICONS, sprite, x, y);
  Fonts::drawString(text, FontFace::TEAL_MEDIUM_STROKE, x+19, y+4, ALIGN_LEFT);
}

void SkillDraw::draw(const Unit* unit)
{
  s16 curOffset = 0;
  
  // if the unit is a hero and we are on first page we should draw the experience and the items
  if (unit->type() == Productable::Type::HERO && page == 0)
  {
    const Hero* hero = static_cast<const Hero*>(unit);
    
    drawSkill(curOffset++, unit->level->index(), i18n::s(unit->level->name)+Fonts::format(" (%u xp)",unit->getExperience()), SKILLPANE_X, SKILLPANE_Y);
    for (int i = 0; i < 3; ++i, ++curOffset)
    {
      if (!hero->itemAt(i))
        drawSkill(curOffset, ItemSlots::slotsFor(static_cast<const HeroSpec&>(hero->spec).items)->sprites[i], "", SKILLPANE_X, SKILLPANE_Y);
    }
  }
  else
  {
    s16 p = unit->type() == Productable::Type::HERO ? page - 1 : page;
    
    if (unit->type() != Productable::Type::SUMMON && page == 0)
      drawSkill(curOffset++, unit->level->index(), i18n::s(unit->level->name)+Fonts::format(" (%u xp)",unit->getExperience()), SKILLPANE_X, SKILLPANE_Y);
    
    for (int i = p*8; i < unit->skills()->size(); ++i)
    {
      const Skill* s = unit->skills()->get(i);
      drawSkill(curOffset++, static_cast<u16>(s->base), i18n::s(s->base), SKILLPANE_X, SKILLPANE_Y);
    }
  }
}

void SkillDraw::draw(const UnitSpec* spec)
{
  s16 curOffset = 0;
  
  for (auto skill : spec->skills)
    drawSkill(curOffset++, static_cast<u16>(skill->base), i18n::s(skill->base), SKILLPANE_PROD_X, SKILLPANE_PROD_Y);
}