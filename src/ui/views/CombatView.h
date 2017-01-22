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
#include "CombatMap.h"

#include <vector>
#include <memory>

class ViewManager;

//TODO: required for dummyUnit()
class UnitGfxEntry;

using priority_t = s16;

class CombatView : public View
{
public:
  class GfxEntry
  {
    priority_t _priority;
  protected:
    GfxEntry(priority_t priority) : _priority(priority) { }
  public:
    priority_t priority() const { return _priority; }
    void setPriority(priority_t priority) { _priority = priority; }
    virtual u16 x() const = 0;
    virtual u16 y() const = 0;

    virtual void draw() const = 0;
  };
  
private:
  enum
  {
    bt_spell,
    bt_wait,
    bt_info,
    bt_auto,
    bt_flee,
    bt_done,
    
    bt_count
  };
  
  enum class SubState
  {
    COMBAT,
    SPELL_CAST
  };
  
  void draw() override;
  void drawPost() override { }
  
  Coord hover;
  
  combat::Combat* combat;
    
  
  void drawUnitProps(const combat::CombatUnit* unit, bool onTheLeft);
  
  bool entriesDirty;
  std::vector<std::unique_ptr<GfxEntry>> entries;
  
  void addGfxEntry(GfxEntry* entry)
  {
    entriesDirty = true;
    entries.push_back(std::unique_ptr<GfxEntry>(entry));
  }
  
  void addRoads();
  void addFlyingFortress();
  void addMainBuilding(SpriteInfo info);
  void addHouse(SpriteInfo info, int x, int y);
  void prepareGraphics();
  
  UnitGfxEntry* dummyUnit(s16 x, s16 y);
  
  combat::CombatEnvironment environment;
  LBXID environmentLBX;
  
public:
  CombatView(ViewManager* gvm);
  
  void activate() override;
  void deactivate() override;
  
  void setEnvironment(combat::CombatEnvironment env);
  
  //ScreenCoord coordsForTile(u16 x, u16 y);

  void mouseReleased(u16 x, u16 y, MouseButton b) override;
  void mouseMoved(u16 x, u16 y, MouseButton b) override;
  
  
  static ScreenCoord coordsForTile(u16 x, u16 y);
  static Coord tileForCoords(s16 x, s16 y);

  
  constexpr static int TILE_WIDTH = 32;
  constexpr static int TILE_HEIGHT = 16;

};

#endif 
