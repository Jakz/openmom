//
//  UnitDraw.cpp
//  OpenMoM
//
//  Created by Jack on 7/26/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "UnitDraw.h"

#include "Gfx.h"
#include "GfxData.h"
#include "ColorMap.h"
#include "Texture.h"
#include "Army.h"
#include "Unit.h"
#include "Player.h"

#include "LocalPlayer.h"
#include "Game.h"

void UnitDraw::drawStatic(const Army *army, s16 x, s16 y)
{
  if ((army != LocalGame::i->currentPlayer()->getSelectedArmy()) || (Gfx::fticks % 6) < 3)
  {
    Gfx::draw(GfxData::playerGfxSpec(army->getOwner()->color).unitBack, x, y);
    
    const Unit* first = nullptr;
    if (army == LocalGame::i->currentPlayer()->getSelectedArmy())
    {
      for (auto u : *army)
        if (LocalGame::i->currentPlayer()->isSelectedUnit(u))
        {
          first = u;
          break;
        }
    }
    else
      first = army->get(0);
    
    Gfx::bindColorMap(&MiscMaps::FLAG_COLORS_MAP[army->getOwner()->color]);
    
    const SpriteInfo& info = GfxData::unitGfxSpec(&first->spec).still;
    
    if (!army->isPatrolling())
      Gfx::draw(info, x+1, y+1);
    else
      Gfx::drawGrayScale(info, x+1, y+1);
    
    School school = first->glow();
    if (school != NO_SCHOOL)
      Gfx::drawGlow(info, x+1, y+1, school);
    
    Gfx::unbindColorMap();
  }
}

void UnitDraw::drawStatic(const Unit *unit, s16 x, s16 y, bool backdrop, bool grayScale)
{
  if (backdrop)
    Gfx::draw(GfxData::playerGfxSpec(unit->getArmy()->getOwner()->color).unitBack, x, y);
  
  Gfx::bindColorMap(&MiscMaps::FLAG_COLORS_MAP[unit->getArmy()->getOwner()->color]);
  
  const SpriteInfo& info = GfxData::unitGfxSpec(&unit->spec).still;
  
  if (grayScale)
    Gfx::drawGrayScale(info, x+1, y+1);
  else
    Gfx::draw(info, x+1, y+1);
  
  School school = unit->glow();
  if (school != NO_SCHOOL)
    Gfx::drawGlow(info, x+1, y+1, school);
  
  Gfx::unbindColorMap();
}

void UnitDraw::rawDrawStatic(const Army *army, s16 x, s16 y)
{
  // TODO: verify
  Gfx::rawDraw(GfxData::playerGfxSpec(army->getOwner()->color).unitBack, x, y);
  const Unit* first = army->get(0);
  
  Gfx::bindColorMap(&MiscMaps::FLAG_COLORS_MAP[army->getOwner()->color]);

  const SpriteInfo& info = GfxData::unitGfxSpec(&first->spec).still;
  Gfx::draw(info, x+1, y+1);
  
  School school = first->glow();
  if (school != NO_SCHOOL)
    Gfx::drawGlow(info, x+1, y+1, school);
  
  Gfx::unbindColorMap();
}

void UnitDraw::drawHeroPortrait(const Hero *unit, s16 x, s16 y)
{
  Gfx::draw(GfxData::heroGfxSpec(&unit->spec), x, y);
}


struct IsoOffset { s8 x, y; };

static const IsoOffset ISO_1FIGURES[] = {{0,0}};
static const IsoOffset ISO_4FIGURES[] = {{+2,-4},{+11,+1},{-8,+1},{+2,+6}};
static const IsoOffset ISO_6FIGURES[] = {{+1,-5},{+4,-1},{+10,+2},{-10,0},{-3,+3},{+1,+7}};
static const IsoOffset ISO_8FIGURES[] = {{+1,-5},{+7,-2},{+11,0},{-2,-1},{+4,+1},{-10,0},{-4,+3},{+2,+6}};

void UnitDraw::drawUnitIso(const UnitSpec *unit, s16 x, s16 y, const Unit *realUnit)
{
  Gfx::drawClipped(TSI(UNIT_DETAIL_SPECIAL_THINGS,0,0), x, y, 0, 0, 40, 33);
  x += 6;
  
  const IsoOffset* o = nullptr;
  
  switch (unit->figures)
  {
    case 1:
      o = ISO_1FIGURES;
      break;
    case 4:
      o = ISO_4FIGURES;
      break;
    case 6:
      o = ISO_6FIGURES;
      break;
    case 8:
      o = ISO_8FIGURES;
      break;
    default:
      break;
  }
  
  // TODO: bind color map flags missing, probably sohuld be passed by production view since it could come from a city (so no realUnit)

  School glow = NO_SCHOOL;
  
  if (realUnit)
    glow = realUnit->glow();
  
  for (int i = 0; i < unit->figures; ++i)
  {    
    Gfx::draw(GfxData::unitGfxSpec(unit).fullFigure.frame(2, 2), x+o[i].x, y+o[i].y);
    
    //FIXME: commented to test lbx
    /*if (glow != NO_SCHOOL)
      Gfx::drawGlow(GfxData::unitGfxSpec(unit).fullFigure, x+o[i].x, y+o[i].y, 2, 2, glow);*
     */
  }
}

static const IsoOffset ISOC_1FIGURES[] = {{1,2}};
static const IsoOffset ISOC_4FIGURES[] = {{2,-4},{11,1},{-8,1,},{2,6}};
static const IsoOffset ISOC_6FIGURES[] = {{1,-5},{4,-1},{10,2},{-10,0},{-3,3},{1,7}};
static const IsoOffset ISOC_8FIGURES[] = {{1,-5},{7,-2},{11,0},{-2,-1},{4,1},{-10,0},{-4,3},{2,6}};

void UnitDraw::drawUnitIsoCombat(const Unit *unit, s16 x, s16 y, Dir facing, CombatAction caction)
{
  int action = 1;
  
  if (caction == CombatAction::MOVE || (caction == CombatAction::STAY && GfxData::unitGfxSpec(&unit->spec).isFlyingFigure))
  {
    if (Gfx::fticks%6 < 2)
      action = 0;
    else if (Gfx::fticks%6 < 4)
      action = 1;
    else
      action = 2;
  }
  else if (caction == CombatAction::ATTACK)
  {
    if (Gfx::fticks%2 == 0)
      action = 1;
    else if (Gfx::fticks%2 == 1)
      action = 3;
  }
  
  Gfx::bindColorMap(&MiscMaps::FLAG_COLORS_MAP[unit->getArmy()->getOwner()->color]);

  SpriteInfo sprite = GfxData::unitGfxSpec(&unit->spec).fullFigure.relative(static_cast<u8>(facing));
  
  School glow = unit->glow();
  
  const IsoOffset* offsets = nullptr;
  
  switch (unit->spec.figures) {
    case 1: offsets = ISOC_1FIGURES; break;
    case 4: offsets = ISOC_4FIGURES; break;
    case 6: offsets = ISOC_6FIGURES; break;
    case 8: offsets = ISOC_8FIGURES; break;
  }
  
  for (int i = 0; i < unit->getProperty(Property::ALIVE_FIGURES); ++i)
  {
    Gfx::draw(sprite.frame(action), x + offsets[i].x, y + offsets[i].y);
    //if (glow != School::NO_SCHOOL)
    //  Gfx::drawGlow(sprite.relative(action), x + offsets[i].x, y + offsets[i].y, glow); // TODO: check if it works with new management
  }

  Gfx::unbindColorMap();
}
