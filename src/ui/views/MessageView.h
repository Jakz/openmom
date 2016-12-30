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
  
  //void discardMessage();
  void discardAllMessages();
  void handleMessage();
  
public:
  MessageView(ViewManager* gvm);
  
  void mouseReleased(u16 x, u16 y, MouseButton b) override;
  
  void activate() override { handleMessage(); }
  void deactivate() override;
};

#endif
