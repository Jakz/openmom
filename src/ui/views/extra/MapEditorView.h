//
//  CartographerView.h
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#pragma once

#include "common/Common.h"
#include "View.h"


class World;
class Tile;
class MiniMap;

struct Brush
{
  enum class Type
  {
    OCEAN,
    GRASSLANDS,
    DESERT,
    TUNDRA,
    HILLS,
    MOUNTAINS,
    FOREST,
    SWAMP,
    VOLCANO,
    RIVER,
    CHAOS_NODE,
    SORCERY_NODE,
    NATURE_NODE
  };
  
  Type type;
  SpriteInfo info;
  std::function<void(Tile*)> lambda;
};

class MapEditorView : public ViewWithQueue
{
private:
  enum class Mode
  {
    TERRAIN
  };
  
  World* world;
  MiniMap* minimap;
  
  void draw() override;
  void drawPost() override { }
  
  void setup();
  
  std::array<Brush, 13>::const_iterator brush;
  Point hover;
  Point offset;
  Plane plane;
  Mode mode;
  
  Point positionForBrush(size_t index);
  
  Point hoveredTile(Point pt);
  
  void clickOnTile(Point coords);

public:
  MapEditorView(ViewManager* gvm);
  
  void activate() override;
  void deactivate() override;
  
  bool mousePressed(u16 x, u16 y, MouseButton b) override;
  bool mouseMoved(u16 x, u16 y, MouseButton b) override;
  bool mouseDragged(u16 x, u16 y, MouseButton b) override;
  
  bool mouseWheel(s16 dx, s16 dy, u16 d) override;

};
