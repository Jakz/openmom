#include <iostream>

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

#include "UnitDetailView.h"

/*
 Combat
 CombatAIThread
 CombatMechanics
 CombatMind
 CombatUnitAttack
 CombatUnitMovement
 CombatView
 CombatView.SubState
 GlobalSpellEffect
 SpellEffect
 SpellEffects
 Spells
*/

void init()
{
  Game *game = new Game();
  game->dummyInit();
  
  //Util::seed(time(nullptr));
  //WorldGenerator gen = WorldGenerator(game->world);
  //gen.generate();
}



int main(int argc, char * arg[])
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
  /*return 0;*/

  
  FontFaces::buildFonts();
  //return 0;
  
  //const Race &race = Race::race(RACE_BARBARIANS);
  
  //std::unordered_map<const void*, SpriteInfo> map;
  
  //const Race* ptRace = reinterpret_cast<const Race *>(&race);
  
  // init SDL
        
  init();
  
  SDL::init();
  Gfx::init();
  Texture::load();
  SDL::initGVM();
  
  SDL::gvm->switchView(VIEW_MAIN);

  //SDL::gvm->switchView(VIEW_NEW_GAME);
  
  /*Hero* unit = new Hero(*UnitSpec::heroSpec(UnitID::HERO_DWARF));
  SDL::gvm->unitDetailView()->setHeroHire(unit, 200);
  SDL::gvm->switchOverview(VIEW_UNIT);*/
  
  SDL::gvm->switchView(VIEW_DIPLOMACY);
  //SDL::gvm->switchView(VIEW_LBX);
  
  SDL::loop();
  
  // clean up
  Texture::unload();
  Gfx::deinit();
  SDL::deinit();
  
  return 0;
}
