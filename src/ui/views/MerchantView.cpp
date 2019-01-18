//
//  ResearchView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "MerchantView.h"

#include "Gfx.h"
#include "Texture.h"
#include "Buttons.h"
#include "Font.h"

#include "BookView.h"
#include "Spells.h"
#include "LocalPlayer.h"
#include "Game.h"
#include "World.h"

#include "Army.h"

#include "GfxData.h"
#include "Localization.h"

#include "ViewManager.h"

enum lbx_idents : sprite_ref
{
  background = LBXI(HIRE, 2),
  button = LBXI(HIRE, 1),
  item_box = LBXI(ITEMISC, 25),
  list_pin = LBXI(ITEMISC, 26)
};

MerchantView::MerchantView(ViewManager* gvm) : ViewWithQueue(gvm)
{
  buttons.resize(2);
  
  //TODO: font color is slightly wrong
  buttons[BUY] = Button::buildBistate("buy", 257, 136, button, "Buy", fonts::base::SERIF_GOLD);
  buttons[REJECT] = Button::buildBistate("reject", 257, 155, button, "Reject", fonts::base::SERIF_GOLD);

  itemDetails.setPosition({19, 80});

  openWithOffer(nullptr, 200);
}

void MerchantView::activate()
{

}

void MerchantView::draw()
{
  Gfx::draw(background, 5, 15);
  
  if (mode == Mode::ITEM)
  {
    Gfx::draw(item_box, 19, 80);
    
    //TODO: hardcoded for now
    const char* header = "A merchant arrives and offers a magic Shield of Demos for sale. The price is only 200 gold pieces.";
    Fonts::drawStringBounded(header, fonts::base::SERIF_GOLD, 61, 23, 185, ALIGN_LEFT);
    
    items::Item item = items::Item(items::TypeID::SHIELD, 0);
    
    itemDetails.draw(&item);
  }
}

void MerchantView::openWithOffer(const items::Item* item, u16 price)
{
  mode = Mode::ITEM;

  
  //TODO: reenable gvm->switchOverview(VIEW_MERCHANT);
}


bool MerchantView::mouseReleased(u16 x, u16 y, MouseButton b)
{

  return true;
}
