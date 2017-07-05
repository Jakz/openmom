//
//  World.h
//  OpenMoM
//
//  Created by Jack on 7/20/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _WORLD_H_
#define _WORLD_H_

#include "common/Util.h"
#include "Tile.h"
#include "Pathfind.h"

class Game;

class GenerableWorld
{
public:
  virtual s16 width() const = 0;
  virtual s16 height() const = 0;
  
  virtual void set(TileType type, s16 x, s16 y, Plane plane) = 0;
  virtual void rect(TileType type, s16 x, s16 y, u16 w, u16 h, Plane plane) = 0;
  virtual void line(TileType type, int k1, int k2, int j, bool vertical, Plane plane) = 0;
  virtual Tile* get(s16 x, s16 y, Plane plane) const = 0;
};

class World : public GenerableWorld
{
private:
  Tile** map;
  
public:
  World(Game* const game, u16 w, u16 h) : game(game), w(w), h(h), pathfinder(this, w, h)
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
  
  s16 width() const override { return w; }
  s16 height() const override { return h; }
  
  Tile* get(Position position) const
  {
    return get(position.x, position.y, position.plane);
  }
  
  Tile* get(s16 x, s16 y, Plane plane) const override
  {
		if (x < 0) x += w;
		else if (x >= w) x %= w;
		
		/* verical wrap unallowed
     if (y < 0)
     tY = h + y;
     else if (y >= h)
     tY = y % h;
     else
     tY = y;
     */
		if (y < 0)
			return nullptr;
		else if (y >= h)
			return nullptr;
		
		return &map[plane][x+y*w];
  }
  
  Tile *get(Position position, PositionOffset offset) const
  {
    return get(position.x+offset.x, position.y+offset.y, position.plane);
  }
  
  Tile *get(s16 x, s16 y, Plane plane, PositionOffset offset) const
  {
    return get(x+offset.x, y+offset.y, plane);
  }
  
  void fill(TileType type, Plane plane)
  {
    for (int x = 0; x < w*h; ++x)
      map[plane][x].type = type;
  }
  
  void set(TileType type, Position position) { get(position)->type = type; }
  void set(TileType type, s16 x, s16 y, Plane plane) override { get(x,y,plane)->type = type; }
  
  void rect(TileType type, s16 x, s16 y, u16 w, u16 h, Plane plane) override
	{
		for (int i = x; i < x+w; ++i)
			for (int j = y; j < y+h; ++j)
        get(i,j,plane)->type = type;
	}
  
  void line(TileType type, int k1, int k2, int j, bool vertical, Plane plane) override
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
  
  void calcSubTiles()
	{
		for (int i = 0; i < PLANE_COUNT; ++i)
			for (int x = 0; x < w; ++x)
				for (int y = 0; y < h; ++y)
					calcSubTile(x,y,static_cast<Plane>(i));
	}
  
  void calcSubTile(u16 x, u16 y, Plane p);
  
  pathfind::PathFinder pathfinder;
  
  Game* const game;
  const u16 w, h;
};

#endif
