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

#include "SDL.h"

class ViewManager;

class SDL
{
  private:
    static SDL_Window *window;
    static SDL_Renderer *renderer;
    static SDL_Texture *screen;

    static SDL_Surface *filter;
  
    static const u32 SCALE_FACTOR = 3;
    static const u32 FRAME_RATE = 20;
    static constexpr float TICKS_PER_FRAME = 1000 / (float)FRAME_RATE;
    
    static bool willQuit;
  
    static u32 fticksr;
  
  public:
    static bool init();
    static void initGVM();
    static void deinit();
    
    static void loop();
    static void render();
    static void handleEvents();
  
    static void capFPS();
  
    static ViewManager *gvm;
};

#endif
