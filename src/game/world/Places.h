#ifndef _PLACES_H_
#define _PLACES_H_

#include "Common.h"

#include <vector>

class Player;
class Army;

class ManaNode
{
  public:
    const School school;
    s16 mana;
  
    Player* owner;
    std::vector<Position> auras;
  
    Army* guards;
  
    ManaNode(School school) : school(school), owner(nullptr), guards(nullptr), mana(0) { }
};

class Place
{
  public:
    PlaceType type;
    Army *army;
    bool isWeak;
  
    Place(PlaceType type, bool isWeak) : type(type), isWeak(isWeak), army(nullptr) { }
};





class TileGfxSpec
{
  private:
    static const TileGfxSpec specs[];
  
    const std::vector<Color> colors[PLANE_COUNT];

  public:
    TileGfxSpec(u8 row, u8 count, Color c1, Color c2) : row(row), count(count), colors{{c1},{c2}} { }
    TileGfxSpec(u8 row, u8 count, std::initializer_list<Color> c1, std::initializer_list<Color> c2) : row(row), count(count), colors{c1,c2} { }

  
    u8 row;
    u8 count;
  
    Color minimapColor(Plane plane, u16 i = 0)
    {
      if (colors[plane].size() > 1)
        return colors[plane][i];
      else
        return colors[plane][0];
    }
  
  static const TileGfxSpec& spec(TileType type);
};

#endif
