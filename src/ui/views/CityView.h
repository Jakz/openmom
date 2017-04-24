//
//  CityView.h
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _CITY_VIEW_H_
#define _CITY_VIEW_H_

#include "View.h"


class City;
class CityLayout;
class ViewManager;
enum class UpkeepSymbol;

class CityView : public View
{
private:
  enum button
  {
    BUY,
    CHANGE,
    OK,
    
    NEXT_CITY_ENCHANT,
    PREV_CITY_ENCHANT,
    
    BUTTON_COUNT
  };
  
  u16 cityEnchantPage;
  
  City* city;
  CityLayout *cityScape;
  
  void draw() override;
  void drawPost() override { }
  
  s16 drawCityProp(s16 value, UpkeepSymbol type, s16 sx, s16 sy);
  
  void clickOnCitySpell(size_t index);
  
public:
  CityView(ViewManager* gvm);
  
  void activate() override { }
  void deactivate() override { }
  
  void setCity(City* city);
  
  friend class ProductionView;
};

#endif
