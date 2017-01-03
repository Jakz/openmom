#include <iostream>

#include "SDL.h"

#include "SDLHelper.h"
#include "Gfx.h"
#include "Texture.h"
#include "Game.h"
#include "ViewManager.h"

#include "Font.h"
#include "LBX.h"

#include "UnitDetailView.h"

//#include "Platform.h"
//#include "yaml-cpp/yaml.h"

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
  /*YAML::Node file = YAML::LoadFile(Platform::instance()->getResourcePath()+"/data/yaml/test.yaml");
  
  auto skills = file["skills"];
  
  for (auto skill : skills)
  {
    std::cout << skill["name"] << std::endl;
  }*/
  
  
  lbx::Repository::init();
  lbx::Repository::loadLBX(LBXID::ARMYLIST);
  lbx::Repository::loadLBX(LBXID::BACKGRND);
  lbx::Repository::loadLBX(LBXID::CITYSCAP);
  lbx::Repository::loadLBX(LBXID::CMBTCITY);
  lbx::Repository::loadLBX(LBXID::FIGURES3);
  lbx::Repository::loadLBX(LBXID::FIGURES4);
  lbx::Repository::loadLBX(LBXID::FIGURE10);
  lbx::Repository::loadLBX(LBXID::FIGURE11);
  lbx::Repository::loadLBX(LBXID::FIGURE12);
  lbx::Repository::loadLBX(LBXID::FONTS);
  lbx::Repository::loadLBX(LBXID::ITEMS);
  lbx::Repository::loadLBX(LBXID::HELP);
  lbx::Repository::loadLBX(LBXID::HIRE);
  lbx::Repository::loadLBX(LBXID::LILWIZ);
  lbx::Repository::loadLBX(LBXID::MAGIC);
  lbx::Repository::loadLBX(LBXID::MAIN);
  lbx::Repository::loadLBX(LBXID::MAPBACK);
  lbx::Repository::loadLBX(LBXID::MONSTER);
  lbx::Repository::loadLBX(LBXID::NEWGAME);
  lbx::Repository::loadLBX(LBXID::PORTRAIT);
  lbx::Repository::loadLBX(LBXID::RESOURCE);
  lbx::Repository::loadLBX(LBXID::SCROLL);
  lbx::Repository::loadLBX(LBXID::SPECFX);
  lbx::Repository::loadLBX(LBXID::SPELLS);
  lbx::Repository::loadLBX(LBXID::SPELLSCR);
  lbx::Repository::loadLBX(LBXID::UNITVIEW);
  lbx::Repository::loadLBX(LBXID::UNITS1);
  lbx::Repository::loadLBX(LBXID::UNITS2);
  lbx::Repository::loadLBX(LBXID::WIZLAB);
  lbx::LBX::load();
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
  
  //SDL_SaveBMP(Texture::get(TextureID::FONT_YELLOW_SMALL).img, "antania.bmp");

  SDL::gvm->switchView(VIEW_MAIN);

  //SDL::gvm->switchView(VIEW_ITEM_CRAFT);
  
  /*Hero* unit = new Hero(*UnitSpec::heroSpec(UnitID::HERO_DWARF));
  SDL::gvm->unitDetailView()->setHeroHire(unit, 200);
  SDL::gvm->switchOverview(VIEW_UNIT);*/
  
  //SDL::gvm->switchView(VIEW_COMBAT);
  //SDL::gvm->switchView(VIEW_LBX);
  
  SDL::loop();
  
  // clean up
  Texture::unload();
  Gfx::deinit();
  SDL::deinit();
  
  return 0;
}
