#include "NewGameView.h"

#include "Font.h"
#include "Buttons.h"
#include "Gfx.h"

#include "GfxData.h"
#include "UnitSpec.h"
#include "UnitDraw.h"

NewGameView::NewGameView(ViewManager * gvm) : View(gvm)
{
  buttons.resize(BUTTON_COUNT);
  
  buttons[OPTIONS_OK] = Button::buildBistate("Ok", 252, 179, LSI(NEWGAME, 2));
  buttons[OPTIONS_CANCEL] = Button::buildBistate("Cancel", 171, 179, LSI(NEWGAME, 3));
  
  const auto* face = FontFaces::MediumBold::BROWN_START;
  
  buttons[OPTIONS_DIFFICULTY] = Button::buildOffsetted("Difficulty", 251, 39, LSI(NEWGAME,4));
  buttons[OPTIONS_OPPONENTS] = Button::buildOffsetted("Opponents", 251, 66, LSI(NEWGAME,5));
  buttons[OPTIONS_LAND_SIZE] = Button::buildOffsetted("Land Size", 251, 93, LSI(NEWGAME,6));
  buttons[OPTIONS_MAGIC] = Button::buildOffsetted("Magic", 251, 120, LSI(NEWGAME,7));
  
  buttons[OPTIONS_DIFFICULTY]->setTextInfo(TextInfo("", face));
  buttons[OPTIONS_OPPONENTS]->setTextInfo(TextInfo("", face));
  buttons[OPTIONS_LAND_SIZE]->setTextInfo(TextInfo("", face));
  buttons[OPTIONS_MAGIC]->setTextInfo(TextInfo("", face));
  
  buttons[OPTIONS_DIFFICULTY]->setAction([&](){ settings.group(SETTING_DIFFICULTY).next(); updateLabelsOptions(); });
  buttons[OPTIONS_OPPONENTS]->setAction([&](){ settings.group(SETTING_OPPONENTS).next(); updateLabelsOptions(); });
  buttons[OPTIONS_LAND_SIZE]->setAction([&](){ settings.group(SETTING_LAND_SIZE).next(); updateLabelsOptions(); });
  buttons[OPTIONS_MAGIC]->setAction([&](){ settings.group(SETTING_MAGIC_POWER).next(); updateLabelsOptions(); });
  
  updateLabelsOptions();
}

void NewGameView::updateLabelsOptions()
{
  buttons[OPTIONS_DIFFICULTY]->setLabel(settings.group(SETTING_DIFFICULTY).currentValue());
  buttons[OPTIONS_LAND_SIZE]->setLabel(settings.group(SETTING_LAND_SIZE).currentValue());
  buttons[OPTIONS_OPPONENTS]->setLabel(settings.group(SETTING_OPPONENTS).currentValue());
  buttons[OPTIONS_MAGIC]->setLabel(settings.group(SETTING_MAGIC_POWER).currentValue());
}

void NewGameView::draw()
{
  Gfx::draw(LSI(NEWGAME, 0), 0, 0);

  /*Gfx::drawGrayScale(GfxData::unitGfxSpec(UnitSpec::summonSpec(UnitID::GREAT_DRAKE)).still, 30, 30);
  Gfx::drawGlow(GfxData::unitGfxSpec(UnitSpec::summonSpec(UnitID::GREAT_DRAKE)).still, 30, 30, CHAOS);*/
    
  switch (phase)
  {
    case GAME_OPTIONS:
    {
      Gfx::draw(LSI(NEWGAME, 1), 165, 0);

      
    }
  }
}
