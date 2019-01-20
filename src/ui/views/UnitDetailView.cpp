//
//  UnitDetailView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "UnitDetailView.h"

#include "ViewManager.h"

#include "CommonDraw.h"
#include "SkillDraw.h"
#include "UnitDraw.h"
#include "Gfx.h"
#include "Texture.h"
#include "Font.h"
#include "Buttons.h"

#include "UnitSpec.h"
#include "Unit.h"

static const Point normalBaseCoords = Point(31, 6);
static const Point viewOnlyBaseCoord = Point(61, 6);
static const Point heroHireCoords = Point(25, 17);


enum lbx_indices
{
  full_unit_backdrop = LBXI(UNITVIEW, 1),
  buttons_backdrop = LBXI(UNITVIEW, 2),
  
  hero_hire_banner = LBXI(HIRE, 0),
  hero_portrait_border = LBXI(UNITVIEW, 33),
  
  up_arrow = LBXI(UNITVIEW, 3),
  down_arrow = LBXI(UNITVIEW, 4)
};

UnitDetailView::UnitDetailView(ViewManager* gvm) : View(gvm), unit(nullptr), mode(Mode::NORMAL), c(normalBaseCoords)
{
  buttons.resize(BUTTON_COUNT);
  
  buttons[HIGH_ACTION] = Button::buildBistate("High Action", 0, 0, LSI(BACKGRND, 24), "", fonts::base::SERIF_GOLD)->setAction([this](){this->buttonClicked(HIGH_ACTION);});
  buttons[LOW_ACTION] = Button::buildBistate("Low Action", 0, 0, LSI(BACKGRND, 24), "", fonts::base::SERIF_GOLD)->setAction([this](){this->buttonClicked(LOW_ACTION);});

  buttons[UP_ARROW] = Button::buildTristate("Up", 0, 0, up_arrow)->setAction([this](){skillDraw.prevPage();});
  buttons[DOWN_ARROW] = Button::buildTristate("Down", 0, 0, down_arrow)->setAction([this](){skillDraw.nextPage();});
}

void UnitDetailView::buttonClicked(button button)
{
  if (mode == Mode::NORMAL)
  {
    if (button == button::OK)
    {
      gvm->closeOverview();
    }
    else if (button == button::DISMISS)
    {
      // TODO
    }
  }
  else if (mode == Mode::HERO_HIRE)
  {
    if (button == button::HERO_HIRE)
    {
      // TODO
    }
    else if (button == button::HERO_REJECT)
    {
      // TODO
    }
  }
}

void UnitDetailView::switchMode(Mode mode)
{
  this->mode = mode;
  
  // TODO: text alignment on buttons is shifted by a variable amount, try to find exact formula
  
  if (mode == Mode::NORMAL)
  {
    this->c = normalBaseCoords;
    buttons[HIGH_ACTION]->setLabel("Dismiss");
    buttons[LOW_ACTION]->setLabel("Ok");
  }
  else if (mode == Mode::HERO_HIRE)
  {
    this->c = heroHireCoords;
    buttons[HIGH_ACTION]->setLabel("Hire");
    buttons[LOW_ACTION]->setLabel("Reject");
  }
  else if (mode == Mode::VIEW_ONLY)
  {
    this->c = viewOnlyBaseCoord;
  }
  
  /* hide if mode is view only */
  buttons[HIGH_ACTION]->showIf(mode != Mode::VIEW_ONLY);
  buttons[LOW_ACTION]->showIf(mode != Mode::VIEW_ONLY);
  
  /* adjust button positions according to mode */
  buttons[HIGH_ACTION]->setPosition(c.x + 222, c.y + 143);
  buttons[LOW_ACTION]->setPosition(c.x + 222, c.y + 162);
  
  buttons[UP_ARROW]->setPosition(c.x + 205, c.y + 104);
  buttons[DOWN_ARROW]->setPosition(c.x + 205, c.y + 168);
  
  skillDraw.setPosition(c.x + 8, c.y + 108);
}

void UnitDetailView::draw()
{
  Gfx::draw(full_unit_backdrop, c.x, c.y);
  
  if (mode != Mode::VIEW_ONLY)
    Gfx::draw(buttons_backdrop, c.x + 213, c.y + 133);
  
  if (mode == Mode::HERO_HIRE)
  {
    Gfx::draw(hero_hire_banner, 0,0);
    Fonts::drawString(Fonts::format("Hero for Hire: %u gold", hireCost), fonts::base::SERIF_GOLD, 128, 5, ALIGN_CENTER);
  }
  
  //TODO: font has full teal stroke
  Fonts::drawString(i18n::s(I18::UI_UNIT_DETAIL_MOVES), FontFaces::Small::TEAL, c.x + 50, c.y +26, ALIGN_LEFT);
  Fonts::drawString(i18n::s(I18::UI_UNIT_DETAIL_UPKEEP), FontFaces::Small::TEAL, c. x +50, c.y + 33, ALIGN_LEFT);
  
  /* TODO: per ora non gestisce i modificatori dell'unità tipo xp o vari spell, va gestito con un metodo diverso visto che drawUnitProps usa solo la spec
   * e viene usato anche dal production view
   */
  CommonDraw::drawUnitPropsComplete(unit, c.x + 51, c.y + 47, 15);
  
  // draw isometric version of the unit, if it's an hero use its portrait
  if (unit->type() != Productable::Type::HERO)
    UnitDraw::drawUnitIso(unit->spec, c.x + 6, c.y + 5, unit, nullptr);
  else
  {
    Gfx::draw(hero_portrait_border, c.x + 8, c.y + 6);
    UnitDraw::drawHeroPortrait(static_cast<const Hero*>(unit), c.x + 9, c.y + 7);
  }
  
  // draw unit name
  Fonts::drawString(unit->name(), FontFaces::Serif::TEAL, c.x + 50, c.y + 10, ALIGN_LEFT);
  
  // draw skill pane, first check if buttons to change page are needed and display them
  buttons[UP_ARROW]->showIf(skillDraw.showTopArrow());
  buttons[DOWN_ARROW]->showIf(skillDraw.showBottomArrow());
  
  // then draw currently visible skills
  skillDraw.draw(unit);
  
  // draw upkeep cost of the unit and movement
  Upkeep uk = unit->upkeep();
  
  /* TODO: cambiare immagine in base al tipo di movimento */
  // TODO: should it be getProperty and not getBaseProperty?
  CommonDraw::drawMovement(unit->getBaseProperty(Property::MOVEMENT), unit->getEnumProperty<MovementBaseType>(Property::MOVEMENT_BASE_TYPE), c.x + 84, c.y + 25, 0);
  CommonDraw::drawUpkeep(uk, c.x + 85, c.y + 33);
  
  skillDraw.clickable()->draw();
}

void UnitDetailView::setHeroHire(Hero* hero, u32 cost)
{
  this->hireCost = cost;
  this->unit = hero;
  switchMode(Mode::HERO_HIRE);
  skillDraw.reset(unit);
}

void UnitDetailView::setUnit(const Unit *unit, bool withButtons)
{
  this->unit = unit;
  switchMode(withButtons ? Mode::NORMAL : Mode::VIEW_ONLY);
  skillDraw.reset(unit);
}

bool UnitDetailView::mousePressed(u16 x, u16 y, MouseButton b)
{
  SpriteInfo bg = full_unit_backdrop;
  
  bool isOutside = (x < c.x || x >= c.x + bg.sw()) || (y < c.y || y >= c.y + bg.sh());
  
  if (isOutside && (mode == Mode::NORMAL || mode == Mode::VIEW_ONLY))
    gvm->closeOverview();
  
  return true;
  
}
