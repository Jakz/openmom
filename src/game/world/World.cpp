#include "World.h"

#include "GfxData.h"

void World::calcSubTile(u16 x, u16 y, Plane p)
{
  Tile* t = get(x,y,p);
  
  t->gfx.variant = 0;
  t->gfx.joinMask = DirJoin::NONE;
  
  t->gfx.roadMask = t->computeMask([](const Tile* ntile) { return ntile && (ntile->hasRoad || ntile->city); });
  
  switch (t->type)
  {
    case TileType::GRASS:
      t->gfx.variant = Util::randi(TILE_COUNT_GRASSLANDS);
      break;
      
    case TileType::OCEAN:
    case TileType::SHORE:
    {
      DirJoin mask = t->computeMask([](const Tile* tile) { return tile && tile->isSolidLand(); });
      
      if (mask == DirJoin::NONE)
        t->type = TileType::OCEAN;
      else
        t->type = TileType::SHORE;
      
      t->gfx.joinMask = mask;
      
      /* if tile is ocean then 20% chance it's animated texture */
      if (t->type == TileType::OCEAN && Util::chance(0.20f))
        t->gfx.variant = 1;
      else
        t->gfx.variant = 0;
      
      break;
    }
      
    case TileType::DESERT:
    case TileType::TUNDRA:
    {
      const u32 count = t->type == TileType::DESERT ? TILE_COUNT_DESERT : TILE_COUNT_TUNDRA;
      
      /* compute mask to see which border tile should be used */
      const TileType type = t->type;
      t->gfx.joinMask = t->computeMask([type](const Tile* tile) { return tile && tile->type != type; });
      
      /* if mask is none then it's a full tile, choose a variant */
      if (t->gfx.joinMask == DirJoin::NONE)
        t->gfx.variant = Util::randi(count);
      
      break;
    }

    case TileType::MOUNTAIN:
    case TileType::HILL:
    {
      const TileType type = t->type;
      /* first we compute the normal 8 directional mask to see where mountain/hills are */
      t->gfx.joinMask = t->computeMask([type](const Tile* tile) { return tile && tile->type == type; });
      /* then we are interested just in orthogonal directions so we map the 8 bit mask to a 4 bit orthogonal
         and use it as index for the texture (check operator! of DirJoin to see the behavior */
      t->gfx.variant = !t->gfx.joinMask;
      break;
    }
      
    case TileType::SWAMP:
      t->gfx.variant = Util::randi(TILE_COUNT_SWAMP);
      break;
      
    case TileType::FOREST:
      t->gfx.variant = Util::randi(TILE_COUNT_FOREST);
      break;
      
    case TileType::VOLCANO:
      t->gfx.variant = 0;
      break;
      
    case TileType::RIVER:
    {
      /* we compute the normal 8 directional mask to see where rivers are */
      t->gfx.joinMask = t->computeMask([](const Tile* tile) { return tile && tile->type == TileType::RIVER; }) & DirJoin::OCROSS;
      /* TODO FIXME: use number of variants as specified in Viewport data */
      t->gfx.variant = 0;
      break;
    }
  }
}
