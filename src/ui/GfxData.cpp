//
//  GfxData.cpp
//  OpenMoM
//
//  Created by Jack on 7/21/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "GfxData.h"

#include "Gfx.h"

static const Color schoolColors[] = {
  Gfx::color(0,0,0),
  Gfx::color(255, 0, 0),
  Gfx::color(0, 0, 0),
  Gfx::color(255, 255, 255),
  Gfx::color(0, 255, 0),
  Gfx::color(0, 0, 255)
};

const TileGfxSpec GfxData::specs[] =
{
  TileGfxSpec(0,6,RGB(56,94,18),RGB(81,60,48)), // GRASS
  TileGfxSpec(0,0,{RGB(0, 14, 90),RGB(7, 25, 95)},{RGB(8,4,4),RGB(36,28,24)}), // WATER
  TileGfxSpec(0,0,{RGB(0, 14, 90),RGB(7, 25, 95)},{RGB(8,4,4),RGB(36,28,24)}), // SHORE
  TileGfxSpec(0,0,{RGB(105,96,93),RGB(126, 116, 113)},{RGB(255, 255, 255)}), // MOUNTAIN
  TileGfxSpec(0,0,{RGB(105,96,93),RGB(126, 116, 113)},{RGB(255, 255, 255)}), // VOLCANO
  TileGfxSpec(0,0,RGB(53,92,17),RGB(115,83,69)), // HILL
  TileGfxSpec(1,3,RGB(0, 94, 19),RGB(0, 94, 19)),  // FOREST
  TileGfxSpec(2,3,RGB(56,47,43),RGB(56,47,43)), // SWAMP
  TileGfxSpec(3,4,{RGB(190,154,117),RGB(142,113,89)},{RGB(255,255,255)}), // DESERT
  TileGfxSpec(4,3,RGB(56,47,43),RGB(255,255,255)), // TUNDRA
  TileGfxSpec(0,0,RGB(255, 255, 255),RGB(255, 255, 255)), // RIVER
  TileGfxSpec(0,0,{RGB(0, 14, 90),RGB(7, 25, 95)},{RGB(8,4,4),RGB(36,28,24)}) // RIVER_MOUTH TODO: check colors
};


Color GfxData::colorForSchool(const School school)
{
  return school < SCHOOL_COUNT ? schoolColors[school] : 0;
}

