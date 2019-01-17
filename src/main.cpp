#include <iostream>

#define SDL_MAIN_HANDLED
#include "SDL.h"

#include "SDLHelper.h"
#include "Gfx.h"
#include "Texture.h"
#include "Game.h"
#include "ViewManager.h"

#include "Font.h"
#include "LBXRepository.h"
#include "Viewport.h"

#include "YAML.h"

#include "ui/views/UnitDetailView.h"

#include "common/fmt/format.h"

void saveScreenshots(Game* game);

int main(int argc, char* arg[])
{
  yaml::parse();
  //return 0;
  
  lbx::Repository::init();
  lbx::Repository::loadLBX(LBXID::ARMYLIST);
  lbx::Repository::loadLBX(LBXID::BACKGRND);
  lbx::Repository::loadLBX(LBXID::CITYSCAP);
  
  lbx::Repository::loadLBX(LBXID::CITYWALL);
  lbx::Repository::loadLBX(LBXID::CMBTCITY);
  lbx::Repository::loadLBX(LBXID::CMBDESRC);
  lbx::Repository::loadLBX(LBXID::CMBDESRT);
  lbx::Repository::loadLBX(LBXID::CMBGRASC);
  lbx::Repository::loadLBX(LBXID::CMBGRASS);
  lbx::Repository::loadLBX(LBXID::CMBMOUNC);
  lbx::Repository::loadLBX(LBXID::CMBMOUNT);
  lbx::Repository::loadLBX(LBXID::CMBTUNDR);
  lbx::Repository::loadLBX(LBXID::CMBTUNDC);
  lbx::Repository::loadLBX(LBXID::CMBMAGIC);
  lbx::Repository::loadLBX(LBXID::CMBTFX);
  lbx::Repository::loadLBX(LBXID::COMPIX);
  lbx::Repository::loadLBX(LBXID::CONQUEST);

  lbx::Repository::loadLBX(LBXID::DIPLOMAC);

  lbx::Repository::loadLBX(LBXID::FIGURES1);
  lbx::Repository::loadLBX(LBXID::FIGURES3);
  lbx::Repository::loadLBX(LBXID::FIGURES4);
  lbx::Repository::loadLBX(LBXID::FIGURES5);
  lbx::Repository::loadLBX(LBXID::FIGURES8);
  lbx::Repository::loadLBX(LBXID::FIGURES9);
  lbx::Repository::loadLBX(LBXID::FIGURE10);
  lbx::Repository::loadLBX(LBXID::FIGURE11);
  lbx::Repository::loadLBX(LBXID::FIGURE12);
  lbx::Repository::loadLBX(LBXID::FONTS);
  lbx::Repository::loadLBX(LBXID::HELP);
  lbx::Repository::loadLBX(LBXID::HIRE);
  lbx::Repository::loadLBX(LBXID::INTRO);
  lbx::Repository::loadLBX(LBXID::ITEMS);
  lbx::Repository::loadLBX(LBXID::ITEMISC);
  lbx::Repository::loadLBX(LBXID::LILWIZ);
  lbx::Repository::loadLBX(LBXID::LOAD);
  lbx::Repository::loadLBX(LBXID::MAGIC);
  lbx::Repository::loadLBX(LBXID::MAIN);
  lbx::Repository::loadLBX(LBXID::MAINSCRN);
  lbx::Repository::loadLBX(LBXID::MAPBACK);
  lbx::Repository::loadLBX(LBXID::MONSTER);
  lbx::Repository::loadLBX(LBXID::MOODWIZ);
  lbx::Repository::loadLBX(LBXID::NEWGAME);
  lbx::Repository::loadLBX(LBXID::PORTRAIT);
  lbx::Repository::loadLBX(LBXID::RELOAD);
  lbx::Repository::loadLBX(LBXID::RESOURCE);
  lbx::Repository::loadLBX(LBXID::SCROLL);
  lbx::Repository::loadLBX(LBXID::SPECFX);
  lbx::Repository::loadLBX(LBXID::SPECIAL);
  lbx::Repository::loadLBX(LBXID::SPECIAL2);
  lbx::Repository::loadLBX(LBXID::SPELLS);
  lbx::Repository::loadLBX(LBXID::SPELLSCR);
  lbx::Repository::loadLBX(LBXID::UNITVIEW);
  lbx::Repository::loadLBX(LBXID::UNITS1);
  lbx::Repository::loadLBX(LBXID::UNITS2);
  lbx::Repository::loadLBX(LBXID::WIZLAB);
  lbx::Repository::loadLBX(LBXID::WIZARDS);
  
  Viewport::createMapTextureAtlas();
  
  FontFaces::buildFonts();
  
  Game *game = new Game();
  game->dummyInit();
  
  SDL::init();
  Gfx::init();
  Texture::load();
  SDL::initGVM();
  
  //saveScreenshots(game);

  //SDL::gvm->switchView(VIEW_MAP_EDITOR);
  SDL::gvm->switchView(VIEW_NEW_GAME);

  //SDL::gvm->switchView(VIEW_NEW_GAME);
  
  /*Hero* unit = new Hero(*UnitSpec::heroSpec(UnitID::HERO_DWARF));
  SDL::gvm->unitDetailView()->setHeroHire(unit, 200);*/
  //SDL::gvm->switchView(VIEW_NEW_GAME);
  
  //SDL::gvm->switchView(VIEW_DIPLOMACY);
  //SDL::gvm->switchView(VIEW_LBX);
  
  SDL::loop();
  
  // clean up
  Texture::unload();
  Gfx::deinit();
  SDL::deinit();
  
  return 0;
}

#include "Player.h"
#include "LocalPlayer.h"
#include "ui/views/CityView.h"
#include "ui/views/ArmyView.h"
#include "ui/views/MessageView.h"
#include "ui/views/ProductionView.h"
#include "ui/views/OutpostView.h"

#include "Army.h"
#include "format.h"

const char* viewName(ViewID id)
{
  switch (id)
  {
    case VIEW_CITY: return "city";
    case VIEW_UNIT: return "unit";
    case VIEW_MAIN: return "main";
    case VIEW_SPELL_BOOK: return "spellbook";
    case VIEW_MAGIC: return "magic";
    case VIEW_RESEARCH: return "research";
    case VIEW_ALCHEMY: return "alchemy";
    case VIEW_ARMIES: return "armies";
    case VIEW_ARMIES_ITEMS: return "armies-items";
    case VIEW_ITEM_CRAFT: return "item-craft";
    case VIEW_ITEM_CRAFT_CHARGES: return "item-craft-charges";
    case VIEW_MERCHANT: return "merchant";
    case VIEW_ARMY: return "army";
    case VIEW_MESSAGE: return "message";
    case VIEW_CITIES: return "cities";
    case VIEW_MIRROR: return "mirror";
    case VIEW_INFO_MENU: return "info-menu";
    case VIEW_CARTOGRAPHER: return "cartographer";
    case VIEW_DIPLOMACY: return "diplomacy";
    case VIEW_ASTROLOGER: return "astrologer";
    case VIEW_HISTORIAN: return "historian";
    case VIEW_COMBAT: return "combat";
    case VIEW_PRODUCTION: return "production";
    case VIEW_OUTPOST: return "outpost";
    case VIEW_NEW_GAME: return "new-game";
    case VIEW_LOAD: return "load";
    case VIEW_OPTIONS: return "options";
    case VIEW_START: return "start";
    case VIEW_INTRO: return "intro";
      
    case VIEW_DATA:
    case VIEW_CONSOLE:
    case VIEW_MAP_EDITOR:
    case VIEW_COUNT:
      return "";
  }
}

void saveScreenshots(Game* game)
{
  for (int i = VIEW_MAIN; i <= VIEW_INTRO; ++i)
  {
    if (i == VIEW_CITY)
      SDL::gvm->cityView()->setCity(*game->getCities().begin());
    else if (i == VIEW_UNIT)
    {
      Player* player = *game->getPlayers().begin();
      Army* army = *player->getArmies().begin();
      Unit* unit = *army->getUnits().begin();
      SDL::gvm->unitDetailView()->setUnit(unit);
    }
    else if (i == VIEW_ARMY)
    {
      Player* player = *game->getPlayers().begin();
      Army* army = *player->getArmies().begin();
      SDL::gvm->armyView()->setArmy(army);
    }
    else if (i == VIEW_MESSAGE)
    {
      Player* player = *game->getPlayers().begin();
      player->send(new msgs::Error("Error Message Test"));
    }
    else if (i == VIEW_PRODUCTION)
    {
      SDL::gvm->productionView()->setCity(*game->getCities().begin());
    }
    else if (i == VIEW_OUTPOST)
    {
      SDL::gvm->outpostView()->setCity(*game->getCities().begin());
    }

    SDL::gvm->switchView((ViewID)i);
    //SDL::gvm->cityView()->setCity(SDL::gvm->cityView()->)
    SDL::render();
    Gfx::saveScreenshot(fmt::sprintf("screenshot-%s.png", viewName((ViewID)i)).c_str());
  }
}
