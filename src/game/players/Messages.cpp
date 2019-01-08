//
//  Messages.cpp
//  OpenMoM
//
//  Created by Jack on 7/23/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "Messages.h"

#include "common/format.h"
#include "i18n/Localization.h"

#include "City.h"
#include "Buildings.h"

//using namespace messages;

msgs::NewBuilding::NewBuilding(City* city, const Building* building) :
Message(Type::NEW_BUILDING, fmt::sprintf(i18n::s(I18::MESSAGE_NEW_BUILDING), i18n::s(i18n::CITY_SIZE_NAMES[city->tileSize()]), city->getName(), i18n::s(building->name))),
city(city), building(building) { }


#include "game/units/Unit.h"
#include "game/units/Army.h"
#include "game/world/Places.h"

#include "ui/GfxData.h"

const Unit* msgs::LairConfirmation::predominantUnit()
{
  //TODO: finish
  return new FantasticUnit(Data::unit("funit_hell_hounds")->as<SummonSpec>());
}

msgs::LairConfirmation::LairConfirmation(const Place* place) : Message(Type::LAIR_CONFIRMATION), place(place)
{
  const PlaceGfxSpec& spec = GfxData::placeGfxSpec(place->type);
  
  setMessage(fmt::sprintf("You have found %s.  Scouts have spotted %s within %s.  Do you wish to enter?",
                          i18n::s(spec.enterMessage.firstName),
                          predominantUnit()->spec->productionName(),
                          i18n::s(spec.enterMessage.lastName)
                          )
             );
}
