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
  

  
    void mousePressed(u16 x, u16 y, MouseButton b) override {}
    void mouseClicked(u16 x, u16 y, MouseButton b) override {}
    void mouseReleased(u16 x, u16 y, MouseButton b) override {}
    void mouseMoved(u16 x, u16 y, MouseButton b) override {}
    void mouseDragged(u16 x, u16 y, MouseButton b) override {}
      
    void keyPressed(KeyboardKey key, KeyboardMod mod) override {}
    void keyReleased(KeyboardKey key, KeyboardMod mod) override {}
};

#endif
