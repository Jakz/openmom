#include <iostream>

#include <SDL2/SDL.h>

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


int main(int argc, char * arg[])
{
  const Race &race = Race::race(RACE_BARBARIANS);
  
  std::unordered_map<const void*, SpriteInfo> map;
  map[reinterpret_cast<const Race *>(&race)] = {15,15};
  
  //const Race* ptRace = reinterpret_cast<const Race *>(&race);
  
  // init SDL
  if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    std::cout << "ERROR SDL_Init" << std::endl;
    
    return -1;
  }
  
  // create a window
  SDL_Window * window = SDL_CreateWindow("SDL 2 window",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
  
  SDL_Surface *buffer = SDL_CreateRGBSurface(0, 640, 480, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
  SDL_Texture *screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 640, 480);
  
  while (true)
  {
    for (int i = 0; i < 640; ++i)
    {
      SDL_Rect r = {i,0,i,480};
      SDL_FillRect(buffer, &r, SDL_MapRGB(buffer->format, rand()%256, rand()%256, rand()%256));
    }
    
    
    SDL_UpdateTexture(screen, nullptr, buffer->pixels, buffer->pitch);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, screen, NULL, NULL);
    SDL_RenderPresent(renderer);
    
    
    SDL_Delay(16);
  }
  
  
  // wait 5 secs
  SDL_Delay(5000);
  
  // clean up
  SDL_DestroyWindow(window);
  SDL_Quit();
  
  return 0;
}