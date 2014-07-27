#include "MainView.h"

#include "Buttons.h"
#include "Gfx.h"
#include "Viewport.h"

#include "Game.h"
#include "LocalPlayer.h"
#include "Army.h"



MainView::MainView() : View()
{
  buttons.resize(BUTTON_COUNT);
  
  buttons[GAME] = TristateButton::build("Game", 7, 4, TextureID::UPPER_MENU, 0);
  buttons[SPELLS] = TristateButton::build("Spells", 47, 4, TextureID::UPPER_MENU, 1);
  buttons[ARMIES] = TristateButton::build("Armies", 89, 4, TextureID::UPPER_MENU, 2);
  buttons[CITIES] = TristateButton::build("Cities", 140, 4, TextureID::UPPER_MENU, 3);
  buttons[MAGIC] = TristateButton::build("Magic", 184, 4, TextureID::UPPER_MENU, 4);
  buttons[INFO] = TristateButton::build("Info", 226, 4, TextureID::UPPER_MENU, 5);
  buttons[PLANE] = TristateButton::build("Plane", 270, 4, TextureID::UPPER_MENU, 6);
  
  buttons[NEXT] = TristateButton::build("Next", 240, 173, TextureID::MAIN_LOW_BUTTONS, 0);
  buttons[CANCEL] = TristateButton::build("Cancel", 240, 173, TextureID::MAIN_LOW_BUTTONS, 1);

  buttons[DONE] = TristateButton::build("Done", 246, 176, TextureID::MAIN_SMALL_BUTTONS, 0);
  buttons[PATROL] = TristateButton::build("Patrol", 280, 176, TextureID::MAIN_SMALL_BUTTONS, 1);
  buttons[WAIT] = TristateButton::build("Wait", 246, 186, TextureID::MAIN_SMALL_BUTTONS, 2);
  buttons[BUILD] = TristateButton::build("Build", 280, 186, TextureID::MAIN_SMALL_BUTTONS, 3);
  
  for (auto e : {DONE,PATROL,WAIT,BUILD} )
    buttons[e]->hide();
  
  buttons[BUILD]->deactivate();
}

void MainView::switchToSpellCast(LocalPlayer *p)
{
	p->resetArmy();
	
	buttons[NEXT]->hide();
	buttons[DONE]->hide();
	buttons[PATROL]->hide();
	buttons[WAIT]->hide();
	buttons[BUILD]->hide();
	buttons[CANCEL_SURVEYOR]->show();
	
	substate = SPELL_CAST;
}

void MainView::switchToUnitSelection(LocalPlayer *p, Army* a)
{
	p->selectArmy(a);
	a->unpatrol();
	p->selectAll();
	
	buttons[CANCEL_SURVEYOR]->hide();
	buttons[NEXT]->hide();
	buttons[DONE]->show();
	buttons[PATROL]->show();
	buttons[WAIT]->show();
	buttons[BUILD]->show();
	
	updateBuildButton(p);
  
	substate = UNIT;
}

void MainView::switchToNormalState(LocalPlayer *p)
{
	p->resetArmy();
	
	buttons[NEXT]->show();
	buttons[DONE]->hide();
	buttons[PATROL]->hide();
	buttons[WAIT]->hide();
	buttons[BUILD]->hide();
	buttons[CANCEL_SURVEYOR]->hide();
	
	// TODO: Surveyor.updateTile(null);
	
	substate = MAIN;
}

void MainView::switchToSurveyor(LocalPlayer *p)
{
	p->resetArmy();
	
	buttons[CANCEL_SURVEYOR]->show();
	buttons[NEXT]->hide();
	buttons[DONE]->hide();
	buttons[PATROL]->hide();
	buttons[WAIT]->hide();
	buttons[BUILD]->hide();
	
	substate = SURVEYOR;
}

void MainView::updateBuildButton(LocalPlayer *p)
{
	if (p->selectedArmyCanBuildOutpost())
		buttons[BUILD]->activate();
	else
		buttons[BUILD]->deactivate();
}

void MainView::draw()
{
  if (substate != SPELL_CAST && player->getSpellTarget() != Target::NONE)
    switchToSpellCast(player);
  else if (substate == SPELL_CAST && player->getSpellTarget() == Target::NONE)
    switchToNormalState(player);
  
  Gfx::draw(TextureID::MAIN_BACKDROP, 0, 0);
  
  
  Viewport::drawMainViewport(player, g->world);
  const Position vp = player->getViewport();
  Viewport::drawMicroMap(player, 251, 21, 58, 30, vp.x, vp.y, vp.plane); // 58, 30
}

void MainView::drawPost()
{
  /*Gfx.draw(Texture.RESEARCH_LAB_BACKDROP, 0, 0);
   Gfx.draw(Texture.RESEARCH_WIZARDS, player.wizard.ordinal(), 69, 75);
   Gfx.bindColorMap(ColorMap.researchBeam);
   Gfx.drawAnimated(Texture.RESEARCH_BEAM, 0, 132, -3, 0);
   Gfx.unbindColorMap();
   Gfx.draw(Texture.RESEARCH_TOME,149,133);
   //TODO: real major school of wizard
   Gfx.draw(Texture.RESEARCH_MASCOTS,4,190,160);*/
  
  //ColorMap.BlinkMap blink = new ColorMap.BlinkMap(new int[]{Gfx.color(223, 150, 28)}, 0,0,0,    180,30,0,  6);
  //Fonts.drawString("Test Blink", Fonts.Face.YELLOW_SMALL, 50, 50, Fonts.Align.LEFT, blink);
  
  /*Gfx.bindBuffer();
   Gfx.resetBuffer(179,136);
   Gfx.draw(Texture.SUMMON_BACKDROP, 0, 0);
   Gfx.drawAnimated(Texture.SUMMON_ANIMATION, 1, 14+82-31, 26+97-43, 0);
   Gfx.drawAnimated(Texture.SUMMON_ANIMATION, 0, 0+82-31, 0+97-43, 0);
   Gfx.colorMapBuffer(179, 136, ColorMap.summonAnimationRed);
   Gfx.mergeBuffer(0, 0, 31, 43, 179, 136);
   Gfx.bindCanvas();
   Gfx.drawClippedFromHeight(Texture.SUMMON_WIZARDS, 0, 7, 39, 46+5, 5);*/
}
