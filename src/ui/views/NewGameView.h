#ifndef _NEW_GAME_VIEW_H_
#define _NEW_GAME_VIEW_H_

#include "View.h"

#include "Settings.h"

class ViewManager;

class Textfield : public KeyEventListener
{
protected:
  size_t caret;
  std::string text;
  Point position;
  const FontSpriteSheet* font;
  
  std::function<void(const std::string&)> onComplete;
  std::function<void(void)> onCancel;
  
  bool caretEnabled;

public:
  Textfield() : caret(0), text(""), font(nullptr), position(Point::ZERO), caretEnabled(true) { }
  
  void setFace(const FontSpriteSheet* font) { this->font = font; }
  void setPosition(const Point& position) { this->position = position; }
  void setText(const std::string& text) { this->text = text; this->caret = text.size(); }
  void setCaretEnabled(bool enabled) { this->caretEnabled = enabled; }
  
  void setOnCancel(std::function<void(void)> onCancel) { this->onCancel = onCancel; }
  void setOnComplete(std::function<void(const std::string&)> onComplete) { this->onComplete = onComplete; }
  
  bool keyReleased(KeyboardCode key, KeyboardKey kkey, KeyboardMod mod) override;
  bool textInput(sdl_text_input data) override;
  virtual void draw();
};

class PlayerNameField : public Textfield
{
public:
  void draw() override;
};

class NewGameView : public ViewWithQueue
{
private:
  enum class Phase
  {
    GAME_OPTIONS = 0,
    WIZARD_CHOICE,
    PORTRIT_CHOICE,
    NAME_CHOICE,
    BOOKS_CHOICE,
    SPELLS_CHOICE,
    RACE_CHOICE,
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
  u32 availablePicks;
  
  struct
  {
    std::array<optional<SpellRarity>, 2> shownRarities;
    School currentSchool;
    spell_enum_map<s32> spellChoicePicks;
    spell_enum_map<s32> spellChoiceTotals;
  } spellChoiceData;
  
  Settings settings;
  
  PlayerSetupInfo info;
  bool isPremadeWizard;
  
  PlayerNameField nameField;

  void switchToPhase(Phase phase);
  
  void draw() override;
  void drawPost() override { }
  
  void drawSpellBooks(const school_value_map& books, Point position);
  void drawRetortList();
  void errorMessage(const std::string& message);
  
  u32 freePicks() const { return availablePicks - countPicks(); }
  u32 countPicks() const;
  
  const Spell* spellForSlot(SpellRarity rarity, School school, s32 slot);
  
  Button* bookPhaseOkButton;
  
  void booksPicked(School school, u16 amount);
  void retortToggled(const Retort* retort);
  void spellToggled(const Spell* spell);
  
  struct
  {    
    school_map<FontPalette> schoolFonts;
    
    const FontSpriteSheet* darkBoldFont;
    const FontSpriteSheet* brightBoldFont;
    
    const FontSpriteSheet* darkSerifFont;
    
    const FontSpriteSheet *tinyBright, *tinyGold, *tinyInactive, *tinyRetortList;
    
  } fonts;
  
public:
  NewGameView(ViewManager* gvm);
  
  void activate() override;
  void deactivate() override { }
  
  bool keyReleased(KeyboardCode key, KeyboardKey kkey, KeyboardMod mod) override;
  bool textInput(sdl_text_input data) override;

  bool mouseReleased(u16 x, u16 y, MouseButton b) override;
  bool mouseDragged(u16 x, u16 y, MouseButton b) override;

};

#endif
