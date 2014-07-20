#include "Tile.h"

#include "World.h"
#include "City.h"

void Tile::settleCity(City* city)
{
  this->city = city;
  
  for (u16 i = 0;  i < Util::DIRS_LENGTH+1; ++i)
  {
    Tile* tile = i < 8 ? world->get(position, Util::DIRS[i]) : this;
    
    if (tile)
    {
      switch (tile->type) {
        case TILE_WATER:
        case TILE_SHORE: city->setPlacement(CITY_BY_SEA); break;
        case TILE_RIVER: city->setPlacement(CITY_BY_RIVER); break;
        case TILE_HILL: city->setPlacement(CITY_BY_HILL); break;
        case TILE_MOUNTAIN: city->setPlacement(CITY_BY_MOUNTAIN); break;
        default: break;
      }
    }
  }
}