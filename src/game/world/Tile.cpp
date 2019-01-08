#include "Tile.h"

#include "World.h"
#include "City.h"

#include "LocalPlayer.h"
#include "Game.h"

void Tile::settleCity(City* city)
{
  this->city = city;
  
  for_each_neighbor_and_itself([city] (Tile* tile) {
    switch (tile->type) {
      case TileType::OCEAN:
      case TileType::SHORE: city->setPlacement(CITY_BY_SEA); break;
      case TileType::RIVER: city->setPlacement(CITY_BY_RIVER); break;
      case TileType::HILL: city->setPlacement(CITY_BY_HILL); break;
      case TileType::MOUNTAIN: city->setPlacement(CITY_BY_MOUNTAIN); break;
      default: break;
    }
  });
}

void Tile::placeArmy(Army* army)
{
  if (this->army && this->army->getOwner() == army->getOwner() && this->army->size() + army->size() <= 9)
  {
    this->army->merge(army);
    delete army;
    
    // since armies are merged we need to update the current selected army of the player
    LocalGame::i->currentPlayer()->selectArmy(this->army);
  }
  else
  {
    army->setPosition(position);
    this->army = army;
  }
}

static const std::array<DirJoin, 8> dirs = { DirJoin::N, DirJoin::NE, DirJoin::E, DirJoin::SE, DirJoin::S, DirJoin::SW, DirJoin::W, DirJoin::NW };

Tile* Tile::neighbor(DirJoin dir) const
{
  switch (dir)
  {
    case DirJoin::N: return world->get(position.dx(0,-1));
    case DirJoin::NE: return world->get(position.dx(+1,-1));
    case DirJoin::E: return world->get(position.dx(+1,0));
    case DirJoin::SE: return world->get(position.dx(+1,+1));
    case DirJoin::S: return world->get(position.dx(0,+1));
    case DirJoin::SW: return world->get(position.dx(-1,+1));
    case DirJoin::W: return world->get(position.dx(-1,0));
    case DirJoin::NW: return world->get(position.dx(-1,-1));
    default: assert(false);
  }
}

void Tile::for_each_neighbor_and_itself(const std::function<void(Tile*)> lambda)
{
  lambda(this);
  for_each_neighbor(lambda);
}

void Tile::for_each_neighbor(const std::function<void(Tile*)> lambda) const
{
  for (DirJoin dir : dirs)
    lambda(neighbor(dir));
}

DirJoin Tile::computeMask(const std::function<bool(const Tile*)> predicate) const
{
  DirJoin mask = DirJoin::NONE;
  for (DirJoin dir : dirs)
    if (predicate(neighbor(dir)))
      mask |= dir;

  return mask;
}
