#include "MainView.h"

#include "Buttons.h"
#include "Gfx.h"
#include "Font.h"
#include "Viewport.h"
#include "UnitDraw.h"
#include "Animations.h"

#include "Game.h"
#include "World.h"
#include "LocalPlayer.h"
#include "Army.h"
#include "Unit.h"
#include "Tile.h"
#include "City.h"

#include "ViewManager.h"

#include "OutpostView.h"
#include "CityView.h"
#include "ArmyView.h"
#include "UnitDetailView.h"

MainView::MainView(ViewManager *gvm) : View(gvm)
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
  buttons[CANCEL_SURVEYOR] = TristateButton::build("Cancel", 240, 173, TextureID::MAIN_LOW_BUTTONS, 1);

  buttons[DONE] = TristateButton::build("Done", 246, 176, TextureID::MAIN_SMALL_BUTTONS, 0);
  buttons[PATROL] = TristateButton::build("Patrol", 280, 176, TextureID::MAIN_SMALL_BUTTONS, 1);
  buttons[WAIT] = TristateButton::build("Wait", 246, 186, TextureID::MAIN_SMALL_BUTTONS, 2);
  buttons[BUILD] = TristateButton::build("Build", 280, 186, TextureID::MAIN_SMALL_BUTTONS, 3);
  // CANCEL BUTTON MISSING
  
  buttons[SPELLS]->setAction([gvm](){ gvm->switchView(VIEW_MAGIC); });
  buttons[ARMIES]->setAction([gvm](){ gvm->switchView(VIEW_ARMIES); });
  buttons[PLANE]->setAction([this](){ player->switchPlane(); switchToNormalState(); });
  buttons[NEXT]->setAction([this](){ LocalGame::i->getGame()->nextTurn(); switchToNormalState(); });
  buttons[CANCEL_SURVEYOR]->setAction([this](){ if (substate == SPELL_CAST) g->cancelCast(player); switchToNormalState(); });
  buttons[DONE]->setAction([this](){ switchToNormalState(); }); /*if (player.selectedArmy() != null && player.selectedRoute() != null && !player.selectedRoute().completed()) player.saveRoute();*/
  buttons[PATROL]->setAction([this](){ player->getSelectedArmy()->patrol(); switchToNormalState(); });
  buttons[BUILD]->setAction([this](){ g->settleCity(player->getSelectedArmy(), "Test"); player->resetArmy(); });
  
  for (auto e : {DONE,PATROL,WAIT,BUILD,CANCEL_SURVEYOR} )
    buttons[e]->hide();
  
  buttons[BUILD]->deactivate();
  
  substate = MAIN;
}

void MainView::switchToSpellCast()
{
	player->resetArmy();
	
	buttons[NEXT]->hide();
	buttons[DONE]->hide();
	buttons[PATROL]->hide();
	buttons[WAIT]->hide();
	buttons[BUILD]->hide();
	buttons[CANCEL_SURVEYOR]->show();
	
	substate = SPELL_CAST;
}

void MainView::switchToUnitSelection(Army* a)
{
	player->selectArmy(a);
	a->unpatrol();
	player->selectAll();
	
	buttons[CANCEL_SURVEYOR]->hide();
	buttons[NEXT]->hide();
	buttons[DONE]->show();
	buttons[PATROL]->show();
	buttons[WAIT]->show();
	buttons[BUILD]->show();
	
	updateBuildButton();
  
	substate = UNIT;
}

void MainView::switchToNormalState()
{
	player->resetArmy();
	
	buttons[NEXT]->show();
	buttons[DONE]->hide();
	buttons[PATROL]->hide();
	buttons[WAIT]->hide();
	buttons[BUILD]->hide();
	buttons[CANCEL_SURVEYOR]->hide();
	
	surveyor.updateInfo(nullptr);
	
	substate = MAIN;
}

void MainView::switchToSurveyor()
{
	player->resetArmy();
	
	buttons[CANCEL_SURVEYOR]->show();
	buttons[NEXT]->hide();
	buttons[DONE]->hide();
	buttons[PATROL]->hide();
	buttons[WAIT]->hide();
	buttons[BUILD]->hide();
	
	substate = SURVEYOR;
}

void MainView::updateBuildButton()
{
	if (player->selectedArmyCanBuildOutpost())
		buttons[BUILD]->activate();
	else
		buttons[BUILD]->deactivate();
}

void MainView::draw()
{
  //Fonts::drawString("Antani", FontFace::WHITE_SMALL, 278, 100, ALIGN_CENTER); return;
  
  if (substate != SPELL_CAST && player->getSpellTarget() != Target::NONE)
    switchToSpellCast();
  else if (substate == SPELL_CAST && player->getSpellTarget() == Target::NONE)
    switchToNormalState();
  
  Gfx::draw(TextureID::MAIN_BACKDROP, 0, 0);
  
  const Army* army = player->getSelectedArmy();
  if (army)
  {
    for (int j = 0; j < army->size(); ++j)
    {
      Unit *unit = army->get(j);
      
      int x = 246+23*(j%3), y = 78+29*(j/3);
      Gfx::draw(TextureID::MAIN_UNITS_RIGHT_BACKDROP, 0, (j/3)*3+(j%3), x, y);
      
      UnitDraw::drawStatic(unit, x+1, y+1, player->isSelectedUnit(unit), false);
      
      // draw unit level
      s16 levelIndex = unit->level ? unit->level->index(): -1;
      if (levelIndex > 0)
      {
        --levelIndex;
        s16 badge = levelIndex / 3;
        s16 badgeCount = levelIndex % 3;
        
        // TODO: in realtà le icone non hanno bordo nero nel gioco anche se nelle png sì
        for (int u = 0; u < badgeCount+1; ++u)
          Gfx::draw(TextureID::UNITS_MISC_SYMBOLS, 1, badge, x + 3 + 4*u, y + 22);
      }
    }
    
    /* compute and draw movement of current selection */
    s16 moves = player->selectedAvailMoves();
    if (moves > 0)
    {
      Fonts::drawString(Fonts::format("Moves: %d%s",moves/2, moves%2 == 0 ? ".5" : "" ), FontFace::WHITE_SMALL, 246, 167, ALIGN_LEFT);
      auto movement = player->selectedArmyMovementType();
      
      /* TODO: non corporeal symbol in game looks like swimming, not flying */
      if (movement.find(&Effects::NON_CORPOREAL) != movement.end() || movement.find(&Effects::FLYING) != movement.end()) // TODO: why?
        movement.erase(movement.find(&Effects::NON_CORPOREAL));

      //TODO: fixare icona movimento che ora usa un Set<SkillEffect.MovementBonus>
      /*if (movement.size() > 0)
       for (int i = 0; i < movement.size(); ++i)
       Gfx.draw(Texture.UNITS_MISC_SYMBOLS, 0, 1+movement.get(i).ordinal(), 306-10*i, 167);
       else
       Gfx.draw(Texture.UNITS_MISC_SYMBOLS, 0, 0, 306, 167);*/
    }
  }
  else if (substate == SURVEYOR)
  {
    Gfx::draw(TextureID::MAIN_RIGHT_BACKDROPS, 0, 1, 240, 76);
    surveyor.draw();
  }
  else if (substate == SPELL_CAST)
  {
    Gfx::draw(TextureID::MAIN_RIGHT_BACKDROPS, 0, 2, 240, 76);
    Fonts::drawString("Casting", FontFace::SURVEY_SERIF, 240+6+8+4, 76+2+1, ALIGN_LEFT);
    
    //TODO: colors etc
    Fonts::drawStringBounded("^wSelect a friendly unit as the target for a ^yBless^^ ^wspell", FontFace::SMALL_SHORTER, 249, 114, 64, ALIGN_LEFT);
  }
  else
    Gfx::draw(TextureID::MAIN_RIGHT_BACKDROPS, 0, 0, 240, 76);
  
  //Sprites.drawTile(p, Sprites.Texture.RIGHT_BACKDROP, 0, 0, 480, 152);
  Viewport::drawMainViewport(player, g->world);
  //Fonts.drawStringBounded("This shows the current city and the surrounding area. The city does not control/work the darkened corners. If a nearby city and this city control the same square, each city receives only half of the square's value.", Fonts.Face.TEAL_SMALL, 20, 30, 173, Fonts.Align.CENTER);

  const Position vp = player->getViewport();
  Viewport::drawMicroMap(player, 251, 21, 58, 30, vp.x, vp.y, vp.plane); // 58, 30
  Gfx::drawClipped(TextureID::UNIT_DETAIL_SPECIAL_THINGS, 251 + 58/2 - 7, 21 + 30/2 - 6, 59, 0, 14, 12);


  if (substate == MAIN)
  {
    int gg = player->goldDelta(), f = player->foodDelta(), m = player->manaDelta();
    
    Fonts::drawString(Fonts::format("%d Gold",gg), gg > 0 ? FontFace::YELLOW_TINY_STROKE : FontFace::RED_TINY_STROKE, 278, 100, ALIGN_CENTER);
    Fonts::drawString(Fonts::format("%d Food",f), f > 0 ? FontFace::YELLOW_TINY_STROKE : FontFace::RED_TINY_STROKE, 278, 132, ALIGN_CENTER);
    Fonts::drawString(Fonts::format("%d Mana",m), m > 0 ? FontFace::YELLOW_TINY_STROKE : FontFace::RED_TINY_STROKE, 278, 164, ALIGN_CENTER);
  }
  
  if (substate == MAIN || substate == UNIT)
  {
    Fonts::drawString(Fonts::format("%d",player->totalGoldPool()), FontFace::WHITE_SMALL, 259, 68, ALIGN_RIGHT);
    Fonts::drawString("GP", FontFace::WHITE_TINY, 268, 67, ALIGN_LEFT);
    Fonts::drawString(Fonts::format("%d",player->totalManaPool()), FontFace::WHITE_SMALL, 297, 68, ALIGN_RIGHT);
    Fonts::drawString("MP", FontFace::WHITE_TINY, 306, 67, ALIGN_LEFT);
  }
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

void MainView::mouseReleased(u16 x, u16 y, MouseButton b)
{
  //gvm->push(new Blink(1000, Gfx::color(0, 0, 255), {0,0,320,200}, 220));
  /*gvm->cityView()->setCity(g->getCities().front());*/
  //gvm->switchView(VIEW_ITEM_CRAFT);
  
  Position pos = Viewport::hoveredPosition(g->world, player, x, y);
  Tile* t = g->world->get(pos);
  
  if (t)
  {
    pos = t->position;
    City* c = t->city;
    Army* a = t->army;
    
    if (b == BUTTON_RIGHT)
    {
      if (a && (!c || (c && !a->isPatrolling())) && a->getOwner() == player)
      {
        switchToUnitSelection(a);
      }
      else if (c)
      {
        if (!c->isOutpost())
        {
          gvm->cityView()->setCity(c);
          gvm->switchView(VIEW_CITY);
        }
        else
        {
          gvm->outpostView()->setCity(c);
          gvm->switchOverview(VIEW_OUTPOST);
        }
        
        player->resetArmy();
        switchToNormalState();
      }
      else
      {
        s16 upperBoundY = pos.y - Viewport::viewportH/2;
        s16 lowerBoundY = pos.y + (Viewport::viewportH  - Viewport::viewportH/2);
        s16 ty = pos.y;
        
        if (upperBoundY < 0)
          ty = Viewport::viewportH/2;
        else if (lowerBoundY >= g->world->h)
          ty = g->world->h - (Viewport::viewportH  - Viewport::viewportH/2);
        
        player->setViewport(pos.x, ty);
      }
    }
    else if (b == BUTTON_LEFT)
    {
      if (substate == SPELL_CAST && player->getSpellTarget() == Target::MAP_TILE)
        g->castSpell(t, player);
      else if (substate == SPELL_CAST && player->getSpellTarget() == Target::FRIENDLY_UNIT)
      {
        if (t->army && t->army->getOwner() == player)
        {
          // TODO: check if unit is just 1 directly cast?
          gvm->armyView()->setArmy(t->army);
          gvm->armyView()->setAcceptSpellTarget();
          gvm->switchOverview(VIEW_ARMY);
        }
      }
      else if (substate == SPELL_CAST && player->getSpellTarget() == Target::FRIENDLY_CITY)
      {
        if (t->city && t->city->getOwner() == player)
          g->castSpell(t->city, player);
      }
      else
      {
        Army* army = player->getSelectedArmy();
        Route* route = player->getRoute();
        
        if (army && !route)
          player->computeRoute(t->x(), t->y());
        else if (army && !route->completed() && route->dx() == t->x() && route->dy() == t->y() && player->selectedAvailMoves() > 0)
        {
          player->consumeRoute();
          player->push(new anims::UnitMovement(player, army, army->getRoute()->pendingPositions()));

          updateBuildButton();
        }
        else if (army)
          player->computeRoute(t->x(),t->y());
        
        /*if (route != null && !route.completed())
         System.out.println(cx+" "+cy+"  "+route.dx()+" "+route.dy());*/
      }
    }
    
  }
  else
  {
    Army* army = player->getSelectedArmy();
    if (army)
    {
      for (int j = 0; j < army->size(); ++j)
      {
        int xx = (246+23*(j%3)), yy = (78+29*(j/3));
        if (x >= xx+2 && x <= xx+20+2 && y >= yy+2 && y <= yy+18+2)
        {
          // select/deselect unit in army
          if (b == BUTTON_LEFT)
          {
            Unit* u = army->get(j);
            if (player->isSelectedUnit(u))
              player->deselectUnit(u);
            else
              player->selectUnit(u);
            
            updateBuildButton();
            
            army->clearRoute();
          }
          // open detail view on unit
          else if (b == BUTTON_RIGHT)
          {
            gvm->unitDetailView()->setUnit(army->get(j));
            gvm->switchOverview(VIEW_UNIT);
          }
        }
      }
    }
  }
}

void MainView::mouseMoved(u16 x, u16 y, MouseButton b)
{
  if (substate == SURVEYOR)
  {
    const Position pos = Viewport::hoveredPosition(g->world, player, x, y);
    Tile* tile = g->world->get(pos);
    if (tile && tile != hoveredTile) surveyor.updateInfo(tile);
    hoveredTile = tile;
  }
}

void MainView::keyPressed(KeyboardKey key, KeyboardMod mod)
{
  switch (key)
  {
    case SDL_SCANCODE_RETURN: buttons[NEXT]->press(); break;
      
    default: break;
  }
}

void MainView::keyReleased(KeyboardKey key, KeyboardMod mod)
{
  switch (key)
  {
    case SDL_SCANCODE_RETURN:
    {
      buttons[NEXT]->release();
      buttons[NEXT]->click();
    }
    case SDL_SCANCODE_F1: switchToSurveyor(); break;
    case SDL_SCANCODE_F3: gvm->switchView(VIEW_RESEARCH); break;
    case SDL_SCANCODE_F9: gvm->switchView(VIEW_MIRROR); break;
      
    default: break;

  }
}

void MainView::Surveyor::updateInfo(Tile *t)
{
  tile = t;
  
  if (!tile)
  {
    cityCanBeBuilt = false;
    cityForbidMsg = I18::EMPTY;
    return;
  }
  
  cityCanBeBuilt = view.g->cityMechanics.canCityBeBuiltOnTile(t);
  
  if (!cityCanBeBuilt)
  {
    World* w = view.g->world;
    
    if (!t || t->city)
    {
      cityForbidMsg = I18::EMPTY;
      return;
    }
    else if (t->type == TILE_WATER || t->type == TILE_SHORE)
    {
      cityForbidMsg = I18::SURVEYOR_CITY_FORBID_WATER;
      return;
    }
    
    for (int i = -3; i <= 3; ++i)
      for (int j = -3; j <= 3; ++j)
      {
        Tile* t2 = w->get(t->position.x+i, t->position.y+j, t->position.plane);
        
        if (t2 && t2->city)
        {
          cityForbidMsg = I18::SURVEYOR_CITY_FORBID_DISTANCE;
          return;
        }
      }
    
    cityForbidMsg = I18::EMPTY;
  }
}

void MainView::Surveyor::draw()
{
  Fonts::drawString("Surveyor", FontFace::SURVEY_SERIF, 240+6+8, 76+2+1, ALIGN_LEFT);
  
  if (tile && view.g->currentPlayer()->fog()->get(tile->position))
  {
    Fonts::setFace(FontFace::YELLOW_SMALL, 0, 2);
    
    if (!cityCanBeBuilt)
    {
      const City* city = tile->city;
      
      if (!city)
        Fonts::drawStringBounded(i18n::s(cityForbidMsg), 246, 143, 68, ALIGN_LEFT, &FontMap::Small::YELLOW_PALE);
      else
      {
        // draw city name and size
        Fonts::drawString(i18n::s(i18n::CITY_SIZE_NAMES[city->tileSize()])+" of", 276, 115, ALIGN_CENTER, &FontMap::Small::YELLOW_PALE);
        Fonts::drawString(city->getName(), 276, 115+7, ALIGN_CENTER, &FontMap::Small::YELLOW_PALE);
        
        // draw city specs
        Fonts::drawString("City Resources", 276, 143, ALIGN_CENTER, &FontMap::Small::YELLOW_PALE);
        
        const std::string pop = Fonts::format(i18n::s(I18::SURVEYOR_MAX_POPULATION).c_str(), city->getMaxPopulation());
        const std::string prod = Fonts::format(i18n::s(I18::SURVEYOR_MAX_POPULATION).c_str(), (s32)(view.g->cityMechanics.computeProductionBonus(city)*100));
        
        Fonts::drawString(pop, 276, 150, ALIGN_CENTER, &FontMap::Small::WHITE_PALE);
        Fonts::drawString(prod, 276, 157, ALIGN_CENTER, &FontMap::Small::WHITE_PALE);
      }
    }
    
    auto t = i18n::surveyorDesc(tile->type);

    /* TODO: finish surveyor */
    
    if (!t.empty())
    {
      for (int i = 0; i < t.size(); ++i)
      {
        Fonts::setMap(i == 0 ? &FontMap::Small::YELLOW_PALE : &FontMap::Small::WHITE_PALE);
        Fonts::drawString(t[i], 276, 91+7*i, ALIGN_CENTER);
      }
    }
  }
}
