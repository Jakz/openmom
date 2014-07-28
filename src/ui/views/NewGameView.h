#ifndef _NEW_GAME_VIEW_H_
#define _NEW_GAME_VIEW_H_

#include "View.h"

#include "Settings.h"

class ViewManager;

class NewGameView : public View
{
  private:
    enum Phase
    {
      GAME_OPTIONS = 0,
      WIZARD_CHOICE
    };
  
    enum Button
    {
      OPTIONS_OK,
      OPTIONS_CANCEL,
      OPTIONS_DIFFICULTY,
      OPTIONS_OPPONENTS,
      OPTIONS_LAND_SIZE,
      OPTIONS_MAGIC,
      
      BUTTON_COUNT
    };
  
    Phase phase;
    Settings settings;
  
    void draw() override;
    void drawPost() override { }
  
  public:
    NewGameView(ViewManager* gvm);
  
    void updateLabelsOptions();
  
    void activate() override { updateLabelsOptions(); }
    void deactivate() override { }
};

#endif
