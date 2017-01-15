//
//  LoadView.h
//  OpenMoM
//
//  Created by Jack on 1/15/17.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#pragma Once

#include "View.h"

class LoadView : public View
{
private:
  void draw() override;
  void drawPost() override { }
  
  enum bts
  {
    LOAD = 0,
    QUIT,
    SAVE,
    OK,
    
    COUNT
  };
  
public:
  LoadView(ViewManager* gvm);
  
  void mouseReleased(u16 x, u16 y, MouseButton b) override;
  
  void activate() override { }
  void deactivate() override { }
};
