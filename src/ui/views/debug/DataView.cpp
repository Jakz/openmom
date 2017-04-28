//
//  ResearchView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "DataView.h"

#include "LBX.h"
#include "Gfx.h"
#include "Font.h"
#include "ViewManager.h"

#include "UnitSpec.h"

#include "Data.h"

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
    return Data::unit(s1)->productionName() < Data::unit(s2)->productionName();
  });
}

void DataView::switchMode(Mode mode)
{
  this->mode = mode;
  
}

void DataView::deactivate()
{
  
}

void DataView::draw()
{
  u16 y = 10;
  auto it = keys.begin();
  
  const auto* face = FontFaces::Small::WHITE;
  
  while (y < HEIGHT && it != keys.end())
  {
    const UnitSpec* unit = Data::unit(*it);
    
    Fonts::drawString(unit->productionName(), face, 5, y, ALIGN_LEFT);
    y += 10;
    ++it;
  }
  
  
  
}
