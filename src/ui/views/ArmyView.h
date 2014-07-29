//
//  ArmyView.h
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _ARMY_VIEW_H_
#define _ARMY_VIEW_H_

#include "View.h"

class ViewManager;
class Army;

class ArmyView : public View
{
private:
  enum Button
  {
    
    BUTTON_COUNT
  };
  
  bool acceptSpellTarget;
  Army* army;
  
  void draw() override;
  void drawPost() override { }
  
public:
  ArmyView(ViewManager* gvm);
  
  void activate() override { }
  void deactivate() override { acceptSpellTarget = false; }
  
  void mouseReleased(u16 x, u16 y, MouseButton b) override;
  
  void setArmy(Army* army) { this->army = army; }
  void setAcceptSpellTarget() { acceptSpellTarget = true; }
};

#endif