//
//  MessageView.cpp
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "MessageView.h"

#include "Font.h"
#include "gfx/Gfx.h"
#include "gfx/Texture.h"
#include "Messages.h"
#include "Buttons.h"
#include "ViewManager.h"

#include "LocalPlayer.h"

#include "CityView.h"
#include "CityScape.h"

MessageView::MessageView(ViewManager* gvm) : View(gvm), message(nullptr)
{
  buttons.resize(BUTTON_COUNT);
  
  buttons[NO] = TristateButton::build("No", 0, 0, TextureID::MESSAGE_CONFIRM_BUTTONS, 0)->setAction([this]() { discardMessage(); });
  buttons[YES] = TristateButton::build("Yes", 30, 0, TextureID::MESSAGE_CONFIRM_BUTTONS, 1);
}

void MessageView::handleMessage()
{
  if (player->hasMessage())
  {
    message = player->firstMessage();
    
    for (auto b : buttons) b->hide();
    
    if (message->type == msgs::Message::Type::CONFIRM)
    {
      buttons[YES]->setAction([this](){ (buttons[YES]->getAction())(); discardMessage(); }); // FIXME: understand behavior and fix because now it's a stackoverflow

      buttons[NO]->show();
      buttons[YES]->show();
    }
  }
  else
    gvm->closeOverview();
}

void MessageView::discardMessage()
{
  player->clearFirstMessage();
  handleMessage();
}

void MessageView::discardAllMessages()
{
  player->clearMessages();
}

void MessageView::mouseReleased(u16 x, u16 y, MouseButton b)
{
  if (message->type == msgs::Message::Type::NEW_BUILDING)
  {
    discardAllMessages();
    
    /* TODO: così smista tutti i messaggi prima di switchare la vista (ad esempio due buildings completati), ci piace? */
    const msgs::NewBuilding* msg = static_cast<const msgs::NewBuilding*>(message);
    gvm->closeOverview();
    gvm->cityView()->setCity(msg->city);
    gvm->switchView(VIEW_CITY);
  }
  else if (message->type == msgs::Message::Type::ERROR || message->type == msgs::Message::Type::HELP_SKILL)
  {
    discardMessage();
  }
}

void MessageView::draw()
{
  switch (message->type) {
    case msgs::Message::Type::NEW_BUILDING:
    {
      const msgs::NewBuilding* msg = static_cast<const msgs::NewBuilding*>(message);
      
      Gfx::draw(TextureID::MESSAGE_LEFT, 7, 61);
      Gfx::draw(TextureID::MESSAGE_RIGHT, 243, 61);
      Gfx::draw(TextureID::MESSAGE_BUILDING_BACKDROP, 249, 69);
      /* TODO: choose accordingly to major school of magic of the wizard and check if they are centered */
      Gfx::draw(TextureID::MAGIC_SCHOOL_ICONS, 0, 1, 6, 40);
      CityLayout::drawBuildingCentered(msg->building, 249, 106);
      
      Fonts::drawStringBounded(msg->getMessage(), FontFaces::Serif::YELLOW_SHADOW, 79, 68, 170, ALIGN_LEFT);
      break;
    }
      
    case msgs::Message::Type::HELP_SKILL:
    {
      Gfx::resetBuffer();
      Gfx::bindBuffer();
      int h = Fonts::drawStringBounded(message->getMessage(), FontFaces::Small::BROWN, 76, 40, 175, ALIGN_LEFT);
      Gfx::bindCanvas();
      Gfx::drawClipped(TextureID::HELP_BACKDROP, 55, 10, 0, 0, 210, h);
      Gfx::drawClipped(TextureID::HELP_BACKDROP, 55, 3+h, 0, 200, 210, 23);
      Gfx::mergeBuffer();
      break;

    }
      
    case msgs::Message::Type::CONFIRM:
    {
      const msgs::Confirm* msg = static_cast<const msgs::Confirm*>(message);

      /* TODO: gestire font silver ovvero gestire colori diversi all'interno della stringa con marcatori */
      
      // draw dialog text onto buffer
      Gfx::resetBuffer();
      Gfx::bindBuffer();
      int h = Fonts::drawStringBounded(msg->getMessage(), FontFaces::Serif::GOLD_SHADOW, 4, 5, 167, ALIGN_LEFT) + 3;
      int y = 200/2 - (h+29)/2;
      Gfx::bindCanvas();
      // draw backdrop of the dialog by using text height
      Gfx::drawClipped(TextureID::MESSAGE_CONFIRM_DIALOG, 68, y, 0, 0, 186, h);
      // draw bottom part of the dialog
      Gfx::drawClipped(TextureID::MESSAGE_CONFIRM_DIALOG, 68, y+h, 0, 113, 186, 29);
      
      // enable buttons and position them
      buttons[NO]->setPosition(68 + 18, y+h+5);
      buttons[YES]->setPosition(68 + 18 + 83, y+h+5);
      
      // blit text on dialog from buffer
      
      Gfx::mergeBuffer(4, 4, 68+8, y+7, 186, h);
      
      break;
    }
    
    case msgs::Message::Type::ERROR:
    {
      const msgs::Error* msg = static_cast<const msgs::Error*>(message);
      
      Gfx::resetBuffer();
      Gfx::bindBuffer();
      int h = Fonts::drawStringBounded(msg->getMessage(), FontFaces::Serif::GOLD_SHADOW, 4, 5, 150, ALIGN_CENTER) + 3; // TODO: check align with new font management
      int y = 200/2 - (h+29)/2;
      Gfx::bindCanvas();
      Gfx::drawClipped(TextureID::MESSAGE_ERROR_BACKDROP, 68, y, 0, 0, 186, h);
      Gfx::drawClipped(TextureID::MESSAGE_ERROR_BACKDROP, 68, y+h, 0, 113, 186, 9);
      Gfx::mergeBuffer(4,4,68+12,y+7,186,h+10);
      
      break;
    }
      
    default:
    {
      Gfx::bindBuffer();
      int h = Fonts::drawStringBounded(message->getMessage(), FontFaces::Small::BROWN, 76, 40, 175, ALIGN_LEFT);
      Gfx::bindCanvas();
      Gfx::drawClipped(TextureID::HELP_BACKDROP, 55, 10, 0, 0, 210, h);
      Gfx::drawClipped(TextureID::HELP_BACKDROP, 55, 3+h, 0, 200, 210, 23);
      Gfx::mergeBuffer();
      
      break;
    }
  }
}
