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

struct Brush
{
  enum class Type
  {
    OCEAN,
    GRASSLANDS
  };
  
  Type type;
  SpriteInfo info;
  std::function<void(Tile*)> lambda;
};

class MapEditorView : public ViewWithQueue
{
private:
  World* world;
  
  void draw() override;
  void drawPost() override { }
  
  void setup();
  
  std::array<Brush, 2>::const_iterator brush;
  Point hover;
  Point offset;
  Plane plane;
  
  Point hoveredTile(Point pt);
  
  void clickOnTile(Point coords);

public:
  MapEditorView(ViewManager* gvm);
  
  void activate() override;
  void deactivate() override { }
  
  bool mouseReleased(u16 x, u16 y, MouseButton b) override;
  bool mouseMoved(u16 x, u16 y, MouseButton b) override;
  bool mouseDragged(u16 x, u16 y, MouseButton b);

};
