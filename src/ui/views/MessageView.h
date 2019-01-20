//
//  MessageView.h
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _MESSAGE_VIEW_
#define _MESSAGE_VIEW_

#include "View.h"

class ViewManager;
namespace msgs { class Message; }

enum class MessageHandlingState;

class MessageView : public View
{
private:
  enum
  {
    NO,
    YES,
    
    BUTTON_COUNT
  };
  
  std::unique_ptr<const msgs::Message> message;
  
  void draw() override;
  void drawPost() override { }
  
  void discardMessage();
  void discardAllMessages();
  void handleMessage();
  
  MessageHandlingState state;
  
public:
  MessageView(ViewManager* gvm);
  
  bool mouseReleased(u16 x, u16 y, MouseButton b) override;
  
  void activate() override { handleMessage(); }
  void deactivate() override;
  
  /* since messages are handled in main view we need a way to force immediate handling for things like dialogs or help */
  void showMessage(const msgs::Message* message);
};

#endif
