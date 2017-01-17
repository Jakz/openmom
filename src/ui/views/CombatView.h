//
//  CombatView.h
//  OpenMoM
//
//  Created by Jack on 7/28/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#ifndef _COMBAT_VIEW_H_
#define _COMBAT_VIEW_H_

#include "View.h"

#include <vector>
#include <memory>

class ViewManager;
class CombatUnit;
class Combat;

//TODO: required for dummyUnit()
class UnitGfxEntry;

using priority_t = s16;

class CombatView : public View
{
public:
  class TileGfxEntry
  {
    priority_t _priority;
  protected:
    TileGfxEntry(priority_t priority) : _priority(priority) { }
  public:
    priority_t priority() const { return _priority; }
    void setPriority(priority_t priority) { _priority = priority; }
    virtual u16 x() const = 0;
    virtual u16 y() const = 0;

    virtual void draw() = 0;
  };
  
private:
  enum
  {
    OPTIONS_OK,
    OPTIONS_CANCEL,
    OPTIONS_DIFFICULTY,
    OPTIONS_OPPONENTS,
    OPTIONS_LAND_SIZE,
    OPTIONS_MAGIC,
    
    BUTTON_COUNT
  };
  
  enum class SubState
  {
    COMBAT,
    SPELL_CAST
  };
  
  void draw() override;
  void drawPost() override { }
  
  Coord hover;
  
  Combat* combat;
    
  
  void drawUnitProps(CombatUnit* unit, bool onTheLeft);
  
  bool entriesDirty;
  std::vector<std::unique_ptr<TileGfxEntry>> entries;
  
  void addGfxEntry(TileGfxEntry* entry)
  {
    entriesDirty = true;
    entries.push_back(std::unique_ptr<TileGfxEntry>(entry));
  }
  
  void addRoads();
  void addMainBuilding(SpriteInfo info);
  void addHouse(SpriteInfo info, int x, int y);
  void prepareGraphics();
  
  UnitGfxEntry* dummyUnit(s16 x, s16 y);
  
public:
  CombatView(ViewManager* gvm);
  
  void activate() override;
  void deactivate() override;
  
  //ScreenCoord coordsForTile(u16 x, u16 y);

  void mouseReleased(u16 x, u16 y, MouseButton b) override;
  void mouseMoved(u16 x, u16 y, MouseButton b) override;
  
};

#endif 
