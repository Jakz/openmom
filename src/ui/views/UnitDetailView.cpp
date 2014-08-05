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

UnitDetailView::UnitDetailView(ViewManager* gvm) : View(gvm), unit(nullptr)
{
  buttons.resize(BUTTON_COUNT);
  
  buttons[DISMISS] = TristateButton::build("Dismiss", 253, 149, TextureID::UNIT_DETAIL_BUTTONS, 0); // TODO: action
  buttons[OK] = TristateButton::build("Ok", 253, 168, TextureID::UNIT_DETAIL_BUTTONS, 1)->setAction([gvm](){gvm->closeOverview();});
  
  buttons[UP_ARROW] = TristateButton::build("Up", 235, 110, TextureID::UNIT_DETAIL_SKILL_ARROWS, 0)->setAction([](){SkillDraw::i.prevPage();});
  buttons[DOWN_ARROW] = TristateButton::build("Down", 235, 174, TextureID::UNIT_DETAIL_SKILL_ARROWS, 1)->setAction([](){SkillDraw::i.nextPage();});
  
  /*for (int i = 0; i < 8; ++i)
   {
   Clickable area = SkillDraw.clickableForIndex(i);
   areas.add(area);
   }
   
   areas.get(0).setAction(new Action() { public void execute() { SkillDraw.openHelpForSkill(unit, 0); } });
   areas.get(1).setAction(new Action() { public void execute() { SkillDraw.openHelpForSkill(unit, 1); } });
   areas.get(2).setAction(new Action() { public void execute() { SkillDraw.openHelpForSkill(unit, 2); } });
   areas.get(3).setAction(new Action() { public void execute() { SkillDraw.openHelpForSkill(unit, 3); } });
   areas.get(4).setAction(new Action() { public void execute() { SkillDraw.openHelpForSkill(unit, 4); } });
   areas.get(5).setAction(new Action() { public void execute() { SkillDraw.openHelpForSkill(unit, 5); } });
   areas.get(6).setAction(new Action() { public void execute() { SkillDraw.openHelpForSkill(unit, 6); } });
   areas.get(7).setAction(new Action() { public void execute() { SkillDraw.openHelpForSkill(unit, 7); } });*/
}

void UnitDetailView::draw()
{
  Gfx::draw(TextureID::UNIT_DETAIL_BACKDROP, 31, 6);
  Gfx::draw(TextureID::UNIT_DETAIL_BACKDROP_BUTTONS, 244, 139);
  
  Fonts::drawString("Moves", FontFaces::Small::TEAL, 81, 32, ALIGN_LEFT);
  Fonts::drawString("Upkeep", FontFaces::Small::TEAL, 81, 39, ALIGN_LEFT);
  
  /* TODO: per ora non gestisce i modificatori dell'unità tipo xp o vari spell, va gestito con un metodo diverso visto che drawUnitProps usa solo la spec
   * e viene usato anche dal production view
   */
  CommonDraw::drawUnitPropsComplete(unit, 82, 53, 15);
  
  // draw isometric version of the unit, if it's an hero use its portrait
  if (unit->type() != Productable::Type::HERO)
    UnitDraw::drawUnitIso(&unit->spec, 36, 11, unit);
  else
  {
    Gfx::draw(TextureID::UNIT_DETAIL_HERO_BACKDROP, 0, 0, 39, 12);
    UnitDraw::drawHeroPortrait(static_cast<Hero*>(unit), 40, 13);
  }
  
  // draw unit name
  Fonts::drawString(unit->name(), FontFaces::Serif::TEAL, 81, 16, ALIGN_LEFT);
  
  // draw skill pane, first check if buttons to change page are needed and display them
  buttons[UP_ARROW]->showIf(SkillDraw::i.showTopArrow());
  buttons[DOWN_ARROW]->showIf(SkillDraw::i.showBottomArrow());
  
  // then draw currently visible skills
  SkillDraw::i.draw(unit);
  
  // draw upkeep cost of the unit and movement
  Upkeep uk = unit->upkeep();
  
  /* TODO: cambiare immagine in base al tipo di movimento */
  // TODO: should it be getProperty and not getBaseProperty?
  CommonDraw::drawMovement(unit->getBaseProperty(Property::MOVEMENT), 118, 32, 0);
  CommonDraw::drawUpkeep(uk, 117, 39);
}

void UnitDetailView::setUnit(Unit *unit)
{
  this->unit = unit;
  SkillDraw::i.reset(unit);
}