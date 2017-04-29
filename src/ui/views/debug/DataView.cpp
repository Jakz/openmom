//
//  ResearchView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "DataView.h"

#include "Localization.h"
#include "LBX.h"
#include "Gfx.h"
#include "Font.h"
#include "ViewManager.h"

#include "UnitSpec.h"

#include "GfxData.h"
#include "Data.h"

constexpr int BASE = 0;
constexpr int DELTA = 10;
constexpr int ROW_COUNT = HEIGHT / DELTA - 2;
constexpr int MARGIN = 120;

DataView::DataView(ViewManager* gvm) : View(gvm)
{

}

void DataView::activate()
{
  switchMode(Mode::UNITS);
  const auto& units = Data::units();
  
  keys.clear();
  std::transform(units.begin(), units.end(), std::back_inserter(keys), [](const std::remove_reference<decltype(units)>::type::value_type& entry) {
    return entry.first;
  });
  
  std::sort(keys.begin(), keys.end(), [](const std::string& s1, const std::string& s2) {
    return Data::unit(s1)->fullName() < Data::unit(s2)->fullName();
  });
  
  for (int i = 0; i < ROW_COUNT; ++i)
    addArea(new Clickable(0, BASE + i*DELTA, MARGIN, DELTA))->setOnEnterAction([this, i](){
      entries.clear();
      hover(i);
    });
}

void DataView::switchMode(Mode mode)
{
  this->mode = mode;
  page = 0;
}

void DataView::hover(int index)
{
  if (index + page*ROW_COUNT < keys.size())
  {
    const UnitSpec* spec = Data::unit(keys[index + page*ROW_COUNT]);
    const UnitGfxSpec& gfx = GfxData::unitGfx(spec);

    entries.add(new BasicSpriteEntry(gfx.fullFigure.relative(5), Point(MARGIN+5,5)));
    entries.add(new BasicSpriteEntry(gfx.still, Point(MARGIN+5+gfx.fullFigure.sw(),5)));
  }
}

void DataView::deactivate()
{
  
}

void DataView::draw()
{
  u16 y = 0;
  auto it = keys.begin();
  std::advance(it, page*ROW_COUNT);
  
  const auto* face = FontFaces::Small::WHITE;
  
  while (y < ROW_COUNT && it != keys.end())
  {
    const UnitSpec* unit = Data::unit(*it);
  
    if (unit->type == UnitType::FANTASTIC)
      face = FontFaces::Small::YELLOWW;
    else if (unit->type == UnitType::RACIAL)
      face = FontFaces::Small::WHITE;
    else if (unit->type == UnitType::HERO)
      face = FontFaces::Small::GREENW;
    
    Fonts::drawString(unit->fullName(), face, 5, BASE + y*DELTA + 2, ALIGN_LEFT);
    
    ++y;
    ++it;
  }
  
  entries.draw();
  
}
