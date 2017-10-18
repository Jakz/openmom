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
  else if (key == KeyboardCode::SDL_SCANCODE_BACKSPACE)
  {
    if (!text.empty())
      text.pop_back();
  }
  
  return true;
}

bool Textfield::textInput(sdl_text_input data)
{
  if (data[1] == '\0')
  {
    if (isalpha(data[0]))
      text += data[0];
  }
  
  return true;
}

void Textfield::draw()
{
  u16 x = Fonts::drawString(text, font, position.x, position.y, ALIGN_LEFT);
  
  if (Gfx::fticks % 4 < 2)
    Fonts::drawString("_", font, position.x + x, position.y, ALIGN_LEFT);
}



enum sprite_id : sprite_ref
{
  main_bg = LBXI(NEWGAME, 0),
  options_bg = LBXI(NEWGAME, 1),
  wizard_choice_bg = LBXI(NEWGAME, 8),
  portrait_choice_bg = LBXI(NEWGAME, 39),
  name_choice_bg = LBXI(NEWGAME, 40)
};

static const Point gameOptionsButtonPositions[] = {
  { 251, 39 },
  { 251, 66 },
  { 251, 93 },
  { 251, 120 }
};

NewGameView::NewGameView(ViewManager * gvm) : View(gvm), wizard(nullptr), spellBooks(0)
{
  nameField.setFace(new fonts::MediumBoldFont({158, 125, 101}));
}

void NewGameView::activate()
{
  wizard = nullptr;
  spellBooks = school_value_map(0);
  isPremadeWizard = true;
  
  switchToPhase(Phase::NAME_CHOICE);
  
  nameField.setPosition(Point(194,35));
  nameField.setText("Lo Pan");
}

void NewGameView::switchToPhase(Phase phase)
{
  const auto* face = FontFaces::MediumBold::BROWN_START;
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
      wizard = nullptr;
      spellBooks = school_value_map(0);
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
        button->setOnEnterAction([this,wizard,wizards]() { this->wizard = wizard; });
      }

      const auto customButton = addButton(Button::buildOffsetted("custom", baseX[1], baseY+deltaY*7, LSI(NEWGAME, 23)));
      customButton->setTextInfo(TextInfo("Custom", face));
      customButton->setAction([this]() { switchToPhase(Phase::PORTRIT_CHOICE); });
      customButton->setOnEnterAction([this]() { this->wizard = nullptr; });
      
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
        button->setOnEnterAction([this,wizard,wizards]() { this->wizard = wizard; });
      }
      
      break;
    }
      
    case Phase::NAME_CHOICE:
    {
      
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
      //TODO: draw buttons bg
      //const Color buttonBG = Color(12,12,12);
      Gfx::draw(options_bg, 165, 0);
      //Gfx::fillRect(0, 0, 100, 20, buttonBG);
      
      break;
    }
      
    case Phase::WIZARD_CHOICE:
    {
      Fonts::drawString("Select Wizard", FontFaces::Huge::GOLD, 242, 0, ALIGN_CENTER);
      Gfx::draw(wizard_choice_bg, 165, 17);
      break;
    }
      
    case Phase::PORTRIT_CHOICE:
    {
      Fonts::drawString("Select Picture", FontFaces::Huge::GOLD, 242, 0, ALIGN_CENTER);
      Gfx::draw(portrait_choice_bg, 165, 17);
      break;
    }
      
    case Phase::NAME_CHOICE:
    {
      Fonts::drawString("Wizard's Name", FontFaces::Huge::GOLD, 242, 0, ALIGN_CENTER);
      Gfx::draw(name_choice_bg, 165+16, 17);
      nameField.draw();
      break;
    }
  }
  
  if (wizard)
  {
    Gfx::draw(GfxData::wizardGfx(wizard).portraitLarge, 24, 10);
    Fonts::drawString(i18n::s(GfxData::wizardGfx(wizard).name), FontFaces::Serif::BROWN_START, 76, 118, ALIGN_CENTER);
    
    if (isPremadeWizard)
      CommonDraw::drawSpellBooks(wizard->defaultBooks, Point(36, 135), false);
  }
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
