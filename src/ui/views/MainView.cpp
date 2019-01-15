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

#include "Help.h"
#include "Animations.h"
#include "Dialogs.h"

enum lbx_indices
{
  main_backdrop = LBXI(MAIN,0),
  
  right_backdrop_main = LBXI(MAIN, 34),  // 80x97
  right_backdrop_cast = LBXI(MAIN, 40),  // 80x98
  right_backdrop_survey = LBXI(MAIN, 57),  // 80x98
  right_backdrop_road = LBXI(MAIN, 45), // 80x98
  
  next_button_backdrop = LBXI(MAIN, 35), // 80x27
  cancel_button_backdrop = LBXI(MAIN, 47), // 80x27
  road_buttons_backdrop = LBXI(MAIN, 48), // 80x27
};

MainView::MainView(ViewManager *gvm) : View(gvm), hoveredTile(nullptr)
{
  buttons.resize(BUTTON_COUNT);
  
  buttons[GAME] = Button::buildBistate("Game", 7, 4, LSI(MAIN, 1));
  buttons[SPELLS] = Button::buildBistate("Spells", 47, 4, LSI(MAIN, 2));
  buttons[ARMIES] = Button::buildBistate("Armies", 89, 4, LSI(MAIN, 3));
  buttons[CITIES] = Button::buildBistate("Cities", 140, 4, LSI(MAIN, 4));
  buttons[MAGIC] = Button::buildBistate("Magic", 184, 4, LSI(MAIN, 5));
  buttons[INFO] = Button::buildBistate("Info", 226, 4, LSI(MAIN, 6));
  buttons[PLANE] = Button::buildBistate("Plane", 270, 4, LSI(MAIN, 7));
  
  buttons[NEXT] = Button::buildPressedOnly("Next", 246, 178, LSI(MAIN, 58));
  
  buttons[DONE] = Button::buildTristate("Done", 246, 176, LSI(MAIN, 8), LSI(MAIN,12));
  buttons[PATROL] = Button::buildTristate("Patrol", 280, 176, LSI(MAIN, 9), LSI(MAIN,13));
  buttons[WAIT] = Button::buildTristate("Wait", 246, 186, LSI(MAIN, 10), LSI(MAIN,14));
  buttons[BUILD_ROAD] = Button::buildTristate("Build", 280, 186, LSI(MAIN, 11), LSI(MAIN,15));
  buttons[BUILD_OUTPOST] = Button::buildTristate("Build", 280, 186, LSI(MAIN, 11), LSI(MAIN,15));
  buttons[PURIFY] = Button::buildTristate("Purify", 280, 186, LSI(MAIN, 42), LSI(MAIN, 43));
  buttons[MELD] = Button::buildBistate("Meld", 280, 186, LSI(MAIN, 49));
  
  buttons[ROAD_OK] = Button::buildBistate("Road Ok", 246, 181, LSI(MAIN, 46));
  buttons[ROAD_CANCEL] = Button::buildBistate("Road Cancel", 280, 181, LSI(MAIN, 41));
  
  buttons[SURVEYOR_CANCEL] = Button::buildBistate("surveyor cancel", 263, 181, LSI(MAIN, 41));
  buttons[SPELLCAST_CANCEL] = Button::buildBistate("spellcast cancel", 263, 181, LSI(MAIN, 41));

  // CANCEL BUTTON MISSING
  
  buttons[GAME]->setAction([gvm](){ gvm->switchView(VIEW_LOAD); });
  buttons[SPELLS]->setAction([gvm](){ gvm->switchOverview(VIEW_SPELL_BOOK); });
  buttons[MAGIC]->setAction([gvm](){ gvm->switchView(VIEW_MAGIC); });
  buttons[ARMIES]->setAction([gvm](){ gvm->switchView(VIEW_ARMIES); });
  buttons[CITIES]->setAction([gvm](){ gvm->switchView(VIEW_CITIES); });
  buttons[INFO]->setAction([gvm](){ gvm->switchOverview(VIEW_INFO_MENU); });
  buttons[PLANE]->setAction([this](){ player->switchPlane(); switchToNormalState(); });
  buttons[NEXT]->setAction([this](){ LocalGame::i->getGame()->nextTurn(); switchToNormalState(); });
  
  buttons[SURVEYOR_CANCEL]->setAction([this]() { switchToNormalState(); });
  buttons[SPELLCAST_CANCEL]->setAction([this]() { g->cancelCast(player); switchToNormalState(); });
  
  buttons[DONE]->setAction([this](){ switchToNormalState(); }); /*if (player.selectedArmy() != null && player.selectedRoute() != null && !player.selectedRoute().completed()) player.saveRoute();*/
  buttons[PATROL]->setAction([this](){ player->getSelectedArmy()->patrol(); switchToNormalState(); });
  buttons[BUILD_OUTPOST]->setAction([this](){ g->settleCity(player->getSelectedArmy(), "Test"); player->resetArmy(); });
  buttons[BUILD_ROAD]->setAction([this](){ switchToRoadBuilding(); });
  
  buttons[ROAD_CANCEL]->setAction([this](){ switchToUnitSelection(player->getSelectedArmy()); });
  buttons[ROAD_OK]->setAction([this](){ /* TODO */ });

  
  for (const auto e : { DONE,PATROL,WAIT,BUILD_ROAD,BUILD_OUTPOST,PURIFY,MELD,SURVEYOR_CANCEL,SPELLCAST_CANCEL,ROAD_OK,ROAD_CANCEL } )
    buttons[e]->hide();
  
  buttons[BUILD_OUTPOST]->deactivate();
  
  substate = MAIN;
}

void MainView::activate()
{  
  //player->send(new msgs::Help(help::Data::at(289)));
  /*player->send(new msgs::Error("fotius"));
  player->send(new msgs::NewBuilding(player->getCities().front(), Building::ARMORY));*/
  //player->push(new anims::SummonAnimation(WizardID::FREYA, UnitSpec::summonSpec(UnitID::HELL_HOUNDS)));
  //player->push(new anims::SpellDiscoverAnimation(WizardID::SSS_RA, School::LIFE));
  //player->push(new anims::GlobalEnchantmentAnimation(player->wizard, LSI(SPECFX, 34)));
}

void MainView::switchToSpellCast()
{
	player->resetArmy();
	
	buttons[NEXT]->hide();
  
  for (const auto e : { SURVEYOR_CANCEL, ROAD_CANCEL, ROAD_OK })
    buttons[e]->hide();
  
  for (const auto e : { DONE, PATROL, WAIT, BUILD_OUTPOST, BUILD_ROAD })
    buttons[e]->hide();
  
  buttons[SPELLCAST_CANCEL]->show();
	
	substate = SPELL_CAST;
}

void MainView::switchToUnitSelection(Army* a)
{
	player->selectArmy(a);
	a->unpatrol();
	player->selectAll();
  
  buttons[NEXT]->hide();
  
  for (const auto e : { SURVEYOR_CANCEL, ROAD_CANCEL, ROAD_OK })
    buttons[e]->hide();
  
  for (const auto e : { DONE, PATROL, WAIT })
    buttons[e]->show();
	
	updateBuildButton();
  
	substate = UNIT;
}

void MainView::switchToNormalState()
{
	player->resetArmy();
	
	buttons[NEXT]->show();
  
  for (const auto e : { DONE, PATROL, WAIT, BUILD_OUTPOST, BUILD_ROAD })
    buttons[e]->hide();
  
  for (const auto e : { SURVEYOR_CANCEL, ROAD_CANCEL, ROAD_OK, SPELLCAST_CANCEL })
    buttons[e]->hide();
	
	surveyor.updateInfo(nullptr);
	
	substate = MAIN;
}

void MainView::switchToSurveyor()
{
	player->resetArmy();
	
	buttons[NEXT]->hide();
  
  for (const auto e : { DONE, PATROL, WAIT, BUILD_ROAD, BUILD_OUTPOST })
    buttons[e]->hide();
  
  for (const auto e : { ROAD_CANCEL, ROAD_OK, SPELLCAST_CANCEL })
    buttons[e]->hide();
  
	buttons[SURVEYOR_CANCEL]->show();

	substate = SURVEYOR;
}

void MainView::switchToRoadBuilding()
{
  buttons[NEXT]->hide();

  for (const auto e : { DONE, PATROL, WAIT, BUILD_ROAD, BUILD_OUTPOST, SURVEYOR_CANCEL, SPELLCAST_CANCEL })
    buttons[e]->hide();
  
  for (const auto e : { ROAD_OK, ROAD_CANCEL })
    buttons[e]->show();
  
  substate = ROAD_BUILDING;
}

void MainView::updateBuildButton()
{
  const Army* army = player->getSelectedArmy();
  const Tile* tile = g->world->get(army->getPosition());
  
  bool hasCreateOutpost = army->any_of([](const Unit* unit) { return unit->skills()->hasSimpleEffect(SimpleEffect::Type::CREATE_OUTPOST); });
  bool hasCreateRoad = army->any_of([](const Unit* unit) { return unit->skills()->hasSimpleEffect(SimpleEffect::Type::CREATE_ROAD); });
  
  if (hasCreateOutpost && g->cityMechanics.canCityBeBuiltOnTile(tile))
  {
    buttons[BUILD_OUTPOST]->show();
    buttons[BUILD_OUTPOST]->activate();
  }
  else if (hasCreateRoad && g->cityMechanics.canRoadBeBuiltOnTile(tile))
    buttons[BUILD_ROAD]->show();
  else
  {
    buttons[BUILD_OUTPOST]->show();
    buttons[BUILD_OUTPOST]->deactivate();
  }
}

SpriteInfo MainView::movementIconForType(const MovementType effect)
{
  switch (effect)
  {
    case MovementType::NORMAL: return LBXI(MAIN, 38);
    case MovementType::SWIMMING: return LBXI(MAIN, 19);
    case MovementType::FLYING: return LBXI(MAIN, 22);
    case MovementType::SAILING: return LBXI(MAIN, 18);
    case MovementType::FORESTWALK: return LBXI(MAIN, 21);
    case MovementType::MOUNTAINWALK: return LBXI(MAIN, 20);
    case MovementType::PATH_FINDER: return LBXI(MAIN, 23);
    case MovementType::PLANAR_TRAVEL: return LBXI(MAIN, 36);
    case MovementType::WINDWALK: return LBXI(MAIN, 37);
  }
  
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
  if (substate == UNIT)
  {
    for (int j = 0; j < army->size(); ++j)
    {
      Unit *unit = army->get(j);
      
      int x = 246+23*(j%3), y = 78+29*(j/3);
      Gfx::draw(LSI(MAIN,24).relative(j), x, y);
      
      UnitDraw::drawStatic(unit, x+1, y+1, player->isSelectedUnit(unit), false);
      
      /* draw unit level */
      UnitDraw::drawUnitLevel(unit->getExperienceLevel(), x + 3, y + 22, 4, false);
    }
    
    /* compute and draw movement of current selection */
    s16 moves = player->selectedAvailMoves();
    if (moves > 0)
    {
      Fonts::drawString(fmt::sprintf("Moves: %s", UnitDraw::stringForDoubleMovement(moves)), FontFaces::Small::WHITE, 245, 166, ALIGN_LEFT);
      movement_list movement = player->selectedArmyMovementType();
      
      /* draw movement icons */
      if (movement.size() > 0)
      {
        size_t i = 0;
        bool hasFlying = movement.contains(MovementType::FLYING);
        for (const auto& effect : movement)
        {
          //TODO: maybe this should be decided by the algorithm which computes the movement type
          if (effect == MovementType::SWIMMING && hasFlying)
            continue;
          
          Gfx::draw(movementIconForType(effect), 306-10*i++, 167);
        }
      }
      else
        Gfx::draw(movementIconForType(MovementType::NORMAL), 306, 167);
    }
  }
  else if (substate == ROAD_BUILDING)
  {
    Gfx::draw(right_backdrop_road, 240, 76);
    Gfx::draw(road_buttons_backdrop, 240, 76 + 97);
    
    Fonts::drawString("Road", FontFaces::Serif::WHITE_SURVEY, 280, 79, ALIGN_CENTER);
    Fonts::drawString("Building", FontFaces::Serif::WHITE_SURVEY, 279, 89, ALIGN_CENTER);

    Fonts::setFace(FontFaces::Small::YELLOW_PALE, 1, 0);
    
    s16 turnsRequired = 6;
    
    Fonts::drawStringBounded(fmt::sprintf("It will take %d turns to complete the construction of this road", turnsRequired), 249, 105, 50, ALIGN_LEFT);
    
    //TODO: management of multiple road by clicking
  }
  else if (substate == SURVEYOR)
  {
    Gfx::draw(right_backdrop_survey, 240, 76);
    Gfx::draw(cancel_button_backdrop, 240, 76 + 97);
    surveyor.draw();
  }
  else if (substate == SPELL_CAST)
  {
    Gfx::draw(right_backdrop_cast, 240, 76);
    Gfx::draw(cancel_button_backdrop, 240, 76 + 97);
    Fonts::drawString("Casting", FontFaces::Serif::WHITE_SURVEY, 240+6+8+4, 76+2+1, ALIGN_LEFT);
    
    //TODO: colors etc
    Fonts::setFace(FontFaces::Small::YELLOW);
    Fonts::setVerSpace(2);
    Fonts::drawStringBounded("^wSelect a friendly unit as the target for a ^yBless^^ ^wspell", 119, 49, 64, ALIGN_LEFT);
  }
  else
  {
    Gfx::draw(right_backdrop_main, 240, 76);
    Gfx::draw(next_button_backdrop, 240, 76+97);
  }
  
  Viewport::drawMainViewport(player, g->world);
  //Fonts.drawStringBounded("This shows the current city and the surrounding area. The city does not control/work the darkened corners. If a nearby city and this city control the same square, each city receives only half of the square's value.", Fonts.Face.TEAL_SMALL, 20, 30, 173, Fonts.Align.CENTER);

  const Position vp = player->getViewport();
  Viewport::drawMicroMap(player, 251, 21, 58, 30, vp.x, vp.y, vp.plane); // 58, 30
  Gfx::drawClipped(TSI(UNIT_DETAIL_SPECIAL_THINGS,0,0), 251 + 58/2 - 7, 21 + 30/2 - 6, 59, 0, 14, 12);


  if (substate == MAIN)
  {
    int gg = player->goldDelta(), f = player->foodDelta(), m = player->manaDelta();
    
    Fonts::drawString(fmt::sprintf("%d Gold",gg), gg > 0 ? FontFaces::Tiny::YELLOW_STROKE : FontFaces::Tiny::RED_STROKE, 277, 100, ALIGN_CENTER);
    Fonts::drawString(fmt::sprintf("%d Food",f), f > 0 ? FontFaces::Tiny::YELLOW_STROKE : FontFaces::Tiny::RED_STROKE, 277, 132, ALIGN_CENTER);
    Fonts::drawString(fmt::sprintf("%d Mana",m), m > 0 ? FontFaces::Tiny::YELLOW_STROKE : FontFaces::Tiny::RED_STROKE, 277, 164, ALIGN_CENTER);
  }
  
  if (substate == MAIN || substate == UNIT || substate == ROAD_BUILDING || substate == SURVEYOR)
  {
    Fonts::drawString(Fonts::format("%d",player->totalGoldPool()), FontFaces::Small::WHITE, 266, 67, ALIGN_RIGHT);
    Fonts::drawString("GP", FontFaces::Tiny::WHITE, 267, 67, ALIGN_LEFT);
    Fonts::drawString(Fonts::format("%d",player->totalManaPool()), FontFaces::Small::WHITE, 304, 67, ALIGN_RIGHT);
    Fonts::drawString("MP", FontFaces::Tiny::WHITE, 305, 67, ALIGN_LEFT);
  }
  
#if defined(DEBUG)
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
  /*
  
   Gfx.bindColorMap(ColorMap.researchBeam);
   Gfx.drawAnimated(Texture.RESEARCH_BEAM, 0, 132, -3, 0);
   Gfx.unbindColorMap();
   Gfx.draw(Texture.RESEARCH_TOME,149,133);
   //TODO: real major school of wizard
   Gfx.draw(Texture.RESEARCH_MASCOTS,4,190,160);*/
  
  //ColorMap.BlinkMap blink = new ColorMap.BlinkMap(new int[]{Gfx.color(223, 150, 28)}, 0,0,0,    180,30,0,  6);
  //Fonts.drawString("Test Blink", Fonts.Face.YELLOW_SMALL, 50, 50, Fonts.Align.LEFT, blink);
}

bool MainView::mouseReleased(u16 x, u16 y, MouseButton b)
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
      if (a)
      {
        bool couldBeSelected = (!c || (c && !a->isPatrolling()));
        
        if (couldBeSelected && a->getOwner() == player)
        {
          switchToUnitSelection(a);
          return true;
        }
        else if (a->getOwner() != player)
        {
          gvm->armyView()->open(a);
          return true;
        }
      }
      
      
      if (c)
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
        const unit_list& selectedUnits = player->getSelectedUnits();;
        const auto& route = player->getRoute();
        const bool hasSelectedUnits = !selectedUnits.empty();
        
        if (hasSelectedUnits && !route)
          player->computeRoute(t->position);
        else if (hasSelectedUnits && !route->completed() && route->dx() == t->x() && route->dy() == t->y() && player->selectedAvailMoves() > 0)
        {
          player->consumeRoute();
          //player->push(new anims::UnitMovement(player, army, army->getRoute()->pendingPositions()));

          updateBuildButton();
        }
        else if (hasSelectedUnits)
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
  
  return true;
}

bool MainView::mouseMoved(u16 x, u16 y, MouseButton b)
{
  //if (substate == SURVEYOR)
  {
    const Position pos = Viewport::hoveredPosition(g->world, player, x, y);
    Tile* tile = g->world->get(pos);
    if (tile && tile != hoveredTile) surveyor.updateInfo(tile);
    hoveredTile = tile;
  }
  
  return true;

}

bool MainView::keyPressed(KeyboardCode key, KeyboardKey kkey, KeyboardMod mod)
{
  switch (key)
  {
    case SDL_SCANCODE_RETURN: buttons[NEXT]->press(); break;
    case SDL_SCANCODE_GRAVE: gvm->switchOverview(VIEW_CONSOLE);
      
    default: break;
  }
  
  return true;

}

bool MainView::keyReleased(KeyboardCode key, KeyboardKey kkey, KeyboardMod mod)
{
  switch (key)
  {
    case SDL_SCANCODE_RETURN:
    {
      buttons[NEXT]->release();
      buttons[NEXT]->click();
    }
    case SDL_SCANCODE_F1: switchToSurveyor(); break;
    case SDL_SCANCODE_F2: gvm->switchView(VIEW_CARTOGRAPHER); break;
    case SDL_SCANCODE_F3: gvm->switchView(VIEW_RESEARCH); break;
    case SDL_SCANCODE_F4: gvm->switchView(VIEW_HISTORIAN); break;
    case SDL_SCANCODE_F5: gvm->switchOverview(VIEW_ASTROLOGER); break;
    case SDL_SCANCODE_F9: gvm->switchOverview(VIEW_MIRROR); break;
      
    default: break;

  }
  
  return true;

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
    else if (t->type == TileType::OCEAN || t->type == TileType::SHORE)
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
        
        const std::string pop = fmt::sprintf(i18n::s(I18::SURVEYOR_MAX_POPULATION), city->getMaxPopulation());
        const std::string prod = fmt::sprintf(i18n::s(I18::SURVEYOR_MAX_POPULATION), (view.g->cityMechanics.computeProductionBonus(city)*100));
        
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
        Fonts::setPalette(i == 0 ? FontFaces::Palettes::SMALL_YELLOW_PALE : FontFaces::Palettes::SMALL_WHITE_PALE);
        Fonts::drawString(t[i], 274, 90+7*i, ALIGN_CENTER);
      }
    }
  }
}
