#include "World.h"

#include "GfxData.h"

void World::calcSubTile(u16 x, u16 y, Plane p)
{
  Tile* t = get(x,y,p);
  
  t->variant = 0;
  t->joinMask = DirJoin::NONE;
  
  switch (t->type) {
    case TILE_GRASS:
      t->variant = Util::randi(TILE_COUNT_GRASSLANDS);
      break;
      
    case TILE_WATER:
    case TILE_SHORE:
    {
      DirJoin mask = t->computeMask([](const Tile* tile) { return tile && tile->isSolidLand(); });
      
      if (mask == DirJoin::NONE)
        t->type = TILE_WATER;
      else
        t->type = TILE_SHORE;
      
      t->joinMask = mask;
      
      /* if tile is ocean then 20% chance it's animated texture */
      if (t->type == TILE_WATER && Util::chance(0.20f))
        t->variant = 1;
      else
        t->variant = 0;
      
      break;
    }
      
    case TILE_DESERT:
    case TILE_TUNDRA:
    {
      const u32 count = t->type == TILE_DESERT ? TILE_COUNT_DESERT : TILE_COUNT_TUNDRA;
      
      /* compute mask to see which border tile should be used */
      const TileType type = t->type;
      t->joinMask = t->computeMask([type](const Tile* tile) { return tile && tile->type != type; });
      
      /* if mask is none then it's a full tile, choose a variant */
      if (t->joinMask == DirJoin::NONE)
        t->variant = Util::randi(count);
      
      break;
    }

    case TILE_MOUNTAIN:
    case TILE_HILL:
    {
      const TileType type = t->type;
      /* first we compute the normal 8 directional mask to see where mountain/hills are */
      t->joinMask = t->computeMask([type](const Tile* tile) { return tile && tile->type == type; });
      /* then we are interested just in orthogonal directions so we map the 8 bit mask to a 4 bit orthogonal
         and use it as index for the texture (check operator! of DirJoin to see the behavior */
      t->variant = !t->joinMask;
      break;
    }
      
    case TILE_SWAMP:
      t->variant = Util::randi(TILE_COUNT_SWAMP);
      break;
      
    case TILE_FOREST:
      t->variant = Util::randi(TILE_COUNT_FOREST);
      break;
      
    case TILE_VOLCANO:
      break;
  }
}
