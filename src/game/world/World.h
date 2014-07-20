//
//  World.h
//  OpenMoM
//
//  Created by Jack on 7/20/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _WORLD_H_
#define _WORLD_H_

#include "Util.h"
#include "Tile.h"

class Game;

class World
{
private:
  Tile** map;
  
public:
  World(Game* const game, u16 w, u16 h) : game(game), w(w), h(h)
  {
    map = new Tile*[PLANE_COUNT];
    
    for (int i = 0; i < PLANE_COUNT; ++i)
    {
      map[i] = new Tile[w*h];
      for (int x = 0; x < w; ++x)
        for (int y = 0; y < h; ++y)
          map[i][x+y*w] = Tile(this,Position(x,y,static_cast<Plane>(i)));
    }
  }
  
  Tile* get(Position position)
  {
    return get(position.x, position.y, position.plane);
  }
  
  Tile* get(u16 x, u16 y, Plane plane)
  {
    int tX, tY;
		
		if (x < 0)
			tX = w + x;
		else if (x >= w)
			tX = x % w;
		else
			tX = x;
		
		/* verical wrap unallowed
     if (y < 0)
     tY = h + y;
     else if (y >= h)
     tY = y % h;
     else
     tY = y;
     */
		tY = y;
		if (y < 0)
			return nullptr;
		else if (y >= h)
			return nullptr;
		
		//System.out.println(x+" "+y);
		return &map[plane][tX+tY*w];
  }
  
  Tile *get(Position position, PositionOffset offset)
  {
    return get(position.x+offset.x, position.y+offset.y, position.plane);
  }
  
  Tile *get(u16 x, u16 y, Plane plane, PositionOffset offset)
  {
    return get(x+offset.x, y+offset.y, plane);
  }
  
  void fill(TileType type, Plane plane)
  {
    for (int x = 0; x < w*h; ++x)
      map[plane][x].type = type;
  }
  
  void tile(TileType type, Position position) { get(position)->type = type; }
  
  void rect(TileType type, u16 x, u16 y, u16 w, u16 h, Plane plane)
	{
		for (int i = x; i < x+w; ++i)
			for (int j = y; j < y+h; ++j)
        get(i,j,plane)->type = type;
	}
  
  void line(TileType type, int k1, int k2, int j, bool vertical, Plane plane)
	{
		if (vertical)
			for (int i = k1; i <= k2; ++i)
				get(i,j,plane)->type = type;
		else
			for (int i = k1; i <= k2; ++i)
				get(j,i,plane)->type = type;
	}
  
  void updateRoads()
  {
    for (int i = 0; i < PLANE_COUNT; ++i)
    {
      Plane p = static_cast<Plane>(i);
      
      for (int x = 0; x < w; ++x)
      {
				for (int y = 0; y < h; ++y)
        {
          u8 roads = 0;
          
          for (int i = 0; i < Util::DIRS_LENGTH; ++i)
          {
            Tile* tile = get(x,y,p,Util::DIRS[i]);
            if (tile && (tile->hasRoad || tile->city))
              roads |= (1 << i);
          }
          
          get(x,y,p)->roads = roads;
        }
      }
    }
  }
  
  void calcSubTile(u16 x, u16 y, Plane p);
  
  Game* const game;
  const u16 w, h;
};

#endif
