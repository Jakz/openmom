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
  enum
  {
    
    BUTTON_COUNT
  };
  
  std::unique_ptr<const Palette> propPalette;
  
  bool acceptSpellTarget;
  Army* army;
  
  void draw() override;
  void drawPost() override { }
  
  void clickOnIth(size_t i);
  
public:
  ArmyView(ViewManager* gvm);
  
  void activate() override;
  void deactivate() override { acceptSpellTarget = false; }
  
  bool mouseReleased(u16 x, u16 y, MouseButton b) override;
  
  void open(Army* army, bool acceptSpellTarget = false);
  void setArmy(Army* army);
  void setAcceptSpellTarget(bool acceptSpellTarget = true) { acceptSpellTarget = true; }
};

#endif
