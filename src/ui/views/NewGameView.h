#ifndef _NEW_GAME_VIEW_H_
#define _NEW_GAME_VIEW_H_

#include "View.h"

#include "Settings.h"

class ViewManager;

class Textfield : public KeyEventListener
{
private:
  std::string text;
  Point position;
  const FontSpriteSheet* font;
  
  std::function<void(const std::string&)> onComplete;
  std::function<void(void)> onCancel;

public:
  Textfield() : text(""), font(nullptr), position(Point::ZERO) { }
  
  void setFace(const FontSpriteSheet* font) { this->font = font; }
  void setPosition(const Point& position) { this->position = position; }
  void setText(const std::string& text) { this->text = text; }
  
  bool keyReleased(KeyboardCode key, KeyboardKey kkey, KeyboardMod mod) override;
  bool textInput(sdl_text_input data) override;
  void draw();
};

class NewGameView : public View
{
private:
  enum class Phase
  {
    GAME_OPTIONS = 0,
    WIZARD_CHOICE,
    PORTRIT_CHOICE,
    NAME_CHOICE
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
  std::string name;
  school_value_map spellBooks;


  Textfield nameField;
  bool isPremadeWizard;
  
  
  void switchToPhase(Phase phase);
  
  void draw() override;
  void drawPost() override { }
  
  void drawSpellBooks(const school_value_map& books, Point position);
  
public:
  NewGameView(ViewManager* gvm);
  
  void activate() override;
  void deactivate() override { }
  
  bool keyReleased(KeyboardCode key, KeyboardKey kkey, KeyboardMod mod) override;
  bool textInput(sdl_text_input data) override;
};

#endif
