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
  SpriteInfo info;
  std::function<void(Tile*)> lambda;
};

class MapEditorView : public ViewWithQueue
{
private:
  enum class Mode
  {
    TERRAIN,
    RESOURCES
  };
  
  std::vector<Button*> terrainButtons;
  std::vector<Button*> resourceButtons;
  
  World* world;
  MiniMap* minimap;
  
  void draw() override;
  void drawPost() override { }
  
  void setup();
  
  std::array<Brush, 13>::const_iterator brush;
  std::array<Resource, 11>::const_iterator resource;
  
  Point hover;
  Point offset;
  Plane plane;
  Mode mode;
  
  bool downscaled;
  
  Size tileSize;
  Size viewportSize;
  
  Point positionForBrush(size_t index);
  
  Point hoveredTile(Point pt);
  
  void clickOnTile(Point coords);
  
  void switchMode(Mode mode);
  void toggleDownscale(bool value);

public:
  MapEditorView(ViewManager* gvm);
  
  void activate() override;
  void deactivate() override;
  
  bool mousePressed(u16 x, u16 y, MouseButton b) override;
  bool mouseMoved(u16 x, u16 y, MouseButton b) override;
  bool mouseDragged(u16 x, u16 y, MouseButton b) override;
  
  bool mouseWheel(s16 dx, s16 dy, u16 d) override;
  
  bool keyPressed(KeyboardCode key, KeyboardKey kkey, KeyboardMod mod);

};
