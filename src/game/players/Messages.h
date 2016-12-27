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
class Building;

namespace msgs {
  
  typedef std::function<void()> Action;
  
  class Message
  {
  protected:
    std::string message;
    
  public:
    enum Type
    {
      MESSAGE,
      ERROR,
      CONFIRM,
      NEW_BUILDING,
      HELP
    } type;
    
    const std::string& getMessage() const { return message; }
    
    Message(std::string message) : message(message), type(MESSAGE) { }
    
    template<typename T> const T* as() const { return static_cast<const T*>(this); }
    
  protected:
    Message(std::string message, Type type) : message(message), type(type) { }
  };
  
  class Error : public Message
  {
  public:
    Error(std::string& message) : Message(message, ERROR) { }
    Error(std::string&& message) : Message(message, ERROR) { }
  };
  
  class Confirm : public Message
  {
  public:
    Confirm(std::string message, Action&& action) : Message(message, CONFIRM), action(action) { }
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
    
    Help(const help::Paragraph* data) : Message("", HELP), data(data) { }
  };
  
}

#endif
