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

static const Point gameOptionsButtonPositions[] = {
  { 251, 39 },
  { 251, 66 },
  { 251, 93 },
  { 251, 120 }
};

NewGameView::NewGameView(ViewManager * gvm) : View(gvm)
{

}

void NewGameView::activate()
{
  switchToPhase(Phase::WIZARD_CHOICE);
}

void NewGameView::switchToPhase(Phase phase)
{
  const auto* face = FontFaces::MediumBold::BROWN_START;
  
  buttons.clear();
  
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
      const u16 baseX[] = { 169, 245 };
      const u16 baseY = 27;
      const u16 baseButtonSprite = 9;
      const u16 deltaY = 22;
      const WizardID wizards[] = {
        WizardID::MERLIN, WizardID::RAVEN, WizardID::SHAREE, WizardID::LO_PAN, WizardID::JAFAR, WizardID::OBERIC, WizardID::RJAK,
        WizardID::SSS_RA, WizardID::TAURON, WizardID::FREYA, WizardID::HORUS, WizardID::ARIEL, WizardID::TLALOC, WizardID::KALI
      };
      
      for (size_t i = 0; i < 14; ++i)
      {
        const auto button = addButton(Button::buildOffsetted(Data::wizard(wizards[i]).name(), baseX[i/7], baseY + deltaY*(i%7), LSI(NEWGAME, baseButtonSprite+i)));
        button->setOnEnterAction([this,i,wizards]() { this->wizard = wizards[i]; });
        button->setOnExitAction([i](){ printf("Exiting %zu\n", i); });
      }
      
      break;
    }
  }
  
  this->phase = phase;
}

void NewGameView::draw()
{
  Gfx::draw(LSI(NEWGAME, 0), 0, 0);

  /*Gfx::drawGrayScale(GfxData::unitGfxSpec(UnitSpec::summonSpec(UnitID::GREAT_DRAKE)).still, 30, 30);
  Gfx::drawGlow(GfxData::unitGfxSpec(UnitSpec::summonSpec(UnitID::GREAT_DRAKE)).still, 30, 30, CHAOS);*/
    
  switch (phase)
  {
    case Phase::GAME_OPTIONS:
    {
      //TODO: draw buttons bg
      //const Color buttonBG = Color(12,12,12);
      Gfx::draw(LSI(NEWGAME, 1), 165, 0);
      //Gfx::fillRect(0, 0, 100, 20, buttonBG);
      
      break;
    }
      
    case Phase::WIZARD_CHOICE:
    {
      Fonts::drawString("Select Wizard", FontFaces::Huge::GOLD, 242, 0, ALIGN_CENTER);
      Gfx::draw(LSI(NEWGAME, 8), 165, 17);
      
      if (wizard.isPresent())
        Gfx::draw(GfxData::wizardGfxSpec(wizard).portraitLarge, 24, 10);

      break;
    }
  }
}
