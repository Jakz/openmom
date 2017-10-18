#include "NewGameView.h"

#include "Data.h"
#include "Font.h"
#include "Buttons.h"
#include "Gfx.h"
#include "ViewManager.h"
#include "Localization.h"

#include "GfxData.h"
#include "UnitSpec.h"
#include "UnitDraw.h"
#include "CommonDraw.h"


bool Textfield::keyReleased(KeyboardCode key, KeyboardKey kkey, KeyboardMod mod)
{
  if (key == KeyboardCode::SDL_SCANCODE_ESCAPE)
    onCancel();
  else if (key == KeyboardCode::SDL_SCANCODE_KP_ENTER || key == KeyboardCode::SDL_SCANCODE_RETURN || key == KeyboardCode::SDL_SCANCODE_RETURN2)
    onComplete(text);
  else if (key == KeyboardCode::SDL_SCANCODE_BACKSPACE)
  {
    if (caret > 0)
    {
      text.erase(text.begin()+caret-1);
      --caret;
    }
  }
  else if (key == KeyboardCode::SDL_SCANCODE_DELETE)
  {
    if (caret < text.length())
    {
      text.erase(text.begin()+caret);
    }
  }
  else if (key == KeyboardCode::SDL_SCANCODE_LEFT)
  {
    if (caretEnabled && caret > 0)
      --caret;
  }
  else if (key == KeyboardCode::SDL_SCANCODE_RIGHT)
  {
    if (caretEnabled && caret < text.size())
      ++caret;
  }
  
  return true;
}

bool Textfield::textInput(sdl_text_input data)
{
  if (data[1] == '\0')
  {
    if (isalpha(data[0]))
    {
      text.insert(caret, &data[0]);
      ++caret;
    }
  }
  
  return true;
}

void Textfield::draw()
{
  Fonts::drawString(text, font, position.x, position.y, ALIGN_LEFT);
}




void PlayerNameField::draw()
{
  Textfield::draw();
  
  u16 x, caretWidth;
  u16 y = position.y + font->sh() - 1;
  
  if (caret == text.length())
  {
    x = Fonts::stringWidth(font, text);
    caretWidth = font->charWidth(' ');
  }
  else
  {
    x = Fonts::stringWidth(font, text.substr(0, caret));
    caretWidth = font->charWidth(text[caret]);
  }
  
  if (Gfx::fticks % 4 < 2)
    Gfx::drawLine(Color(158, 125, 101), position.x + x, y, position.x + x + caretWidth + 2, y);
  
  //if (Gfx::fticks % 4 < 2)
  //  Fonts::drawString("_", font, position.x + x, position.y, ALIGN_LEFT);
}



enum sprite_id : sprite_ref
{
  main_bg = LBXI(NEWGAME, 0),
  options_bg = LBXI(NEWGAME, 1),
  wizard_choice_bg = LBXI(NEWGAME, 8),
  portrait_choice_bg = LBXI(NEWGAME, 39),
  name_choice_bg = LBXI(NEWGAME, 40),
  spell_choice_bg = LBXI(NEWGAME, 41)
};

static const Point gameOptionsButtonPositions[] = {
  { 251, 39 },
  { 251, 66 },
  { 251, 93 },
  { 251, 120 }
};

NewGameView::NewGameView(ViewManager * gvm) : View(gvm), info({nullptr, "", school_value_map(0)})
{
  nameField.setFace(new fonts::MediumBoldFont({158, 125, 101}));
  nameField.setOnCancel([this](){ switchToPhase(isPremadeWizard ? Phase::WIZARD_CHOICE : Phase::PORTRIT_CHOICE); });
  nameField.setOnComplete([this](const std::string& name) {
    info.name = name;
    switchToPhase(isPremadeWizard ? Phase::RACE_CHOICE : Phase::BOOKS_CHOICE);
  });
  
  fonts.darkBoldFont = new fonts::MediumBoldFont({52,40,28}, {166,134,105}); // TODO: fix last color?
  fonts.brightBoldFont = new fonts::MediumBoldFont({166,134,105}, {52,40,28});
  fonts.darkSerifFont = new fonts::SerifFont({52,40,28}, {166, 134, 105});
}

void NewGameView::activate()
{
  info.portrait = Data::wizard("kali"); //TODO nullptr
  info.books = school_value_map({{CHAOS, 3}, {LIFE, 2}, {SORCERY, 4},  {ARCANE, 0}, {DEATH, 0}, {NATURE, 0}}); //TODO school_value_map(0);
  info.name = "Jack"; // "";
  isPremadeWizard = false; // TODO true
  
  
  switchToPhase(Phase::BOOKS_CHOICE);
  
  nameField.setPosition(Point(194,35));
  nameField.setText("Lo Pan");
  
  availablePicks = 11;
}

u32 NewGameView::countPicks()
{
  u32 picksFromRetort = std::accumulate(info.retorts.begin(), info.retorts.end(), 0u, [](u32 value, const Retort* retort) { return value + retort->cost; });
  u32 picksFromBooks = std::accumulate(info.books.begin(), info.books.end(), 0u, [](u32 value, const u32& entry) { return value + entry; });
  
  return picksFromRetort + picksFromBooks;
}

void NewGameView::switchToPhase(Phase phase)
{
  const auto* face = fonts.darkBoldFont;
  const u16 baseX[] = { 169, 245 };
  const u16 baseY = 27;
  const u16 baseButtonSprite = 9;
  const u16 deltaY = 22;
  
  buttons.clear();
  
  switch (phase)
  {
    case Phase::PORTRIT_CHOICE:
    case Phase::WIZARD_CHOICE:
    {
      isPremadeWizard = true;
      info.portrait = nullptr;
      info.books = school_value_map(0);
      break;
    }
    default: break;
  }
  
  switch (phase)
  {
    case Phase::GAME_OPTIONS:
    {
      static const char* names[] = { "difficulty", "opponents", "land size", "magic" };
      static const u16 spriteIndex[] = { 4, 5, 6, 7 };
      static const SettingGroupID groups[] = { SETTING_DIFFICULTY, SETTING_OPPONENTS, SETTING_LAND_SIZE, SETTING_MAGIC_POWER };
      
      for (size_t i = 0; i < 4; ++i)
      {
        const auto button = addButton(Button::buildOffsetted(names[i], gameOptionsButtonPositions[i].x, gameOptionsButtonPositions[i].y, LSI(NEWGAME, spriteIndex[i])));
        
        button->setAction([this, i, face, button]() {
          settings.group(groups[i]).next();
          button->setTextInfo(TextInfo(settings.group(groups[i]).currentValue(), face));

        });
        
        button->setTextInfo(TextInfo(settings.group(groups[i]).currentValue(), face));
      }
      
      addButton(Button::buildBistate("Cancel", 171, 179, LSI(NEWGAME, 3)))->setAction([this](){ gvm->switchView(VIEW_START); });
      addButton(Button::buildBistate("Ok", 252, 179, LSI(NEWGAME, 2)))->setAction([this](){ switchToPhase(Phase::WIZARD_CHOICE); });
      
      break;
    }
      
    case Phase::WIZARD_CHOICE:
    {
      const auto wizards = Data::values<const Wizard*>();
      
      //TODO: labels are not correctly centered on buttons, verify buildOffsetted
      
      auto it = wizards.begin;
      for (size_t i = 0; i < wizards.size; ++i, ++it)
      {
        const Wizard* wizard = it->second;
        const WizardGfxSpec& gfx = GfxData::wizardGfx(wizard);
        
        const auto button = addButton(Button::buildOffsetted(i18n::s(gfx.name), baseX[i/7], baseY + deltaY*(i%7), LSI(NEWGAME, baseButtonSprite+i)));
        button->setTextInfo(TextInfo(i18n::s(gfx.name), face));
        button->setOnEnterAction([this,wizard,wizards]() {
          info.portrait = wizard;
          info.books = wizard->defaultBooks;
          info.retorts = wizard->defaultRetorts;
          info.name = i18n::s(GfxData::wizardGfx(wizard).name);
        });
      }

      const auto customButton = addButton(Button::buildOffsetted("custom", baseX[1], baseY+deltaY*7, LSI(NEWGAME, 23)));
      customButton->setTextInfo(TextInfo("Custom", face));
      customButton->setAction([this]() { switchToPhase(Phase::PORTRIT_CHOICE); });
      customButton->setOnEnterAction([this]() {
        info.portrait = nullptr;
        info.books = school_value_map(0);
        info.retorts.clear();
        info.name = "";
      });
      
      break;
    }
      
    case Phase::PORTRIT_CHOICE:
    {
      const auto wizards = Data::values<const Wizard*>();
      
      auto it = wizards.begin;
      for (size_t i = 0; i < wizards.size; ++i, ++it)
      {
        const Wizard* wizard = it->second;
        const WizardGfxSpec& gfx = GfxData::wizardGfx(wizard);
        
        const auto button = addButton(Button::buildOffsetted(i18n::s(gfx.name), baseX[i/7], baseY + deltaY*(i%7), LSI(NEWGAME, baseButtonSprite+i)));
        button->setTextInfo(TextInfo(i18n::s(gfx.name), face));
        button->setOnEnterAction([this,wizard,wizards]() { info.portrait = wizard; });
      }
      
      break;
    }
      
    case Phase::NAME_CHOICE:
    {
      
      break;
    }
      
    case Phase::BOOKS_CHOICE:
    {
      //TODO: background
      const auto button = addButton(Button::buildOffsetted("spell_choice_ok", 252, 182, LSI(NEWGAME, 42), LSI(NEWGAME, 43)));
      button->deactivate();
      
      break;
    }
  }
  
  this->phase = phase;
}

void NewGameView::draw()
{
  Gfx::draw(main_bg, 0, 0);

  /*Gfx::drawGrayScale(GfxData::unitGfxSpec(UnitSpec::summonSpec(UnitID::GREAT_DRAKE)).still, 30, 30);
  Gfx::drawGlow(GfxData::unitGfxSpec(UnitSpec::summonSpec(UnitID::GREAT_DRAKE)).still, 30, 30, CHAOS);*/
    
  switch (phase)
  {
    case Phase::GAME_OPTIONS:
    {
      Gfx::draw(main_bg, 0, 0);
      //TODO: draw buttons bg
      //const Color buttonBG = Color(12,12,12);
      Gfx::draw(options_bg, 165, 0);
      //Gfx::fillRect(0, 0, 100, 20, buttonBG);
      
      break;
    }
      
    case Phase::WIZARD_CHOICE:
    {
      Gfx::draw(main_bg, 0, 0);
      Fonts::drawString("Select Wizard", FontFaces::Huge::GOLD, 242, 0, ALIGN_CENTER);
      Gfx::draw(wizard_choice_bg, 165, 17);
      break;
    }
      
    case Phase::PORTRIT_CHOICE:
    {
      Gfx::draw(main_bg, 0, 0);
      Fonts::drawString("Select Picture", FontFaces::Huge::GOLD, 242, 0, ALIGN_CENTER);
      Gfx::draw(portrait_choice_bg, 165, 17);
      break;
    }
      
    case Phase::NAME_CHOICE:
    {
      Gfx::draw(main_bg, 0, 0);
      Fonts::drawString("Wizard's Name", FontFaces::Huge::GOLD, 242, 0, ALIGN_CENTER);
      Gfx::draw(name_choice_bg, 165+16, 17);
      nameField.draw();
      break;
    }
      
    case Phase::BOOKS_CHOICE:
    {
      Gfx::draw(spell_choice_bg, 0, 0);
      Fonts::drawString(std::to_string(availablePicks - countPicks())+ " picks", fonts.brightBoldFont, 221, 184, ALIGN_CENTER);
      
      for (size_t i = 0; i < SCHOOL_NO_ARCANE_COUNT; ++i)
      {
        School school = CommonDraw::schools[i];
        CommonDraw::drawSpellBooks(school, info.books[school], Point(197, 49 + 26*i));
      }

      break;
    }
  }
  
  if (info.portrait)
  {
    Gfx::draw(GfxData::wizardGfx(info.portrait).portraitLarge, 24, 10);
    Fonts::drawString(info.name, fonts.darkSerifFont, 76, 118, ALIGN_CENTER);
  }
  
  CommonDraw::drawSpellBooks(info.books, Point(36, 135), false);
}

bool NewGameView::keyReleased(KeyboardCode key, KeyboardKey kkey, KeyboardMod mod)
{
  if (phase == Phase::NAME_CHOICE)
    nameField.keyReleased(key, kkey, mod);
  
  if (key == KeyboardCode::SDL_SCANCODE_ESCAPE)
  {
    if (phase == Phase::PORTRIT_CHOICE)
    {
      switchToPhase(Phase::WIZARD_CHOICE);
    }

    return true;
  }
  
  return false;
}

bool NewGameView::textInput(sdl_text_input data)
{
  if (phase == Phase::NAME_CHOICE)
  {
    nameField.textInput(data);
    return true;
  }
  
  return false;
}
