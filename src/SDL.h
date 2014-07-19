//
//  SDL.h
//  OpenMoM
//
//  Created by Jack on 7/19/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _SDL_H_
#define _SDL_H_

#include "Common.h"

#include <SDL2/SDL.h>

class SDL
{
  private:
    static SDL_Window *window;
    static SDL_Renderer *renderer;
    static SDL_Texture *screen;
    
    static const u32 FRAME_RATE = 60;
    static constexpr float TICKS_PER_FRAME = 1000 / (float)FRAME_RATE;
    
    static bool willQuit;
    
  public:
    static bool init();
    static void deinit();
    
    static void loop();
    static void render();
    
    static void capFPS();
    
    static u32 ticks;
  
};

#endif
