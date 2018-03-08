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

#include "format.h"

#include "Messages.h"
#include "LocalPlayer.h" //TODO: required for error message, maybe unneeded


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
  spell_choice_bg = LBXI(NEWGAME, 41),
  
  checkmark = LBXI(NEWGAME, 52)
};

static const Point gameOptionsButtonPositions[] = {
  { 251, 39 },
  { 251, 66 },
  { 251, 93 },
  { 251, 120 }
};

NewGameView::NewGameView(ViewManager * gvm) : ViewWithQueue(gvm), info({nullptr, "", school_value_map(0)}), fonts{school_map<const FontSpriteSheet*>(nullptr)}
{
  nameField.setFace(new fonts::MediumBoldFont({158, 125, 101}));
  nameField.setOnCancel([this](){ switchToPhase(isPremadeWizard ? Phase::WIZARD_CHOICE : Phase::PORTRIT_CHOICE); });
  nameField.setOnComplete([this](const std::string& name) {
    info.name = name;
    switchToPhase(isPremadeWizard ? Phase::RACE_CHOICE : Phase::BOOKS_CHOICE);
  });
  
  fonts.darkBoldFont = new fonts::MediumBoldFont({52,40,28}, {166,134,105}); // TODO: fix last color?
  fonts.brightBoldFont = new fonts::MediumBoldFont({166,134,105}, {52,40,28});
  fonts.darkSerifFont = fonts::SerifFont::of({52,40,28}, {166, 134, 105});
  
  fonts.tinyBright = fonts::TinyFont::of({190, 154, 117}, {113, 85, 69}, {56, 31, 27});
  fonts.tinyInactive = fonts::TinyFont::of({97, 73, 60}, {113, 85, 69}, {56, 31, 27});
  fonts.tinyGold = fonts::TinyFont::of({239, 166, 35}, {97, 73, 60}, {56, 31, 27});
  fonts.tinyRetortList = fonts::TinyFont::of({239, 166, 35}, {142, 97, 36}, {52, 40, 28});
  
}

void NewGameView::activate()
{
  info.portrait = Data::wizard("kali"); //TODO nullptr
  info.books = school_value_map({{CHAOS, 3}, {LIFE, 4}, {SORCERY, 0},  {ARCANE, 0}, {DEATH, 0}, {NATURE, 0}}); //TODO school_value_map(0);
  info.name = "Jack"; // "";
  info.retorts.insert(Data::retort("archmage")); // ...
  info.retorts.insert(Data::retort("conjurer"));
  info.retorts.insert(Data::retort("node_mastery"));
  info.retorts.insert(Data::retort("charismatic"));
  isPremadeWizard = false; // TODO true
  
  
  switchToPhase(Phase::BOOKS_CHOICE);
  
  nameField.setPosition(Point(194,35));
  nameField.setText("Lo Pan");
  
  availablePicks = 11;
}

void NewGameView::errorMessage(const std::string& message)
{
  player->send(new msgs::Error(message));
}

void NewGameView::drawRetortList()
{
  if (!info.retorts.empty())
  {
    size_t i = 0;
    std::string ss;
  
    /* we must use original list because this is the order shown in game */
    // TODO: order now always correspond to the one of original game
    const auto& retorts = Data::values<const Retort*>();
    for (auto it = retorts.obegin(); it != retorts.oend(); ++it)
    {
      const Retort* retort = *it;
      
      if (info.retorts.find(retort) != info.retorts.end())
      {
        if (i == 0)
          ss = i18n::s(retort->i18n) + '.';
        else if (i == 1)
          ss = i18n::s(retort->i18n) + " and " + ss;
        else
          ss = i18n::s(retort->i18n) + ", " + ss;
        
        ++i;
      }
    }
  
    // TODO: ugly hack to increase vertical spacing between lines, we should
    // understand if this font is used with multiple spacings or this is the most
    // used one
    Fonts::setFace(fonts.tinyRetortList, fonts.tinyRetortList->ver+1, fonts.tinyRetortList->hor);
    Fonts::drawStringBounded(ss, 12, 180, 145, ALIGN_LEFT);
  }
}

u32 NewGameView::countPicks() const
{
  u32 picksFromRetort = std::accumulate(info.retorts.begin(), info.retorts.end(), 0u, [](u32 value, const Retort* retort) { return value + retort->cost; });
  u32 picksFromBooks = std::accumulate(info.books.begin(), info.books.end(), 0u, [](u32 value, const u32& entry) { return value + entry; });
  
  return picksFromRetort + picksFromBooks;
}

void NewGameView::booksPicked(School school, u16 amount)
{
  int dx = amount - info.books[school];
  
  
  /* we're removing books, alwasys do it then check for broken retorts */
  if (dx < 0)
  {
    info.books.set(school, amount);
    //TODO: check for broken retorts and remove them + error message
  }
  else if (dx > 0)
  {
    int freePicks = availablePicks - countPicks();
    /* if we're allowed to add books, if all picks are used then message */
    if (freePicks > 0)
    {
      s16 maxIncrement = std::min(freePicks, dx);
      info.books.set(school, info.books[school] + maxIncrement);
    }
    else
    {
      errorMessage("You have already made all your picks");
    }
  }
  
  bookPhaseOkButton->activateIf(freePicks() == 0);
}

void NewGameView::retortToggled(const Retort* retort)
{
  //TODO: localize (this will be a problem for plurals )
  static const std::string no_picks_available = "You have already made all your picks";
  static const std::string no_enough_picks_available = "You don't have enough picks left to make this selection.\nYou need %d picks";
  static const std::string no_enough_books_school_prefix = "To select %s you need:   ";
  static const std::string no_enough_books_any_school = "%d pick%s in any %sRealm%s of Magic";
  static const std::string no_enough_books_specific_school = "%d pick%s in %s Magic";

  
  if (info.retorts.find(retort) != info.retorts.end())
    info.retorts.erase(retort);
  else if (retort->canBePicked(freePicks(), info.books))
    info.retorts.insert(retort);
  else if (freePicks() == 0)
    errorMessage(no_picks_available);
  else
  {
    /* generate error message for requirements */
    if (freePicks() < retort->cost)
      errorMessage(fmt::sprintf(no_enough_picks_available, retort->cost));
    else if (!retort->requirements.empty())
    {
      const Retort::Requirement& req = retort->requirements[0];
      std::string prefix = fmt::sprintf(no_enough_books_school_prefix, i18n::c(retort->i18n));
      
      if (req.type == Retort::Requirement::Type::ANY_SCHOOL_AT_LEAST)
      {
        errorMessage(
          prefix +
          fmt::sprintf(no_enough_books_any_school,
            req.count,
            req.count > 1 ? "s" : "",
            req.times > 1 ? std::to_string(req.times) + " " : "",
            req.times> 1 ? "s" : "")
        );
        
        assert(retort->requirements.size() == 1);
      }
      else if (req.type == Retort::Requirement::Type::SPECIFIC_SCHOOL_AT_LEAST)
      {
        std::string suffix = "";
        for (const auto& r : retort->requirements)
        {
          assert(r.type == Retort::Requirement::Type::SPECIFIC_SCHOOL_AT_LEAST);
          if (!suffix.empty()) suffix += ", ";
          suffix += fmt::sprintf(no_enough_books_specific_school, r.count, r.count > 1 ? "s" : "", "Chaos"); // TODO: use localized school name
        }
        
        errorMessage(prefix + suffix);
      }
    }
  }
  
  bookPhaseOkButton->activateIf(freePicks() == 0);
}

void NewGameView::switchToPhase(Phase phase)
{
  const auto* face = fonts.darkBoldFont;
  const u16 baseX[] = { 169, 245 };
  const u16 baseY = 27;
  const u16 baseButtonSprite = 9;
  const u16 deltaY = 22;
  
  buttons.clear();
  bookPhaseOkButton = nullptr;
  
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
      const auto& wizards = Data::values<const Wizard*>();
      
      //TODO: labels are not correctly centered on buttons, verify buildOffsetted
      
      auto it = wizards.obegin();
      for (size_t i = 0; i < wizards.size(); ++i, ++it)
      {
        const Wizard* wizard = *it;
        const WizardGfxSpec& gfx = GfxData::wizardGfx(wizard);
        
        const auto button = addButton(Button::buildOffsetted(i18n::s(gfx.name), baseX[i/7], baseY + deltaY*(i%7), LSI(NEWGAME, baseButtonSprite+i)));
        button->setTextInfo(TextInfo(i18n::s(gfx.name), face));
        button->setOnEnterAction([this,wizard]() {
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
      const auto& wizards = Data::values<const Wizard*>();
      
      auto it = wizards.obegin();
      for (size_t i = 0; i < wizards.size(); ++i, ++it)
      {
        const Wizard* wizard = *it;
        const WizardGfxSpec& gfx = GfxData::wizardGfx(wizard);
        
        const auto button = addButton(Button::buildOffsetted(i18n::s(gfx.name), baseX[i/7], baseY + deltaY*(i%7), LSI(NEWGAME, baseButtonSprite+i)));
        button->setTextInfo(TextInfo(i18n::s(gfx.name), face));
        button->setOnEnterAction([this,wizard]() { info.portrait = wizard; });
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
      bookPhaseOkButton = addButton(Button::buildOffsetted("books_choice_ok", 252, 182, LSI(NEWGAME, 42), LSI(NEWGAME, 43)));
      bookPhaseOkButton->deactivate();
      
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
      
      /* draw spellbooks */
      for (size_t i = 0; i < SCHOOL_NO_ARCANE_COUNT; ++i)
      {
        School school = CommonDraw::schools[i];
        CommonDraw::drawSpellBooks(school, info.books[school], Point(197, 49 + 26*i));
      }
      
      /* draw retort list */
      {
        const static u16 X[] = { 170, 209, 259 };
        const static u16 Y = 4;
        const static u16 H = 6, RH = 7;
        
        const auto& retorts = Data::values<const Retort*>();
        auto it = retorts.obegin();
        for (u16 i = 0; i < retorts.size(); ++i, ++it)
        {
          const Retort* retort = *it;
          
          bool canBePicked = retort->canBePicked(availablePicks - countPicks(), info.books);
          const FontSpriteSheet* font = canBePicked ? fonts.tinyBright : fonts.tinyInactive;
          
          Point pos = Point(X[i / H], Y + (i % H) * RH);
          
          if (info.retorts.find(retort) != info.retorts.end())
          {
            Gfx::draw(checkmark, pos.delta(-5, 2));
            font = fonts.tinyGold;
          }
          
          Fonts::drawString(i18n::s(retort->i18n), font, pos.x, pos.y, ALIGN_LEFT);
        }
      }

      
      

      break;
    }
  }
  
  if (info.portrait)
  {
    Gfx::draw(GfxData::wizardGfx(info.portrait).portraitLarge, 24, 10);
    Fonts::drawString(info.name, fonts.darkSerifFont, 76, 118, ALIGN_CENTER);
  }
  
  drawRetortList();
  
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

bool NewGameView::mouseReleased(u16 x, u16 y, MouseButton b)
{
  if (phase == Phase::BOOKS_CHOICE)
  {
    /* books selection grid */
    {
      constexpr u16 BX = 189, BY = 44;
      constexpr u16 BW = 8, BH = 26, VH = 22;
      constexpr u16 MX = 13, MY = 5;
      
      int rx = (x - BX) / BW;
      int ry = (y - BY) / BH;
      
      if (rx >= 0 && rx <= MX && ry >= 0 && ry < MY)
      {
        if (((y - BY) % BH) > (BH - VH))
        {
          School school = CommonDraw::schools[ry];
          booksPicked(school, rx);
        }
      }
    }
    
    /* retort selection grid */
    {
      const static u16 X[] = { 170, 209, 259 };
      const static u16 Y = 4;
      const static u16 H = 6, RH = 7;
      
      if (x >= X[0] - 5 && y >= Y && y < Y + H*RH)
      {
        int v = (y - Y) / RH;
        int r = x < 209 ? 0 : (x < 259 ? 1 : 2);
        
        int index = v + r*H;
        
        const Retort* retort = Data::values<const Retort*>()[index];
        retortToggled(retort);
      }
    }
    
    
  }
  
  
  return true;
}
