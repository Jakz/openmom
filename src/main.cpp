#include <iostream>

#include <SDL2/SDL.h>

#include "SDL.h"
#include "Gfx.h"
#include "Texture.h"

#include "Race.h"

#include <unordered_map>

#include "EventListener.h"





int main(int argc, char * arg[])
{
  const Race &race = Race::race(RACE_BARBARIANS);
  
  std::unordered_map<const void*, SpriteInfo> map;
  map[reinterpret_cast<const Race *>(&race)] = {MAIN_BACKDROP, 15,15};
  
  //const Race* ptRace = reinterpret_cast<const Race *>(&race);
  
  // init SDL
  SDL::init();
  Gfx::init();
  Texture::load();

  
  SDL::loop();
  
  
  // wait 5 secs
  
  // clean up
  Texture::unload();
  Gfx::deinit();
  SDL::deinit();
  
  return 0;
}