//
//  WorldGenerator.h
//  OpenMoM
//
//  Created by Jack on 7/25/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _WORLD_GENERATOR_H_
#define _WORLD_GENERATOR_H_

#include "common/Common.h"

#include "World.h"
#include "Game.h"

enum class IslandType : u8 { RANDOM = 0, LONELY, ARCHIPELAGO, MIXED };
enum class CoastWaters : u8 { RANDOM = 0, VARIED, VERY_CALM, CALM, NORMAL, WILD, VERY_WILD };
enum class CoastMode : u8 { RANDOM = 0, IRREGULAR, CHAOTIC, BLOBBY };
  
class AtlasMap
{
private:
  char *mapGrid;
  
  s32 W, H;
  bool wrapX, wrapY;
  CoastMode coastlineMode;

  s32 ISLAND_SIZE;
  
public:
  AtlasMap(s32 width, s32 height, char defaultValue);
  //~AtlasMap() { delete [] mapGrid; }
  void initialize(s32 w, s32 h, s32 continents, s32 continentSize, s32 islands, IslandType islandType, CoastWaters coastType, bool wrapX, bool wrapY, CoastMode coastMode);

  s32 adjacentType(s32,s32,char);
  s32 surroundingType(s32,s32,char,s32);

  void placeContinents(s32 count);
  void placeContinentSeed(s32,s32,s32);
  
  void placeIslands(s32,IslandType);
  void lonelyIslands(s32);
  void archipelago(s32,bool);
  void mixedIslands(s32);
  void coastalCorrection(CoastWaters);
  
  void cellCount4(s32,float,char,char,char);
  void cellCountRadius(s32,float,char,char,char,s32);
  
  void bays(s32);
  void removeIsles();
  float equatorDistance(s32);
  
  void print()
  {
    for (int y = 0; y < 40; ++y)
    {
      for (int x = 0; x < 60; ++x)
      {
        printf("%c", mapGrid[x+y*W]);
      }
      printf("\n");
    }
  }
  
  void fill(GenerableWorld* world, Plane plane)
  {
    for (int y = 0; y < 40; ++y)
    {
      for (int x = 0; x < 60; ++x)
      {
        world->set(get(x,y) == 'G' ? TILE_GRASS : TILE_WATER, x, y, plane);
      }
    }
  }
  
  inline char get(s32 x, s32 y) { return mapGrid[x+y*W]; }
  inline void set(s32 x, s32 y, char c) { mapGrid[x+y*W] = c; }

  inline s32 downX(s32 x)
  {
    if (x > 0) return x - 1;
    if (wrapX) return W - 1;
    return x;
  }
  
  inline s32 downX(s32 x, s32 d)
  {
    if (d < 0) return upX(x, -d);
    
    for (int i = 0; i < d; ++i)
      x = downX(x);
    return x;
  }
  
  inline s32 downY(s32 y)
  {
    if (y > 0) return y - 1;
    if (wrapY) return H - 1;
    return y;
  }
  
  inline s32 downY(s32 y, s32 d)
  {
    if (d < 0) return upY(y, -d);
    
    for (int i = 0; i < d; ++i)
      y = downY(y);
    return y;
  }
  
  inline s32 upX(s32 x)
  {
    if (x < W-1) return x + 1;
    if (wrapX) return 0;
    return W - 1;
  }
  
  inline s32 upX(s32 x, s32 d)
  {
    if (d < 0) return downX(x, -d);
    
    for (int i = 0; i < d; ++i)
      x = upX(x);
    return x;
  }
  
  inline s32 upY(s32 y)
  {
    if (y < H-1) return y + 1;
    if (wrapY) return 0;
    return H - 1;
  }
  
  inline s32 upY(s32 y, s32 d)
  {
    if (d < 0) return downY(y, -d);
    
    for (int i = 0; i < d; ++i)
      y = upY(y);
    return y;
  }
};

class WorldGenerator
{
private:
  GenerableWorld* world;
  MapMechanics* mapMech;
  s16 W, H;
  
  
  s32 quantities[TILE_TYPES];
  s32 totalTiles;
  
  
public:
  WorldGenerator(World *world) : world(world), W(world->width()), H(world->height()), mapMech(&world->game->mapMechanics), quantities{0}, totalTiles{0} { }
  
  void count(Plane plane);
  
  void makeAreas(Plane plane, TileType type, float perc, float expandO, float expandT, int limit);
	void makeChains(Plane plane, TileType type, float perc, float branchProb, int length);
  void makeSpots(Plane plane, TileType type, float perc);
  void makeTundraEdges(Plane plane);
	void makeNodes(Plane plane);
  void makeLairs();
  
  void atlasGenerate();
  
  void generate();

  
};

#endif
