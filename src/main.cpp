#include <iostream>

#include <SDL2/SDL.h>

#include "Gfx.h"
#include "Font.h"
#include "Upkeep.h"
#include "Race.h"
#include "Texture.h"

#include <unordered_map>

struct SpriteInfo
{
  u16 x, y;
};

constexpr long double operator"" _deg ( long double deg )
{
  return deg*3.141592/180;
}


class SDL
{
  private:
    static SDL_Window *window;
    static SDL_Renderer *renderer;
    static SDL_Texture *screen;
  
  public:
    static bool init();
    static void deinit();
  
    static void render();
  
};

SDL_Window* SDL::window = nullptr;
SDL_Renderer* SDL::renderer = nullptr;
SDL_Texture* SDL::screen = nullptr;

bool SDL::init()
{
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    return false;
  
  window = SDL_CreateWindow("OpenMoM v0.01a",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 400, SDL_WINDOW_OPENGL);
  renderer = SDL_CreateRenderer(window, -1, 0);
  
  screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 320, 200);
  
  return true;
}

void SDL::deinit()
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyTexture(screen);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void SDL::render()
{
  Gfx::draw(MAIN_BACKDROP, 0, 0);
  Fonts::drawString("Population:", YELLOW_SMALL, 20, 20, ALIGN_LEFT);
  
  SDL_Surface *canvas = Gfx::getCanvas();
  SDL_UpdateTexture(screen, nullptr, canvas->pixels, canvas->pitch);


  
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, screen, nullptr, nullptr);
  SDL_RenderPresent(renderer);
  
  SDL_Delay(1600000);
}


int main(int argc, char * arg[])
{
  const Race &race = Race::race(RACE_BARBARIANS);
  
  std::unordered_map<const void*, SpriteInfo> map;
  map[reinterpret_cast<const Race *>(&race)] = {15,15};
  
  //const Race* ptRace = reinterpret_cast<const Race *>(&race);
  
  // init SDL
  SDL::init();
  Gfx::init();
  Texture::load();

  
  while (true)
  {
    SDL::render();
  }
  
  
  // wait 5 secs
  
  // clean up
  Texture::unload();
  Gfx::deinit();
  SDL::deinit();
  
  return 0;
}