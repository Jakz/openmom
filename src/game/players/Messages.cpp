//
//  Messages.cpp
//  OpenMoM
//
//  Created by Jack on 7/23/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "Messages.h"

#include "City.h"
#include "Buildings.h"

//using namespace messages;

msgs::NewBuilding::NewBuilding(City* city, const Building* building) : Message(
Fonts::format(i18n::s(I18::MESSAGE_NEW_BUILDING).c_str(), i18n::s(i18n::CITY_SIZE_NAMES[city->tileSize()]).c_str(), city->getName().c_str(), i18n::s(building->name).c_str()), NEW_BUILDING),
city(city), building(building) { }
