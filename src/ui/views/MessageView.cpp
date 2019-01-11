//
//  MessageView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "MessageView.h"

#include "Font.h"
#include "Gfx.h"
#include "Texture.h"
#include "Messages.h"
#include "Buttons.h"
#include "ViewManager.h"
#include "Dialogs.h"

#include "LocalPlayer.h"

#include "CityView.h"
#include "CityScape.h"

#include "game/world/Places.h"

#include "GfxData.h"

MessageView::MessageView(ViewManager* gvm) : View(gvm), message(nullptr)
{
  buttons.resize(BUTTON_COUNT);
  
  buttons[NO] = Button::buildBistate("No", 0, 0, LSI(RESOURCE,4))->setAction([this]() { handleMessage(); });
  buttons[YES] = Button::buildBistate("Yes", 30, 0, LSI(RESOURCE,3));
}

void MessageView::handleMessage()
{
  if (player->hasMessage())
  {
    message = player->fetchMessage();
    
    for (auto b : buttons) b->hide();
    
    if (message->type == msgs::Message::Type::CONFIRM)
    {
      buttons[YES]->setAction([this](){
        message->as<msgs::Confirm>()->action();
        handleMessage();
      }); // FIXME: understand behavior and fix because now it's a stackoverflow

      buttons[NO]->show();
      buttons[YES]->show();
    }
  }
  else
    gvm->closeOverview();
}

void MessageView::discardMessage()
{
  message.reset(nullptr);
}

void MessageView::discardAllMessages()
{
  player->clearMessages();
}

bool MessageView::mouseReleased(u16 x, u16 y, MouseButton b)
{
  if (message->type == msgs::Message::Type::NEW_BUILDING)
  {
    discardAllMessages();
    
    /* TODO: così smista tutti i messaggi prima di switchare la vista (ad esempio due buildings completati), ci piace? */
    const msgs::NewBuilding* msg = message->as<const msgs::NewBuilding>();
    City* city = msg->city;
    gvm->closeOverview(); /* this will deallocate message */
    gvm->cityView()->setCity(city);
    gvm->switchView(VIEW_CITY);
  }
  else if (message->type == msgs::Message::Type::ERROR || message->type == msgs::Message::Type::HELP || message->type == msgs::Message::Type::MESSAGE)
  {
    handleMessage();
  }
  
  return true;
}

void MessageView::deactivate()
{
  message.reset(nullptr);
}

void MessageView::draw()
{
  if (!message)
    return;
  
  switch (message->type) {
    case msgs::Message::Type::NEW_BUILDING:
    {
      const msgs::NewBuilding* msg = message->as<const msgs::NewBuilding>();
      
      Gfx::draw(TSI(MESSAGE_LEFT, 0, 0), 7, 61);
      Gfx::draw(TSI(MESSAGE_RIGHT, 0, 0), 243, 61);
      Gfx::draw(LSI(BACKGRND, 10), 249, 69); // building background
      const auto& school = GfxData::schoolGfxSpec(msg->city->getOwner()->book()->predominantSchool());
      Gfx::draw(school.messageMascot.gfx, school.messageMascot.offset);
      CityLayout::drawBuildingCentered(msg->city, msg->building, 249, 106);
      
      Fonts::drawStringBounded(msg->getMessage(), FontFaces::Serif::YELLOW_SHADOW, 79, 68, 170, ALIGN_LEFT);
      break;
    }
      
    case msgs::Message::Type::HELP:
    {
      const msgs::Help* msg = message->as<const msgs::Help>();

      
      const auto* titleFace = FontFaces::Serif::BROWN_HELP;
      const auto* textFace = FontFaces::Small::BROWN_HELP;
      
      SpriteInfo upperBG = LSI(HELP, 0);
      SpriteInfo lowerBG = LSI(HELP, 1);
      
      const u16 px = WIDTH/2 - upperBG.sw()/2;
      const u16 bound = 180;
      
      int tth = 0;
      
      Gfx::resetBuffer();
      Gfx::bindBuffer();
      {
        int ty = 4;
        
        const auto* p = msg->data;
        
        while (p)
        {
          int titleX = 4, textX = titleX + 2;
          
          if (p->icon.isPresent())
          {
            Gfx::draw(p->icon, titleX, ty);
            titleX += p->icon->sw() + 3;
          }
          
          Fonts::drawString(p->title, titleFace, titleX, ty, ALIGN_LEFT);
          
          if (!p->icon.isPresent())
            ty += titleFace->ver + titleFace->sh();
          else
            ty += std::max((index_t)p->icon->sh() + 1, titleFace->ver + titleFace->sh());
          
          int h = Fonts::drawStringBounded(p->text, textFace, textX, ty, bound, ALIGN_LEFT) - ty;
          ty += h;
          
          tth = ty;
          
          p = p->next;
          ty += 3;
        }
        
      }
      
      Gfx::bindCanvas();
      
      int totalHeight = tth + 20;
      int finalY = HEIGHT/2 - totalHeight/2 - lowerBG.sh()/2;
      
      Gfx::drawClipped(LSI(HELP, 0), px, finalY, 0, 0, 217, totalHeight);
      Gfx::draw(LSI(HELP,1), px, finalY + totalHeight);
      Gfx::mergeBuffer(4, 4, px + 17, finalY + 25, 186, tth);

      break;

    }
      
    case msgs::Message::Type::CONFIRM:
    {
      const msgs::Confirm* msg = message->as<const msgs::Confirm>();

      /* TODO: gestire font silver ovvero gestire colori diversi all'interno della stringa con marcatori */
      
      // draw dialog text onto buffer
      Gfx::resetBuffer();
      Gfx::bindBuffer();
      int h = Fonts::drawStringBounded(msg->getMessage(), FontFaces::Serif::GOLD_SHADOW, 4, 5, 167, ALIGN_LEFT) + 3;
      int y = 200/2 - (h+29)/2;
      Gfx::bindCanvas();
      // draw backdrop of the dialog by using text height
      Gfx::drawClipped(LSI(RESOURCE,0), 68, y, 0, 0, 186, h);
      // draw bottom part of the dialog
      Gfx::draw(LSI(RESOURCE,1), 68, y+h);
      
      // enable buttons and position them
      buttons[NO]->setPosition(68 + 18, y+h+5);
      buttons[YES]->setPosition(68 + 18 + 83, y+h+5);
      
      // blit text on dialog from buffer
      
      Gfx::mergeBuffer(4, 4, 68+8, y+7, 186, h);
      
      break;
    }
      
    case msgs::Message::Type::LAIR_CONFIRMATION:
    {      
      const msgs::LairConfirmation* msg = message->as<msgs::LairConfirmation>();
      
      const auto& spec = GfxData::placeGfxSpec(msg->place->type);
      
      const s32 x = 68;

      const auto bg = LSI(BACKGRND, 25);
      const auto footer = msg->isConfirmation() ? LSI(BACKGRND, 26) : LSI(BACKGRND, 27);
      
      //TODO: max line width is just set empirically
      //TODO: all the magic numbers for adjustments on buffer are ugly
      
      Gfx::resetBuffer();
      Gfx::bindBuffer();
      Fonts::setFace(FontFaces::Serif::GOLD_SHADOW);
      Fonts::setVerSpace(FontFaces::Serif::GOLD_SHADOW->ver + 1);
      auto remainder = Fonts::drawStringBounded(msg->getMessage(), Point(5 + 49, 5), 118, 4, ALIGN_LEFT);
      s32 h = Fonts::drawStringBounded(remainder.first, 5, remainder.second, 158, ALIGN_LEFT) + 4;
      Gfx::bindCanvas();
      
      const s32 y = HEIGHT/2 - (h + footer.sh()) / 2;
      
      Gfx::drawClipped(bg, x, y, 0, 0, bg.sw(), h);
      Gfx::draw(spec.enterMessage.icon, x + 7, y + 7);
      Gfx::draw(footer, x, y + h);
      
      Gfx::mergeBuffer(6, 5, x + 9, y + 9, bg.sw(), h);
      
      if (msg->isConfirmation())
      {
        buttons[NO]->setPosition(x + 17, y + h + 5);
        buttons[YES]->setPosition(x + 100, y + h + 5);
        
        buttons[NO]->show();
        buttons[YES]->show();
      }
  
      break;
    }
    
    case msgs::Message::Type::ERROR:
    {
      const msgs::Error* msg = message->as<msgs::Error>();
      dialogs::drawErrorDialog(msg->getMessage());
      break;
    }

      
    default:
    {
      assert(false);
      /*Gfx::bindBuffer();
      int h = Fonts::drawStringBounded(message->getMessage(), FontFaces::Small::BROWN, 76, 40, 175, ALIGN_LEFT);
      Gfx::bindCanvas();
      Gfx::drawClipped(TSI(HELP_BACKDROP,0,0), 55, 3+h, 0, 200, 210, 23);
      Gfx::drawClipped(TSI(HELP_BACKDROP,0,0), 55, 10, 0, 0, 210, h);
      Gfx::mergeBuffer();*/
      
      break;
    }
  }
}
