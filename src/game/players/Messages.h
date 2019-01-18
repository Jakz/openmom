//
//  Messages.h
//  OpenMoM
//
//  Created by Jack on 7/23/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _MESSAGES_H_
#define _MESSAGES_H_

#include <string>
#include <functional>
#include <list>

#include "Font.h"
#include "Localization.h"
#include "Help.h"

class Skill;
class City;
class Place;
class Building;

namespace msgs {
  
  typedef std::function<void()> Action;
  
  class Message
  {
  protected:
    std::string message;
    
    void setMessage(std::string&& message) { this->message = message; }
    
  public:
    enum class Type
    {
      MESSAGE,
      ERROR,
      CONFIRM,
      NEW_BUILDING,
      LAIR_CONFIRMATION,
      HELP,
      ITEM_DETAIL
    } type;
    
    const std::string& getMessage() const { return message; }
    
    Message(std::string message) : message(message), type(Type::MESSAGE) { }
    
    template<typename T> const T* as() const { return static_cast<const T*>(this); }
    
  protected:
    Message(Type type) : type(type) { }
    Message(Type type, std::string message) : type(type), message(message) { }
  };
  
  class Error : public Message
  {
  public:
    Error(const std::string& message) : Message(Type::ERROR, message) { }
    Error(std::string&& message) : Message(Type::ERROR, message) { }
  };
  
  class Confirm : public Message
  {
  public:
    Confirm(std::string message, Action&& action) : Message(Type::CONFIRM, message), action(action) { }
    const Action action;
  };
  
  class NewBuilding : public Message
  {
  public:
    NewBuilding(City* city, const Building* building);
    
    const Building* const building;
    City* const city;
  };
  
  class Help : public Message
  {
  public:
    const help::Paragraph* const data;
    
    Help(const help::Paragraph* data) : Message(Type::HELP, ""), data(data) { }
  };
  
  class LairConfirmation : public Message
  {
    const Unit* predominantUnit();
  
  public:
    LairConfirmation(const Place* place);
    bool isConfirmation() const { return true; }
    
    const Place* const place;
  };
  
  class ItemDetail : public Message
  {    
  public:
    const items::Item* const item;
    //TODO: pass position for dialog placement
    ItemDetail(const items::Item* item) : Message(Type::ITEM_DETAIL, ""), item(item) { }
  };
  
}

#endif
