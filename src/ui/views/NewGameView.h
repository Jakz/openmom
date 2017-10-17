#ifndef _NEW_GAME_VIEW_H_
#define _NEW_GAME_VIEW_H_

#include "View.h"

#include "Settings.h"

class ViewManager;

class NewGameView : public View
{
private:
  enum class Phase
  {
    GAME_OPTIONS = 0,
    WIZARD_CHOICE
  };
  
  enum
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
  const Wizard* wizard;
  
  school_value_map spellBooks;
  
  void switchToPhase(Phase phase);
  
  void draw() override;
  void drawPost() override { }
  
  void drawSpellBooks(const school_value_map& books, Point position);
  
public:
  NewGameView(ViewManager* gvm);
  
  void activate() override;
  void deactivate() override { }
};

#endif
