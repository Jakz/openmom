//
//  Common.h
//  OpenMoM
//
//  Created by Jack on 7/17/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _COMMON_H_
#define _COMMON_H_

#include <cstdint>
#include <SDL2/SDL.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;

typedef u32 Color;

enum I18 : u16;
enum TextureID : u16;

enum MouseButton : u8
{
  BUTTON_LEFT = SDL_BUTTON_LEFT,
  BUTTON_RIGHT = SDL_BUTTON_RIGHT,
  BUTTON_MIDDLE = SDL_BUTTON_MIDDLE
};

enum PlayerColor : u8
{
  GREEN = 0,
  BLUE,
  RED,
  PURPLE,
  YELLOW,
  NEUTRAL
};

struct SpriteInfo
{
  TextureID texture;
  s16 x;
  s16 y;
};

#endif
