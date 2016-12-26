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

#include "LBX.h"

#include "Animations.h"

enum lbx_indices
{
  main_backdrop = LBXI(MAIN,0)
};

MainView::MainView(ViewManager *gvm) : View(gvm), hoveredTile(nullptr)
{
  buttons.resize(BUTTON_COUNT);
  
  buttons[GAME] = BistateButton::buildLBX("Game", 7, 4, LSI(MAIN, 1));
  buttons[SPELLS] = BistateButton::buildLBX("Spells", 47, 4, LSI(MAIN, 2));
  buttons[ARMIES] = BistateButton::buildLBX("Armies", 89, 4, LSI(MAIN, 3));
  buttons[CITIES] = BistateButton::buildLBX("Cities", 140, 4, LSI(MAIN, 4));
  buttons[MAGIC] = BistateButton::buildLBX("Magic", 184, 4, LSI(MAIN, 5));
  buttons[INFO] = BistateButton::buildLBX("Info", 226, 4, LSI(MAIN, 6));
  buttons[PLANE] = BistateButton::buildLBX("Plane", 270, 4, LSI(MAIN, 7));
  
  buttons[NEXT] = TristateButton::build("Next", 240, 173, TextureID::MAIN_LOW_BUTTONS, 0);
  buttons[CANCEL_SURVEYOR] = TristateButton::build("Cancel", 240, 173, TextureID::MAIN_LOW_BUTTONS, 1);

  buttons[DONE] = TristateButton::buildLBX("Done", 246, 176, LSI(MAIN, 8), LSI(MAIN,12));
  buttons[PATROL] = TristateButton::buildLBX("Patrol", 280, 176, LSI(MAIN, 9), LSI(MAIN,13));
  buttons[WAIT] = TristateButton::buildLBX("Wait", 246, 186, LSI(MAIN, 10), LSI(MAIN,14));
  buttons[BUILD] = TristateButton::buildLBX("Build", 280, 186, LSI(MAIN, 11), LSI(MAIN,15));
  // CANCEL BUTTON MISSING
  
  buttons[SPELLS]->setAction([gvm](){ gvm->switchOverview(VIEW_SPELL_BOOK); });
  buttons[MAGIC]->setAction([gvm](){ gvm->switchView(VIEW_MAGIC); });
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

void MainView::activate()
{  
  /*player->send(new msgs::Error("antanius"));
  player->send(new msgs::Error("fotius"));
  player->send(new msgs::NewBuilding(player->getCities().front(), Building::ARMORY));*/
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

SpriteInfo MainView::movementIconForType(const MovementEffect* effect)
{
  if (!effect) return LBXI(MAIN, 38);
  else if (effect == Effects::SWIMMING) return LBXI(MAIN, 19);
  else if (effect == Effects::FLYING) return LBXI(MAIN, 22);
  else if (effect == Effects::SAILING) return LBXI(MAIN, 18);
  else if (effect == Effects::FORESTWALK) return LBXI(MAIN, 21);
  else if (effect == Effects::MOUNTAINWALK) return LBXI(MAIN, 20);
  else if (effect == Effects::PATH_FINDER) return LBXI(MAIN, 23);
  else if (effect == Effects::PLANAR_TRAVEL) return LBXI(MAIN, 36);
  else if (effect == Effects::WINDWALK) return LBXI(MAIN, 37);
  
  //TODO: is it correct? since non corporeal is swimming implicitly
  else if (effect == Effects::NON_CORPOREAL) return LBXI(MAIN, 19);

  
  assert(false);
  return 0;
}

void MainView::draw()
{
  if (substate != SPELL_CAST && player->getSpellTarget() != Target::NONE)
    switchToSpellCast();
  else if (substate == SPELL_CAST && player->getSpellTarget() == Target::NONE)
    switchToNormalState();
  
  Gfx::draw(main_backdrop, 0, 0);
    
  const Army* army = player->getSelectedArmy();
  if (army)
  {
    for (int j = 0; j < army->size(); ++j)
    {
      Unit *unit = army->get(j);
      
      int x = 246+23*(j%3), y = 78+29*(j/3);
      Gfx::draw(LSI(MAIN,24).relative(j), x, y);
      
      UnitDraw::drawStatic(unit, x+1, y+1, player->isSelectedUnit(unit), false);
      
      /* draw unit level */
      s16 levelIndex = unit->level ? unit->level->index(): -1;
      if (levelIndex > 0)
      {
        --levelIndex;
        s16 badge = levelIndex / 3;
        s16 badgeCount = levelIndex % 3;
        
        static const SpriteInfo badge_lbx = LSI(MAIN,51);
        
        // TODO: in realtà le icone non hanno bordo nero nel gioco anche se nelle png sì
        for (int u = 0; u < badgeCount+1; ++u)
          Gfx::draw(badge_lbx.relative(badge), x + 3 + 4*u, y + 22);
      }
    }
    
    /* compute and draw movement of current selection */
    s16 moves = player->selectedAvailMoves();
    if (moves > 0)
    {
      Fonts::drawString(Fonts::format("Moves: %d%s",moves/2, moves%2 == 0 ? "" : ".5" ), FontFaces::Small::WHITE, 245, 166, ALIGN_LEFT);
      movement_list movement = player->selectedArmyMovementType();
      
      /* draw movement icons */
      if (movement.size() > 0)
      {
        size_t i = 0;
        bool hasFlying = movement.contains(Effects::FLYING);
        for (const auto* effect : movement)
        {
          //TODO: maybe this should be decided by the algorithm which computes the movement type
          if (effect == Effects::SWIMMING && hasFlying)
            continue;
          
          Gfx::draw(movementIconForType(effect), 306-10*i++, 167);
        }
      }
      else
        Gfx::draw(movementIconForType(nullptr), 306, 167);
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
    Fonts::drawString("Casting", FontFaces::Serif::WHITE_SURVEY, 240+6+8+4, 76+2+1, ALIGN_LEFT);
    
    //TODO: colors etc
    Fonts::setFace(FontFaces::Small::YELLOW);
    Fonts::setVerSpace(2);
    Fonts::drawStringBounded("^wSelect a friendly unit as the target for a ^yBless^^ ^wspell", 119, 49, 64, ALIGN_LEFT);
  }
  else
    Gfx::draw(TextureID::MAIN_RIGHT_BACKDROPS, 0, 0, 240, 76);
  
  //Sprites.drawTile(p, Sprites.Texture.RIGHT_BACKDROP, 0, 0, 480, 152);
  Viewport::drawMainViewport(player, g->world);
  //Fonts.drawStringBounded("This shows the current city and the surrounding area. The city does not control/work the darkened corners. If a nearby city and this city control the same square, each city receives only half of the square's value.", Fonts.Face.TEAL_SMALL, 20, 30, 173, Fonts.Align.CENTER);

  const Position vp = player->getViewport();
  Viewport::drawMicroMap(player, 251, 21, 58, 30, vp.x, vp.y, vp.plane); // 58, 30
  Gfx::drawClipped(TSI(UNIT_DETAIL_SPECIAL_THINGS,0,0), 251 + 58/2 - 7, 21 + 30/2 - 6, 59, 0, 14, 12);


  if (substate == MAIN)
  {
    int gg = player->goldDelta(), f = player->foodDelta(), m = player->manaDelta();
    
    Fonts::drawString(Fonts::format("%d Gold",gg), gg > 0 ? FontFaces::Tiny::YELLOW_STROKE : FontFaces::Tiny::RED_STROKE, 277, 100, ALIGN_CENTER);
    Fonts::drawString(Fonts::format("%d Food",f), f > 0 ? FontFaces::Tiny::YELLOW_STROKE : FontFaces::Tiny::RED_STROKE, 277, 132, ALIGN_CENTER);
    Fonts::drawString(Fonts::format("%d Mana",m), m > 0 ? FontFaces::Tiny::YELLOW_STROKE : FontFaces::Tiny::RED_STROKE, 277, 164, ALIGN_CENTER);
  }
  
  if (substate == MAIN || substate == UNIT)
  {
    Fonts::drawString(Fonts::format("%d",player->totalGoldPool()), FontFaces::Small::WHITE, 266, 67, ALIGN_RIGHT);
    Fonts::drawString("GP", FontFaces::Tiny::WHITE, 267, 67, ALIGN_LEFT);
    Fonts::drawString(Fonts::format("%d",player->totalManaPool()), FontFaces::Small::WHITE, 304, 67, ALIGN_RIGHT);
    Fonts::drawString("MP", FontFaces::Tiny::WHITE, 305, 67, ALIGN_LEFT);
  }
  
#if DEBUG
  {
    u32 turns = g->getTurnCount();
    const auto& players = g->getPlayers();
    auto it = std::find(players.begin(), players.end(), g->currentPlayer());
    size_t indexOfCurrentPlayer = std::distance(players.begin(), it);
    auto it2 = std::find(players.begin(), players.end(), player);
    size_t indexOfViewPlayer = std::distance(players.begin(), it2);
    Fonts::drawString(Fonts::format("t %u gp %zu vp %zu %d,%d", turns, indexOfCurrentPlayer, indexOfViewPlayer, hoveredTile ? hoveredTile->x() : -1, hoveredTile ? hoveredTile->y() : -1), FontFaces::Tiny::WHITE_STROKE, 0, HEIGHT-10, ALIGN_LEFT);
  }
#endif
}

void MainView::drawPost()
{
  /*Gfx::draw(TextureID::RESEARCH_LAB_BACKDROP, 0, 0);
  Gfx::draw(Texture::RESEARCH_WIZARDS, player->wi, 69, 75);
   Gfx.bindColorMap(ColorMap.researchBeam);
   Gfx.drawAnimated(Texture.RESEARCH_BEAM, 0, 132, -3, 0);
   Gfx.unbindColorMap();
   Gfx.draw(Texture.RESEARCH_TOME,149,133);
   //TODO: real major school of wizard
   Gfx.draw(Texture.RESEARCH_MASCOTS,4,190,160);*/
  
  //ColorMap.BlinkMap blink = new ColorMap.BlinkMap(new int[]{Gfx.color(223, 150, 28)}, 0,0,0,    180,30,0,  6);
  //Fonts.drawString("Test Blink", Fonts.Face.YELLOW_SMALL, 50, 50, Fonts.Align.LEFT, blink);
  
  /*SpriteInfo wizard = LSI(SPELLSCR, 53);
  
  lbx::LBXSpriteDataWithPalette summonBg(lbx::Repository::spriteFor(LSI(SPELLSCR,9)), lbx::Repository::spriteFor(LSI(SPELLSCR,64))->getPalette());
  lbx::LBXSpriteDataWithPalette summonFlame(lbx::Repository::spriteFor(LSI(SPELLSCR,11)), lbx::Repository::spriteFor(LSI(SPELLSCR,64))->getPalette());
  lbx::LBXSpriteDataWithPalette summonFlame2(lbx::Repository::spriteFor(LSI(SPELLSCR,10)), lbx::Repository::spriteFor(LSI(SPELLSCR,64))->getPalette());

  ScreenCoord base = ScreenCoord(0, 0);
  u16 creatureTopY = 18;
  u16 creatureBottomY = 90;
  
  static u32 startTicks = Gfx::ticks;
  u32 elapsed = Gfx::ticks - startTicks;
  
  float percent = elapsed / 5000.0f;
  u16 creatureY = creatureBottomY - (creatureBottomY - creatureTopY)*percent;
  
  printf("elapsed: %u, percent: %2.2f, y: %u\n", elapsed, percent, creatureY);

  
  Gfx::draw(&summonBg, base.x, base.y);
  
  u16 frame = (Gfx::fticks % 15);
  
  Gfx::drawClipped(wizard, base.x + 8, base.y + 3 + 5, 0, 5, wizard.sw(), wizard.sh()-5);
  Gfx::draw(&summonFlame2, base.x + 55, base.y + 0+97-43, frame);
  Gfx::draw(LSI(MONSTER, 22), base.x + 76, base.y + creatureY);
  Gfx::draw(&summonFlame, base.x + 65, base.y + 26+97-43, frame);*/
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
      if (a && (!c || (c && !a->isPatrolling())))
      {
        if (a->getOwner() == player)
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
        const auto& route = player->getRoute();
        
        if (army && !route)
          player->computeRoute(t->position);
        else if (army && !route->completed() && route->dx() == t->x() && route->dy() == t->y() && player->selectedAvailMoves() > 0)
        {
          player->consumeRoute();
          player->push(new anims::UnitMovement(player, army, army->getRoute()->pendingPositions()));

          updateBuildButton();
        }
        else if (army)
          player->computeRoute(t->position);
        
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
            Unit* unit = army->get(j);
            
            if (army->size() > 2 && player->isSelectedUnit(unit) && std::all_of(army->begin(), army->end(), [this](Unit* u) { return player->isSelectedUnit(u); }))
            {
              for (Unit* aunit : *army)
                if (aunit != unit)
                  player->deselectUnit(aunit);
            }
            else
            {
              if (player->isSelectedUnit(unit))
                player->deselectUnit(unit);
              else
                player->selectUnit(unit);
            }

            
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
  //if (substate == SURVEYOR)
  {
    const Position pos = Viewport::hoveredPosition(g->world, player, x, y);
    Tile* tile = g->world->get(pos);
    if (tile && tile != hoveredTile) surveyor.updateInfo(tile);
    hoveredTile = tile;
  }
}

void MainView::keyPressed(KeyboardCode key, KeyboardKey kkey, KeyboardMod mod)
{
  switch (key)
  {
    case SDL_SCANCODE_RETURN: buttons[NEXT]->press(); break;
    case SDL_SCANCODE_GRAVE: gvm->switchOverview(VIEW_CONSOLE);
      
    default: break;
  }
}

void MainView::keyReleased(KeyboardCode key, KeyboardKey kkey, KeyboardMod mod)
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
    case SDL_SCANCODE_F9: gvm->switchOverview(VIEW_MIRROR); break;
      
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
  Fonts::drawString("Surveyor", FontFaces::Serif::WHITE_SURVEY, 240+6+7, 76+2+1, ALIGN_LEFT);
  Fonts::setFace(FontFaces::Small::YELLOW_PALE, 0, 0);

  
  if (tile && view.g->currentPlayer()->fog()->get(tile->position))
  {
    if (!cityCanBeBuilt)
    {
      const City* city = tile->city;
      
      if (!city)
      {
        Fonts::setFace(FontFaces::Small::YELLOW_PALE, -1, 0);
        Fonts::setSpaceAdj(-2);
        Fonts::drawStringBounded(i18n::s(cityForbidMsg), 245, 142, 60, ALIGN_LEFT);
      }
      else
      {
        // draw city name and size
        Fonts::drawString(i18n::s(i18n::CITY_SIZE_NAMES[city->tileSize()])+" of", 275, 114, ALIGN_CENTER);
        Fonts::drawString(city->getName(), 275, 114+7, ALIGN_CENTER);
        
        // draw city specs
        Fonts::drawString("City Resources", 272, 142, ALIGN_CENTER);
        
        const std::string pop = Fonts::format(i18n::s(I18::SURVEYOR_MAX_POPULATION).c_str(), city->getMaxPopulation());
        const std::string prod = Fonts::format(i18n::s(I18::SURVEYOR_MAX_POPULATION).c_str(), (s32)(view.g->cityMechanics.computeProductionBonus(city)*100));
        
        Fonts::setFace(FontFaces::Small::WHITE_PALE, 0, 0);
        Fonts::drawString(pop, 244, 149, ALIGN_LEFT);
        Fonts::drawString(prod, 244, 156, ALIGN_LEFT);
      }
    }
    
    auto t = i18n::surveyorDesc(tile->type);

    /* TODO: finish surveyor */
    
    if (!t.empty())
    {
      Fonts::setFace(FontFaces::Small::WHITE_PALE, 0, 0);
      for (int i = 0; i < t.size(); ++i)
      {
        Fonts::setMap(i == 0 ? FontFaces::Palettes::SMALL_YELLOW_PALE : FontFaces::Palettes::SMALL_WHITE_PALE);
        Fonts::drawString(t[i], 274, 90+7*i, ALIGN_CENTER);
      }
    }
  }
}
