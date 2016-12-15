#include <iostream>

#include "SDL.h"

#include "SDLHelper.h"
#include "Gfx.h"
#include "Texture.h"
#include "Game.h"
#include "ViewManager.h"

#include "Font.h"
#include "LBX.h"

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
  lbx::Repository::init();
  lbx::Repository::loadLBX(LBX_ARMYLIST);
  lbx::Repository::loadLBX(LBX_BACKGRND);
  lbx::Repository::loadLBX(LBX_FIGURE4);
  lbx::Repository::loadLBX(LBX_FIGURE10);
  lbx::Repository::loadLBX(LBX_FIGURE11);
  lbx::Repository::loadLBX(LBX_FIGURE12);
  lbx::LBX::load();
  FontFaces::buildFonts();
  //return 0;
  
  //const Race &race = Race::race(RACE_BARBARIANS);
  
  //std::unordered_map<const void*, SpriteInfo> map;
  //map[reinterpret_cast<const Race *>(&race)] = {MAIN_BACKDROP, 15,15};
  
  //const Race* ptRace = reinterpret_cast<const Race *>(&race);
  
  // init SDL
        
  init();
  
  SDL::init();
  Gfx::init();
  Texture::load();
  SDL::initGVM();
  
  //SDL_SaveBMP(Texture::get(TextureID::FONT_YELLOW_SMALL).img, "antania.bmp");

  SDL::gvm->switchView(VIEW_MAIN);
  //SDL::gvm->switchView(VIEW_COMBAT);
  //SDL::gvm->switchView(VIEW_LBX);
  
  SDL::loop();
  
  // clean up
  Texture::unload();
  Gfx::deinit();
  SDL::deinit();
  
  return 0;
}
