//
//  SDL.cpp
//  OpenMoM
//
//  Created by Jack on 7/19/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "SDLHelper.h"

#include "Font.h"
#include "Gfx.h"

#include "Hqx.h"

#include "ViewManager.h"

//#define HQXFILTER

SDL_Window* SDL::window = nullptr;
SDL_Renderer* SDL::renderer = nullptr;
SDL_Texture* SDL::screen = nullptr;

SDL_Surface* SDL::filter = nullptr;

u32 SDL::animTicksCounter = 0;

bool SDL::willQuit = false;

ViewManager* SDL::gvm = nullptr;

bool SDL::init()
{
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    return false;
  
  window = SDL_CreateWindow("OpenMoM v0.01a",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH*SCALE_FACTOR, HEIGHT*SCALE_FACTOR, SDL_WINDOW_OPENGL);
  renderer = SDL_CreateRenderer(window, -1, 0);
  
#ifndef HQXFILTER
  screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
#else
  screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH*SCALE_FACTOR, HEIGHT*SCALE_FACTOR);
  filter = Gfx::createSurface(WIDTH*SCALE_FACTOR, HEIGHT*SCALE_FACTOR);
#endif
  
  /*u16 *r = new u16[256];
  u16 *g = new u16[256];
  u16 *b = new u16[256];
  
  SDL_GetWindowGammaRamp(window, r, g, b);
  for (int i = 0; i < 256; ++i)
    printf("%d %d %d %d\n", r[i], g[i], b[i], 257*i);*/
  
  animTicksCounter = 0;

  return true;
}

void SDL::initGVM()
{
  gvm = new ViewManager();
}

void SDL::loop()
{
  while (!willQuit)
  {
    render();
    handleEvents();
    
    capFPS();
  }
}

char titleBuffer[64];

void SDL::capFPS()
{
  u32 ticks = SDL_GetTicks();
  u32 elapsed = ticks - Gfx::ticks;

  //printf("Ticks: %u, waiting %f ticks, aticks: %u\n", elapsed, TICKS_PER_FRAME - elapsed, Gfx::fticks);
  
  u32 frameTime = elapsed;
  
  if (elapsed < TICKS_PER_FRAME)
  {
    SDL_Delay(TICKS_PER_FRAME - elapsed);
    frameTime = TICKS_PER_FRAME;
  }
  
  if (++animTicksCounter == FRAMES_PER_ANIM_TICK)
  {
    ++Gfx::fticks;
    animTicksCounter = 0;
  }
    
  sprintf(titleBuffer, "OpenMoM v0.01 (%2.2f)", 1000.0f/fmax(elapsed, TICKS_PER_FRAME));
  SDL_SetWindowTitle(window, titleBuffer);
  
  Gfx::ticks = SDL_GetTicks();
}

void SDL::deinit()
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyTexture(screen);
  SDL_DestroyWindow(window);
  
#ifdef HQXFILTER
  SDL_FreeSurface(filter);
#endif
  
  SDL_Quit();
}

bool buttonDown = false;
MouseButton lastButton = BUTTON_LEFT;
void SDL::handleEvents()
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
      case SDL_QUIT: willQuit = true; break;
      
      case SDL_MOUSEBUTTONDOWN:
      {
        gvm->mousePressed(event.button.x/SCALE_FACTOR, event.button.y/SCALE_FACTOR, static_cast<MouseButton>(event.button.button));
        buttonDown = true;
        lastButton = static_cast<MouseButton>(event.button.button);
        break;
      }
      case SDL_MOUSEBUTTONUP:
      {
        gvm->mouseReleased(event.button.x/SCALE_FACTOR, event.button.y/SCALE_FACTOR, static_cast<MouseButton>(event.button.button));
        buttonDown = false;
        break;
      }
      case SDL_MOUSEMOTION:
      {
        if (buttonDown)
          gvm->mouseDragged(event.button.x/SCALE_FACTOR, event.button.y/SCALE_FACTOR, lastButton);
        else
          gvm->mouseMoved(event.button.x/SCALE_FACTOR, event.button.y/SCALE_FACTOR, BUTTON_LEFT);
        break;
      }
      
      case SDL_KEYDOWN:
      {
        gvm->keyPressed(event.key.keysym.scancode, event.key.keysym.sym, static_cast<KeyboardMod>(event.key.keysym.mod));
        break;
      }
        
      case SDL_KEYUP:
      {
        gvm->keyReleased(event.key.keysym.scancode, event.key.keysym.sym, static_cast<KeyboardMod>(event.key.keysym.mod));
        break;
      }
        
      case SDL_MOUSEWHEEL:
      {
        gvm->mouseWheel(event.wheel.x, event.wheel.y, 0);
        break;
      }
    }
  }
}

void SDL::render()
{
  SDL_FillRect(Gfx::getCanvas(), nullptr, 0x00000000);
  gvm->draw();
  
#ifndef HQXFILTER
  SDL_Surface *canvas = Gfx::getCanvas();
  SDL_UpdateTexture(screen, nullptr, canvas->pixels, canvas->pitch);  
#else
  SDL_Surface *canvas = Gfx::getCanvas();
  //Gfx::lock(canvas);
  //Gfx::lock(filter);
  hq4x_32(static_cast<u32*>(canvas->pixels), static_cast<u32*>(filter->pixels), WIDTH, HEIGHT);
  //Gfx::unlock(canvas);
  //Gfx::unlock(filter);
  SDL_UpdateTexture(screen, nullptr, filter->pixels, filter->pitch);
  
#endif
  
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, screen, nullptr, nullptr);
  SDL_RenderPresent(renderer);
}
